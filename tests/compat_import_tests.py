#!/usr/bin/env python3
"""Exercise the compatibility importer through its public CLI and real compiler."""

import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest


IMPORTER = Path(__file__).resolve().parents[1] / "sdk/compat/build_project.py"


class ImportTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="lcom-ng import ")
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name).resolve()
        self.project = self.root / "project with spaces"
        self.project.mkdir()
        self.sdk = self.root / "sdk"
        (self.sdk / "compat/include").mkdir(parents=True)
        (self.sdk / "include").mkdir()
        self.runtime = self.root / "runtime"
        self.runtime.mkdir()
        # These tests cover build inputs and linking, not emulated APIs. Apple ar
        # requires an archive member, unlike GNU ar's empty-archive extension.
        member_source = self.runtime / "placeholder.c"
        member_source.write_text("void lcom_fixture_placeholder(void) {}\n")
        member_object = self.runtime / "placeholder.o"
        subprocess.run([shutil.which("cc"), "-c", str(member_source), "-o", str(member_object)], check=True)
        for name in ("liblcom_minix.a", "libmachinelab.a"):
            subprocess.run(["ar", "rcs", str(self.runtime / name), str(member_object)], check=True)
        self.output = self.root / "output"

    def write(self, path, text):
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)

    def invoke(self, *args, env=None):
        return subprocess.run(
            [sys.executable, str(IMPORTER), str(self.project), "--sdk", str(self.sdk),
             "--runtime-build", str(self.runtime), "--output", str(self.output),
             "--cc", shutil.which("cc"), *args],
            capture_output=True, text=True, env=env)

    def run_binary(self):
        result = subprocess.run([str(self.output / "proj")], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout.strip(), "42")

    def test_macro_arguments_include_paths_and_no_recipe_execution(self):
        self.write(self.project / "inc/config.h", "#define OFFSET 2\n")
        self.write(self.project / "source/main.c", """
#include <stdio.h>
#include <config.h>
#ifdef CLEARED
#error The -U argument was not preserved
#endif
int main(void) { printf("%d\\n", MODE + OFFSET); return 0; }
""")
        marker = self.root / "recipe executed"
        self.write(self.project / "Makefile", f"""
PROG = proj
.PATH: source
SRCS = main.c
CFLAGS += -I inc -D MODE=40 -D CLEARED=1 -U CLEARED
.BEGIN:
\ttouch '{marker}'
all:
\ttouch '{marker}'
.include <minix.lcom.mk>
""")
        before = {p: p.read_bytes() for p in self.project.rglob("*") if p.is_file()}
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()
        self.assertFalse(marker.exists(), "The importer executed a foreign make recipe")
        for path, content in before.items():
            self.assertEqual(path.read_bytes(), content)
        manifest = json.loads((self.output / "build.json").read_text())
        source = self.project / "source/main.c"
        self.assertEqual(manifest["sources"][str(source)], hashlib.sha256(before[source]).hexdigest())

    def test_explicit_object_rules_and_per_source_flags(self):
        self.write(self.project / "main.c", """
#include <stdio.h>
int answer(void);
int main(void) { printf("%d\\n", answer()); return 0; }
""")
        self.write(self.project / "nested/answer.c", """
#ifdef __OPTIMIZE__
#error This translation unit must keep its target-local -O0
#endif
#ifndef TARGET_VALUE
#error The object-specific preprocessor options were not applied
#endif
int answer(void) { return TARGET_VALUE; }
""")
        self.write(self.project / "Makefile", """
PROG = proj
CFLAGS += -O2
main.o: main.c
\t${CC} ${CFLAGS} -c $< -o $@
nested/answer.o: nested/answer.c
nested/answer.o: CFLAGS += -D TARGET_VALUE=42
\t${CC} ${CFLAGS} -O0 -fno-inline -c $< -o $@
.include <minix.lcom.mk>
""")
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()

    def test_separate_source_archives_and_link_dependencies(self):
        self.write(self.project / "main.c", """
#include <stdio.h>
int answer(void);
int main(void) { printf("%d\\n", answer()); return 0; }
""")
        self.write(self.project / "Makefile", """
PROG=proj
SRCS=main.c
LDADD += -L../first -lfirst -L../second -lsecond -llcf
.include <minix.lcom.mk>
""")
        self.write(self.root / "first/Makefile", "LIB_NAME=libfirst.a\nLIB_SRCS=answer.c unused.c\n")
        self.write(self.root / "first/answer.c", "int helper(void); int answer(void) { return helper(); }\n")
        self.write(self.root / "first/unused.c", "int duplicate(void) { return -1; }\n")
        self.write(self.root / "second/Makefile", "LIB=second\nSRCS=helper.c\n")
        self.write(self.root / "second/helper.c", "int duplicate(void) { return 9; } int helper(void) { return 42; }\n")
        self.write(self.root / "first/libfirst.a", "foreign Minix archive must be rebuilt\n")
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()

    def test_explicit_library_and_include_overrides(self):
        self.write(self.root / "drivers/include/answer.h", "#define ANSWER 42\n")
        self.write(self.root / "drivers/answer.c", "#include <answer.h>\nint answer(void) { return ANSWER; }\n")
        self.write(self.project / "main.c", "#include <stdio.h>\nint answer(void); int main(void) { printf(\"%d\\n\", answer()); return 0; }\n")
        for dependency in ("-L./lib -lanswer -llcf", "-llcf"):
            with self.subTest(dependency=dependency):
                self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\nLDADD=" + dependency + "\n")
                result = self.invoke("--library", "answer=../drivers/answer.c", "--include", "../drivers/include")
                self.assertEqual(result.returncode, 0, result.stderr)
                self.run_binary()

    def test_nested_project_resolves_legacy_asset_root(self):
        self.project = self.root / "student workspace/proj/src"
        self.project.mkdir(parents=True)
        self.write(self.project / "assets/answer.txt", "42\n")
        self.write(self.project / "main.c", """
#include <stdio.h>
extern const char *lcom_compat_project_root;
int main(void) {
  char path[4096]; int value = 0;
  snprintf(path, sizeof(path), "%s/proj/src/assets/answer.txt", lcom_compat_project_root);
  FILE *file = fopen(path, "r");
  if (!file || fscanf(file, "%d", &value) != 1) return 3;
  fclose(file); printf("%d\\n", value); return 0;
}
""")
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\n")
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()

    def test_bsd_source_directory_loop_and_header_inputs(self):
        self.write(self.project / "nested/answer.h", "#define ANSWER 42\n")
        self.write(self.project / "nested/answer.c", "#include <answer.h>\nint answer(void) { return ANSWER; }\n")
        self.write(self.project / "main.c", '#include <stdio.h>\nint answer(void); int main(void) { printf("%d\\n", answer()); return 0; }\n')
        self.write(self.project / "Makefile", """
PROG=proj
SRC_DIRS=nested
.for dir in ${SRC_DIRS}
.PATH: ${.CURDIR}/${dir}
CFLAGS += -I${.CURDIR}/${dir}
.endfor
SRCS=main.c answer.c answer.h
""")
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()

    def test_rebuild_archive_from_explicit_object_recipe(self):
        self.write(self.project / "main.c", '#include <stdio.h>\nint answer(void); int main(void) { printf("%d\\n", answer()); return 0; }\n')
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\nLDADD=-L../mouse -lmouse\n")
        self.write(self.root / "mouse/answer.c", "int answer(void) { return 42; }\n")
        self.write(self.root / "mouse/Makefile", """
PROG=lab4
SRCS=lab4.c answer.c
libmouse.a: answer.o
	ar rcs libmouse.a answer.o
lib: libmouse.a
""")
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()

    def test_framework_objects_in_ldadd_are_rebuilt_from_sources(self):
        self.write(self.project / "main.c", '#include <stdio.h>\nint answer(void); int main(void) { printf("%d\\n", answer()); return 0; }\n')
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\nLDADD=../framework/answer.o\nDPADD=../framework/answer.o\n")
        self.write(self.root / "framework/Makefile", "LIB_SRCS=answer.c\nCFLAGS=-DANSWER=42\n")
        self.write(self.root / "framework/answer.c", "int answer(void) { return ANSWER; }\n")
        self.write(self.root / "framework/answer.o", "Foreign Minix object must never be linked\n")
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()

    def test_absolute_minix_library_paths_resolve_under_workspace(self):
        self.write(self.project / "main.c", '#include <stdio.h>\nint answer(void); int main(void) { printf("%d\\n", answer()); return 0; }\n')
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\nLDADD=-L/home/lcom/labs/lab2 -ltimer\n")
        self.write(self.root / "lab2/Makefile", "LIB_NAME=libtimer.a\nLIB_SRCS=answer.c\n")
        self.write(self.root / "lab2/answer.c", "int answer(void) { return 42; }\n")
        result = self.invoke("--asset-root", str(self.root))
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()

    def test_invalid_inputs_have_actionable_errors(self):
        cases = [
            ("SRCS=missing.c\n", (), "missing.c"),
            ("SRCS=main.cpp\n", (), "Unsupported source"),
            ("SRCS=main.c\nLDADD=-lunknown\n", (), "--library"),
            ("SRCS=main.c\n", ("--library", "answer=../missing.c"), "missing.c"),
            ("SRCS=main.c\n", ("--library", "answer"), "name=source"),
            ("SRCS=main.c\nCFLAGS=-D\n", (), "after -D"),
        ]
        self.write(self.project / "main.c", "int main(void) { return 0; }\n")
        for makefile, args, expected in cases:
            with self.subTest(makefile=makefile, args=args):
                self.write(self.project / "Makefile", makefile)
                result = self.invoke(*args)
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(expected, result.stderr)

    def test_failed_rebuild_cannot_run_previous_student_code(self):
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\n")
        self.write(self.project / "main.c", "int main(void) { return 0; }\n")
        self.assertEqual(self.invoke().returncode, 0)
        self.assertTrue((self.output / "proj").is_file())
        self.write(self.project / "main.c", "#error unfinished student implementation\n")
        result = self.invoke()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("compiling main.c", result.stderr)
        self.assertFalse((self.output / "proj").exists())
        report = json.loads((self.output / "build.json").read_text())
        self.assertEqual(report["status"], "failed")
        self.assertIn("compiling main.c", report["error"])

    def test_sanitizer_reports_original_student_memory_error(self):
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\nCFLAGS=-O2\n")
        source = self.project / "main.c"
        sanitizer = "undefined" if sys.platform == "darwin" else "address"
        self.write(source, """
#include <limits.h>
int main(void) { volatile int maximum = INT_MAX; return maximum + 1; }
""" if sanitizer == "undefined" else """
#include <stdlib.h>
int main(void) {
  volatile char *buffer = malloc(1);
  buffer[8] = 42;
  free((void *)buffer);
  return 0;
}
""")
        original = source.read_bytes()
        compiler = "/usr/bin/clang" if sys.platform == "darwin" else shutil.which("cc")
        env = {"PATH": "/usr/bin:/bin:/usr/sbin:/sbin", "LC_ALL": "C"} if sys.platform == "darwin" else None
        result = self.invoke("--cc", compiler, "--debug", "--sanitize", sanitizer, env=env)
        self.assertEqual(result.returncode, 0, result.stderr)
        crashed = subprocess.run([str(self.output / "proj")], capture_output=True, text=True, timeout=30, env=env)
        self.assertNotEqual(crashed.returncode, 0)
        self.assertIn("runtime error: signed integer overflow" if sanitizer == "undefined"
                      else "AddressSanitizer: heap-buffer-overflow", crashed.stderr)
        self.assertEqual(source.read_bytes(), original)


if __name__ == "__main__":
    unittest.main()
