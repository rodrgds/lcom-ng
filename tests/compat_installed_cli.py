#!/usr/bin/env python3
"""Build through a relocated release CLI using its bundled compatibility SDK."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest


class InstalledCliTests(unittest.TestCase):
    def test_installed_sdk_selected_from_path_and_symlinks(self):
        with tempfile.TemporaryDirectory(prefix="lcom-ng installed ") as directory:
            root = Path(directory).resolve()
            release = root / "release"
            sdk = release / "sdk"
            shutil.copytree(SDK_ROOT, sdk)
            for name in ("liblcom_minix.a", "libmachinelab.a"):
                shutil.copy2(LIBRARY_DIR / name, sdk / name)
            (sdk / "compat/include/installed-marker.h").write_text(
                "#define INSTALLED_RUNTIME_MARKER 42\n")
            binary = release / "bin" / ("lcom-ng.exe" if os.name == "nt" else "lcom-ng")
            binary.parent.mkdir(parents=True)
            shutil.copy2(RUNTIME, binary)
            project = root / "student project"
            project.mkdir()
            source = project / "main.c"
            source.write_text("#include <installed-marker.h>\n"
                              "int main(void) { return INSTALLED_RUNTIME_MARKER != 42; }\n")
            makefile = project / "Makefile"
            makefile.write_text("PROG = proj\nSRCS = main.c\n.include <minix.lcom.mk>\n")
            originals = {p: p.read_bytes() for p in (source, makefile)}
            unrelated = root / "unrelated directory"
            unrelated.mkdir()
            env = os.environ.copy()
            env["PATH"] = str(binary.parent) + os.pathsep + env.get("PATH", "")
            invocations = [("path", binary.name),
                           ("relative", os.path.relpath(binary, unrelated))]
            if os.name != "nt":
                links = root / "links"
                links.mkdir()
                alias = links / "student-runtime"
                # MSYS2 defaults to deepcopy for symlink creation. Configure the
                # child at startup to create a real Cygwin/MSYS symbolic link,
                # requiring neither Windows Developer Mode nor admin privileges.
                link_env = env.copy()
                for variable in ("MSYS", "CYGWIN"):
                    options = [word for word in link_env.get(variable, "").split()
                               if not word.startswith("winsymlinks:")]
                    link_env[variable] = " ".join([*options, "winsymlinks:sys"])
                subprocess.run([sys.executable, "-c",
                                "from pathlib import Path; import sys; Path(sys.argv[1]).symlink_to(sys.argv[2])",
                                str(alias), str(binary)], env=link_env, check=True)
                self.assertTrue(alias.is_symlink(), "Fixture must create a symbolic link, not a copied executable")
                invocations.append(("symlink", str(alias)))
            for name, command in invocations:
                with self.subTest(invocation=name):
                    output = root / (name + " output")
                    result = subprocess.run(
                        [command, "compat", "build", str(project), "--output", str(output),
                         "--cc", shutil.which("cc")], cwd=unrelated, env=env,
                        capture_output=True, text=True, timeout=30)
                    self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                    manifest = json.loads((output / "build.json").read_text())
                    compiled = subprocess.run([manifest["binary"]], timeout=10)
                    self.assertEqual(compiled.returncode, 0)
                    self.assertEqual(manifest["sources"][str(source)],
                                     hashlib.sha256(originals[source]).hexdigest())
                    compile_commands = [c for c in manifest["commands"] if "-c" in c]
                    self.assertTrue(compile_commands)
                    for command in compile_commands:
                        self.assertIn(str(sdk / "compat/include"), command)
                    for path, content in originals.items():
                        self.assertEqual(path.read_bytes(), content)


    def test_installed_native_student_setup_build_and_checks(self):
        with tempfile.TemporaryDirectory(prefix="lcom-ng native installed ") as directory:
            root = Path(directory).resolve()
            release = root / "release"
            shutil.copytree(SDK_ROOT / "include", release / "sdk/include")
            with (release / "sdk/include/lcom/lcom.h").open("a") as header:
                header.write("\n#define INSTALLED_RUNTIME_MARKER 42\n")
            shutil.copy2(LIBRARY_DIR / "libmachinelab.a", release / "sdk/libmachinelab.a")
            source = SDK_ROOT.parent
            for owner in ("course", "dev/lab-tests", "scripts"):
                shutil.copytree(source / owner, release / owner)
            binary = release / "bin/lcom-ng"
            binary.parent.mkdir(parents=True)
            shutil.copy2(RUNTIME, binary)
            student = root / "student with spaces"
            setup = subprocess.run([str(binary), "setup", str(student)],
                                   capture_output=True, text=True, timeout=15)
            self.assertEqual(setup.returncode, 0, setup.stdout + setup.stderr)
            (student / "proj/main.c").write_text(
                "#include <lcom/lcom.h>\n"
                "int main(void) { return INSTALLED_RUNTIME_MARKER != 42; }\n")
            build = subprocess.run(["make"], cwd=student, capture_output=True,
                                   text=True, timeout=30)
            self.assertEqual(build.returncode, 0, build.stdout + build.stderr)
            run = subprocess.run([str(student / "build/student_project")], timeout=10)
            self.assertEqual(run.returncode, 0)
            # Fresh TODO code must reach the real predefined assertions.
            check = subprocess.run([str(binary), "test", "rtc", "--project", str(student)],
                                   capture_output=True, text=True, timeout=15)
            self.assertNotEqual(check.returncode, 0)
            self.assertIn("check failed:", check.stdout + check.stderr)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("runtime", type=Path)
    parser.add_argument("sdk", type=Path)
    parser.add_argument("libraries", type=Path)
    args = parser.parse_args()
    RUNTIME = args.runtime.resolve()
    SDK_ROOT = args.sdk.resolve()
    LIBRARY_DIR = args.libraries.resolve()
    unittest.main(argv=[__file__])
