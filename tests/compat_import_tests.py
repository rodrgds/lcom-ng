#!/usr/bin/env python3
"""Exercise the compatibility importer through its public CLI and real compiler."""

import hashlib
import json
from pathlib import Path
import shutil
import shlex
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
        (self.sdk / "compat/include/lcom").mkdir()
        shutil.copy2(IMPORTER.parent / "include/lcom/paths.h", self.sdk / "compat/include/lcom/paths.h")
        self.runtime = self.root / "runtime"
        self.runtime.mkdir()
        # These tests cover build inputs and linking, not emulated APIs. Apple ar
        # requires an archive member, unlike GNU ar's empty-archive extension.
        member_source = self.runtime / "placeholder.c"
        member_source.write_text('#include <stdio.h>\nvoid lcom_fixture_placeholder(void) {}\n'
                                 'FILE *lcom_compat_fopen(const char *path,const char *mode) { return fopen(path,mode); }\n'
                                 'FILE *lcom_compat_freopen(const char *path,const char *mode,FILE *stream) { return freopen(path,mode,stream); }\n')
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

    def test_supported_inline_assembly_preserves_values_evaluation_and_source_identity(self):
        # A real compiler executes generated calls, preserving quoted local includes.
        shutil.copy2(IMPORTER.parent / "include/lcom/compat_asm.h", self.sdk / "compat/include/lcom/compat_asm.h")
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=source/main.c\n")
        self.write(self.project / "source/local.h", '#include "/minix/asm-fixture/answer.h"\n')
        self.write(self.project / "assets/answer.h", "#define ANSWER 42\n")
        self.write(self.project / "lcom.json", json.dumps({"build":{"assets":[],"pathAliases":{"/minix/asm-fixture":"assets"}}}))
        (self.sdk / "export").mkdir()
        shutil.copy2(IMPORTER.parents[1] / "export/config.py", self.sdk / "export/config.py")
        self.write(self.project / "source/main.c", r'''#include <stdio.h>
#include <stdlib.h>
#include "local.h"
static int sequence = 7;
static int port_evals, value_evals, output_evals;
static unsigned short source_port(void){port_evals++;return 0x3ff;}
static unsigned char source_value(void){value_evals++;return 0xe7;}
void lcom_compat_asm_out8(uint16_t port,uint8_t value){if(port!=0x3ff||value!=0xe7)_Exit(8);}
uint8_t lcom_compat_asm_in8(uint16_t port){if(port!=0x3ff)_Exit(9);return 0xa6;}
void lcom_compat_cpu_interrupts(int enabled) { sequence = sequence * 10 + enabled; }
int main(void) {
  const char *literal = "asm(\"unsupported\")";
  /* asm("unsupported") is a comment, not an instruction. */
  asm volatile ("CLI"); __asm__("cli"); __asm ("s" "ti");
  if (sequence != 7001 || literal[0] != 'a') return 1;
  char values[2]={0,0x66};
  asm volatile("outb %0, %1" : : "a"(source_value()), "Nd"(source_port()));
  asm volatile("inb %1, %0" : "=a"(values[(output_evals++,0)]) : "Nd"(source_port()));
  if(port_evals!=2||value_evals!=1||output_evals!=1||(unsigned char)values[0]!=0xa6||values[1]!=0x66)return 2;
  printf("%d\n", ANSWER);
  return 0;
}
''')
        original = (self.project / "source/main.c").read_bytes()
        # This fixture declares an absolute include alias, which requires Clang.
        # Keep ordinary importer tests on the platform's default C compiler.
        clang = shutil.which('clang')
        self.assertIsNotNone(clang, 'Clang is required for the VFS include-alias fixture')
        result = self.invoke('--cc', clang)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()
        self.assertEqual((self.project / "source/main.c").read_bytes(), original)
        # The shared Android/source-export path must retain original hashes and
        # remain buildable after relocation and removal of the input tree.
        script = self.root / "export.py"
        script.write_text("import sys,json\nfrom pathlib import Path\nsys.path.insert(0,sys.argv[1])\nimport project\n"
            "args=project.argument_parser('test').parse_args([sys.argv[2]])\nproject.project_config.resolve(args)\n"
            "out=Path(sys.argv[3]);out.mkdir()\nreport=project.write_native_project(args,out)\n"
            "(out/'report.json').write_text(json.dumps(report))\n")
        output = self.root / 'snapshot'
        result = subprocess.run([sys.executable,str(script),str(IMPORTER.parents[1]/'export'),str(self.project),str(output)],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stdout+result.stderr)
        self.assertEqual(json.loads((output/'report.json').read_text())['sources'][str(self.project/'source/main.c')],hashlib.sha256(original).hexdigest())
        relocated = self.root / 'relocated assembly $ game'
        output.rename(relocated)
        hidden = self.root / 'hidden original'
        self.project.rename(hidden)
        try:
            native=relocated/'native'
            (native/'harness.c').write_text('int lcom_student_main(void); int main(void){return lcom_student_main();}\n')
            (native/'CMakeLists.txt').write_text('cmake_minimum_required(VERSION 3.22)\nproject(AssemblySnapshot C)\nset(LCOM_SDK [=['+str(IMPORTER.parents[1])+']=])\ninclude(student.cmake)\nadd_executable(check harness.c)\ntarget_link_libraries(check PRIVATE student)\n')
            build=self.root/'build assembly snapshot'
            for command in [['cmake','-S',str(native),'-B',str(build),'-G','Ninja','-DCMAKE_C_COMPILER='+clang],['cmake','--build',str(build)]]:
                result=subprocess.run(command,capture_output=True,text=True)
                self.assertEqual(result.returncode,0,result.stdout+result.stderr)
            result=subprocess.run([str(build/'check')],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            self.assertEqual(result.stdout.strip(),'42')
        finally:
            hidden.rename(self.project)
        # Reject unsupported semantics rather than guessing an intended operation.
        cases = [
            ('asm("nop");', 'unsupported inline assembly'),
            ('asm("inb %1, %0" : "=a"(answer) : "Nd"(0x60));', 'requires an explicit volatile'),
            ('asm volatile("inb %1, %0" : "=a"(answer) : "Nd"(0x60));', 'AL operand must have character type'),
            ('asm volatile("outb %0, %1" : : "a"(answer), "Nd"(0x70));', 'AL operand must have character type'),
            ('asm volatile("inb %1, %0" : "=a"(byte) : "Nd"(wide_port));', 'Nd port must be 16-bit'),
            ('asm volatile("inb %1, %0" : "=a"(byte) : "Nd"(256));', 'Nd port must be 16-bit'),
            ('asm volatile("inb %1, %0" : "=a"(flag) : "Nd"(0x60));', 'AL operand must have character type'),
            ('asm volatile("inb %1, %0" : "=r"(byte) : "Nd"(0x60));', 'unsupported byte-port operand constraint'),
            ('asm volatile("outb %0, %1" : : "a"(byte), "Nd"(port) : "memory");', 'unsupported byte-port outputs'),
            ('asm("movl %1, %%eax; movl %%eax, %0;" : "=r"(decimal) : "r"(decimal) : "%eax");', 'unsupported byte-port instruction template'),
        ]
        for body, diagnostic in cases:
            self.write(self.project / "source/main.c", 'int main(void) {\n int answer=0,wide_port=0x3ff; unsigned short port=0x3ff; char byte=0; _Bool flag=0; double decimal=0; '+body+'\n return 0;\n}\n')
            original = (self.project / "source/main.c").read_bytes()
            result = self.invoke()
            self.assertNotEqual(result.returncode, 0)
            self.assertIn(diagnostic, result.stderr)
            self.assertEqual((self.project / "source/main.c").read_bytes(), original)

    def alias_fixture(self):
        # Absolute includes occur in a local header, as in preserved Minix games.
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\n")
        self.write(self.project / "main.c", '#include <stdio.h>\n#include "art.h"\nint main(void) { printf("%d\\n", TOTAL); return 0; }\n')
        self.write(self.project / "art.h", ''.join(f'#include "/minix/student/art/part{i}.h"\n' for i in range(10)) +
                   '#define TOTAL (' + '+'.join(f'PART{i}' for i in range(10)) + ')\n')
        for i in range(10): self.write(self.project / f"art/part{i}.h", f'#define PART{i} {i+1}\n')
        self.write(self.project / "lcom.json", json.dumps({"build": {"assets": [], "pathAliases": {"/minix/student/art": "art"}}}))
        return {str(path): path.read_bytes() for path in self.project.rglob('*') if path.is_file()}

    def test_declared_absolute_includes_compile_without_source_changes(self):
        originals = self.alias_fixture()
        (self.sdk / "export").mkdir()
        shutil.copy2(IMPORTER.parents[1] / "export/config.py", self.sdk / "export/config.py")
        result = self.invoke("--cc", shutil.which("clang"))
        self.assertEqual(result.returncode, 0, result.stderr)
        completed = subprocess.run([str(self.output / "proj")], capture_output=True, text=True, check=True)
        self.assertEqual(completed.stdout.strip(), "55")
        for name, content in originals.items(): self.assertEqual(Path(name).read_bytes(), content)

    def test_absolute_include_alias_rejects_escaping_header_symlinks(self):
        self.alias_fixture()
        outside = self.root / 'private.h'
        outside.write_text('#define PART0 1\n')
        header = self.project / 'art/part0.h'
        header.unlink()
        header.symlink_to(outside)
        (self.sdk / 'export').mkdir()
        shutil.copy2(IMPORTER.parents[1] / 'export/config.py', self.sdk / 'export/config.py')
        result = self.invoke('--cc', shutil.which('clang'))
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('Absolute include alias escapes its asset directory', result.stderr)
        self.assertFalse((self.output / 'proj').exists())

    def test_absolute_include_snapshot_relocates_with_all_headers(self):
        originals = self.alias_fixture()
        # Exercise the same source exporter used by Android, without compiling
        # the unrelated runtime. Delete the input before a real CMake build.
        script = self.root / "export.py"
        script.write_text("""import sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
import project
args = project.argument_parser('test').parse_args([sys.argv[2]])
project.project_config.resolve(args)
out = Path(sys.argv[3]); out.mkdir()
project.write_native_project(args, out)
""")
        output = self.root / 'snapshot'
        result = subprocess.run([sys.executable, str(script), str(IMPORTER.parents[1] / 'export'),
                                 str(self.project), str(output)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        for name, content in originals.items(): self.assertEqual(Path(name).read_bytes(), content)
        # Each independent constant contributes to the observable total, so a
        # missing mapped header or wrong mapping cannot produce a passing build.
        relocated = self.root / 'relocated snapshot $ game'
        output.rename(relocated)
        shutil.rmtree(self.project)
        native = relocated / 'native'
        sdk = IMPORTER.parents[1]
        (native / 'harness.c').write_text('int lcom_student_main(void); int main(void) { return lcom_student_main(); }\n')
        (native / 'CMakeLists.txt').write_text('cmake_minimum_required(VERSION 3.22)\nproject(AliasSnapshot C)\n'
            + 'set(LCOM_SDK [=[' + str(sdk) + ']=])\ninclude(student.cmake)\n'
            + 'add_executable(check harness.c)\ntarget_link_libraries(check PRIVATE student)\n')
        build = self.root / 'build snapshot'
        for command in [['cmake', '-S', str(native), '-B', str(build), '-G', 'Ninja', '-DCMAKE_C_COMPILER=' + shutil.which('clang')],
                        ['cmake', '--build', str(build)]]:
            completed = subprocess.run(command, capture_output=True, text=True)
            self.assertEqual(completed.returncode, 0, completed.stdout + completed.stderr)
        completed = subprocess.run([str(build / 'check')], capture_output=True, text=True, check=True)
        self.assertEqual(completed.stdout.strip(), '55')
        import os
        if shutil.which('emcc') and shutil.which('node'):
            source = next((native / 'sources').rglob('main.c'))
            wasm = self.root / 'mapped-wasm.js'
            subprocess.run(['emcc', '-ivfsoverlay' + str(native / 'include-aliases.json'), str(source), '-o', str(wasm)], check=True)
            completed = subprocess.run(['node', str(wasm)], capture_output=True, text=True, check=True)
            self.assertEqual(completed.stdout.strip(), '55')
        android = os.environ.get('ANDROID_HOME') or os.environ.get('ANDROID_SDK_ROOT')
        if android:
            compilers = sorted(Path(android).glob('ndk/*/toolchains/llvm/prebuilt/*/bin/aarch64-linux-android24-clang'))
            if compilers:
                source = next((native / 'sources').rglob('main.c'))
                subprocess.run([str(compilers[-1]), '-ivfsoverlay' + str(native / 'include-aliases.json'),
                                '-c', str(source), '-o', str(self.root / 'mapped-android.o')], check=True)

    def test_minix_default_optimization_resolves_original_inline_calls(self):
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\n.include <minix.lcom.mk>\n")
        # The original C99 inline helper has no separate external definition.
        # Minix sys.mk's normal -O2 build resolves the call without rewriting it.
        self.write(self.project / "main.c", '#include <stdio.h>\ninline int value(int x){return x+41;}\nint main(int argc,char**argv){printf("%d\\n",value(argc));return 0;}\n')
        original = (self.project / "main.c").read_bytes()
        # Normalize only this fixture compiler baseline: Nix hardening may
        # prepend -O2 even when the original Makefile deliberately leaves flags empty.
        result = self.invoke("--cc", shlex.join([shutil.which("cc"), "-O0"]))
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.run_binary()
        self.assertEqual(original, (self.project / "main.c").read_bytes())

    def test_minix_optimization_defaults_preserve_authored_and_debug_priorities(self):
        # Observe real compiler semantics, rather than the importer's flag list.
        # A leading compiler -O0 makes its baseline independent of host hardening.
        source = r'''#include <stdio.h>
#if EXPECT_OPTIMIZED
#ifndef __OPTIMIZE__
#error Original Minix default optimization was lost
#endif
#else
#ifdef __OPTIMIZE__
#error Authored or requested unoptimized build was overridden
#endif
#endif
#if EXPECT_SIZE
#ifndef __OPTIMIZE_SIZE__
#error Authored size optimization was overridden
#endif
#endif
int main(void){puts("42");return 0;}
'''
        self.write(self.project / "main.c", source)
        cases = [
            ("default", "", "", 1, 0, ()),
            ("append", "CFLAGS += -fno-unroll-loops\n", "", 1, 0, ()),
            ("explicit-empty", "CFLAGS=\n", "", 0, 0, ()),
            ("explicit-O0", "CFLAGS=-O0\n", "", 0, 0, ()),
            ("authored-DBG", "DBG=-O0\n", "", 0, 0, ()),
            ("authored-Os", "CFLAGS=-Os\n", "", 1, 1, ()),
            ("target-O0", "main.o: CFLAGS=-O0\n", "main.o: main.c\n", 0, 0, ()),
            ("recipe-O0", "", "main.o: main.c\n\t${CC} ${CFLAGS} -O0 -c main.c -o main.o\n", 0, 0, ()),
            ("debug", "CFLAGS=-O3\n", "", 0, 0, ("--debug",)),
        ]
        for name, variables, recipe, optimized, size, arguments in cases:
            with self.subTest(name=name):
                self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\n" + variables +
                           f"CPPFLAGS += -DEXPECT_OPTIMIZED={optimized} -DEXPECT_SIZE={size}\n" + recipe)
                result = self.invoke("--cc", shlex.join([shutil.which("cc"), "-O0"]), *arguments)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.run_binary()

    def test_original_lcf_random_alias_respects_authored_build_flags(self):
        shutil.copytree(IMPORTER.parent / "include", self.sdk / "compat/include", dirs_exist_ok=True)
        shutil.copytree(IMPORTER.parents[1] / "include", self.sdk / "include", dirs_exist_ok=True)
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c random.c plain.c\nCPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__\n")
        # The original LCF macro permits an authored local variable named rand.
        self.write(self.project / "random.c", '#include <lcom/lcf.h>\nint choose(void) { int(*libc_random)(void)=rand; int rand = rand() % 26; return libc_random()>=0?rand:0; }\n')
        # A pure libc source must retain its normal rand and function-address API.
        self.write(self.project / "plain.c", '#include <stdlib.h>\n#ifdef rand\n#error LCF alias escaped its owning header\n#endif\nint (*plain_random)(void)=rand;\nint plain(void){srand(7);return plain_random()>=0;}\n')
        self.write(self.project / "main.c", '#include <stdio.h>\nint choose(void);int plain(void);int rand_(void){return 41;}\nint main(void){printf("%d\\n",choose()+27*plain());return 0;}\n')
        before = {p:p.read_bytes() for p in self.project.iterdir()}
        for flags in ("-D __LCOM_OPTIMIZED__", "-D __LCOM__"):
            with self.subTest(flags=flags):
                self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c random.c plain.c\nCPPFLAGS += -pedantic " + flags + "\n")
                result = self.invoke()
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.run_binary()
        self.assertEqual(before[self.project / "random.c"], (self.project / "random.c").read_bytes())
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c random.c plain.c\n")
        self.write(self.project / "random.c", '#include <lcom/lcf.h>\n#ifdef rand\n#error Uninstrumented LCF must preserve libc rand\n#endif\nint choose(void){int(*libc_random)(void)=rand;return libc_random()>=0?15:0;}\n')
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.run_binary()

    def test_lcf_random_seed_lifetime_and_reference_stream(self):
        sdk = IMPORTER.parent.parent
        implementation = self.root / "lcf.o"
        common = [shutil.which("cc"), "-std=gnu11", "-ffunction-sections", "-fdata-sections",
                  "-I", str(sdk / "compat/include"), "-I", str(sdk / "include")]
        compiled = subprocess.run(common + ["-Dfread=lcom_test_entropy_read",
            "-Dfclose=lcom_test_entropy_close", "-c", str(sdk / "compat/src/lcf.c"),
            "-o", str(implementation)], capture_output=True, text=True)
        self.assertEqual(compiled.returncode, 0, compiled.stderr)
        binary = self.root / "random client"
        dead_strip = "-Wl,-dead_strip" if sys.platform == "darwin" else "-Wl,--gc-sections"
        linked = subprocess.run(common + [str(Path(__file__).with_name("compat_lcf_rand.c")),
            str(implementation), dead_strip, "-o", str(binary)], capture_output=True, text=True)
        self.assertEqual(linked.returncode, 0, linked.stderr)
        result = subprocess.run([str(binary)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        # A failed entropy read must not silently manufacture a deterministic seed.
        failed_entropy = subprocess.run([str(binary), "short-read"], capture_output=True, text=True)
        self.assertNotEqual(failed_entropy.returncode, 0)

    def test_profile_c_standard_preserves_old_inline_emission_and_modern_default(self):
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\n")
        (self.sdk / "export").mkdir()
        shutil.copy2(IMPORTER.parents[1] / "export/config.py", self.sdk / "export/config.py")
        # C99 emits an external definition for extern inline. The default stays modern.
        self.write(self.project / "main.c", '#include <stdio.h>\nextern inline int value(void){return 42;}\nint(*volatile fn)(void)=value;int main(void){printf("%d\\n",fn());return 0;}\n')
        modern = self.invoke()
        self.assertEqual(modern.returncode, 0, modern.stdout + modern.stderr)
        self.run_binary()
        # The original Minix3.1.8 GCC4 dialect emits plain inline instead.
        self.write(self.project / "lcom.json", json.dumps({"build":{"cStandard":"gnu89"}}))
        self.write(self.project / "main.c", '#include <stdio.h>\ninline int value(void){return 42;}\nint(*volatile fn)(void)=value;int main(void){printf("%d\\n",fn());return 0;}\n')
        legacy = self.invoke()
        self.assertEqual(legacy.returncode, 0, legacy.stdout + legacy.stderr)
        self.run_binary()

    def test_minix_reallocarr_preserves_pointer_contents_and_errno(self):
        sdk = IMPORTER.parent.parent
        source = self.root / "array client.c"
        source.write_text("""#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
static int fail_next;
void *lcom_test_realloc(void *pointer,size_t size) {
  if(fail_next) { fail_next=0;errno=ENOMEM;return NULL; }
  return realloc(pointer,size);
}
int main(void) {
  int *array=NULL; errno=EDOM;
  if(reallocarr(&array,4,sizeof(*array)) || !array || errno!=EDOM) return 1;
  for(int i=0;i<4;i++)array[i]=40+i;
  if(reallocarr(&array,8,sizeof(*array)) || errno!=EDOM) return 2;
  for(int i=0;i<4;i++)if(array[i]!=40+i)return 3;
  int *before=array;
  fail_next=1;
  if(reallocarr(&array,16,sizeof(*array))!=ENOMEM || array!=before || errno!=EDOM)return 9;
  if(reallocarr(&array,SIZE_MAX,2)!=EOVERFLOW || array!=before || errno!=EDOM) return 4;
  for(int i=0;i<4;i++)if(array[i]!=40+i)return 5;
  if(reallocarr(&array,0,sizeof(*array)) || array || errno!=EDOM)return 6;
  if(reallocarr(&array,2,sizeof(*array)) || !array)return 7;
  if(reallocarr(&array,4,0) || array || errno!=EDOM)return 8;
  return 0;
}
""")
        common=[shutil.which("cc"),"-std=gnu11","-ffunction-sections","-fdata-sections",
            "-I",str(sdk/"compat/include"),"-I",str(sdk/"include")]
        implementation=self.root/"stdlib compatibility.o"
        compiled=subprocess.run(common+["-Drealloc=lcom_test_realloc","-c",str(sdk/"compat/src/compat.c"),"-o",str(implementation)],capture_output=True,text=True)
        self.assertEqual(compiled.returncode,0,compiled.stderr)
        binary=self.root/"array client"
        dead_strip="-Wl,-dead_strip" if sys.platform=="darwin" else "-Wl,--gc-sections"
        linked=subprocess.run(common+[str(source),str(implementation),dead_strip,"-o",str(binary)],capture_output=True,text=True)
        self.assertEqual(linked.returncode,0,linked.stderr)
        result=subprocess.run([str(binary)],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)

    def test_wall_clock_preserves_student_time_object_and_host_calendar(self):
        # Legacy games may own a global `time` object. Host clock APIs must
        # coexist with it and retain time_t/output/error semantics.
        sdk = IMPORTER.parent.parent
        root = sdk.parent
        self.write(self.root / "guest.c", "void *time=(void *)42;\nint guest_time_intact(void) { return time==(void *)42; }\n")
        self.write(self.root / "clock.c", r'''#include <time.h>
#include <errno.h>
time_t lcom_compat_time(time_t *);
int guest_time_intact(void);
static int fail;
int lcom_fixture_clock_gettime(clockid_t clock, struct timespec *value) {
  if(clock!=CLOCK_REALTIME) return -1;
  if(fail) { errno=EIO; return -1; }
  value->tv_sec=(time_t)2200000000LL; value->tv_nsec=999999999;
  return 0;
}
int main(void) {
  time_t output=17;
  if(sizeof(time_t)!=8 || lcom_compat_time(&output)!=(time_t)2200000000LL || output!=(time_t)2200000000LL) return 1;
  if(lcom_compat_time(0)!=(time_t)2200000000LL) return 2;
  fail=1; output=19; errno=0;
  if(lcom_compat_time(&output)!=(time_t)-1 || output!=19 || errno!=EIO) return 3;
  return !guest_time_intact();
}
''')
        self.write(self.root / "calendar.cpp", r'''#include "runtime/devices/RtcCmos.hpp"
#include <lcom/rtc.h>
#include <chrono>
#include <ctime>
extern "C" int guest_time_intact(void);
static bool matches(lcom::RtcCmos &rtc, const std::tm &calendar) {
  const unsigned char regs[]={RTC_REG_SECONDS,RTC_REG_MINUTES,RTC_REG_HOURS,RTC_REG_DAY,RTC_REG_MONTH,RTC_REG_YEAR};
  const int expected[]={calendar.tm_sec,calendar.tm_min,calendar.tm_hour,calendar.tm_mday,calendar.tm_mon+1,(calendar.tm_year+1900)%100};
  for(unsigned i=0;i<6;++i) {
    unsigned char value=0;
    if(!rtc.write8(RTC_ADDR_REG,regs[i]) || !rtc.read8(RTC_DATA_REG,value) || value!=expected[i]) return false;
  }
  return true;
}
int main(void) {
  auto before=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm first=*std::localtime(&before);
  lcom::IrqController irq; lcom::RtcCmos rtc(irq); rtc.setBinaryMode(true);
  auto after=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm last=*std::localtime(&after);
  return !(guest_time_intact() && (matches(rtc,first)||matches(rtc,last)));
}
''')
        common=["-ffunction-sections","-fdata-sections","-I"+str(sdk/"compat/include"),"-I"+str(sdk/"include")]
        def compile_object(source, name, extra=(), cpp=False):
            output=self.root/name
            result=subprocess.run([shutil.which("c++" if cpp else "cc"),*common,*extra,"-c",str(source),"-o",str(output)],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            return str(output)
        guest=compile_object(self.root/"guest.c","guest.o")
        compat=compile_object(sdk/"compat/src/compat.c","compat.o",["-Dclock_gettime=lcom_fixture_clock_gettime"])
        clock=compile_object(self.root/"clock.c","clock.o")
        rtc=compile_object(root/"runtime/devices/RtcCmos.cpp","rtc.o",["-std=c++17"],True)
        irq=compile_object(root/"runtime/core/IrqController.cpp","irq.o",["-std=c++17"],True)
        calendar=compile_object(self.root/"calendar.cpp","calendar.o",["-std=c++17","-I"+str(root)],True)
        strip="-Wl,-dead_strip" if sys.platform=="darwin" else "-Wl,--gc-sections"
        for name,objects in [("clock",[guest,compat,clock]),("calendar",[guest,rtc,irq,calendar])]:
            with self.subTest(owner=name):
                binary=self.root/name
                linked=subprocess.run([shutil.which("c++"),*objects,strip,"-o",str(binary)],capture_output=True,text=True)
                self.assertEqual(linked.returncode,0,linked.stderr)
                result=subprocess.run([str(binary)],capture_output=True,text=True)
                self.assertEqual(result.returncode,0,result.stderr)

    def test_original_lcf_umbrella_exposes_complete_calendar_types(self):
        sdk = IMPORTER.parent.parent
        source = self.root / "calendar client.c"
        source.write_text("""#include <lcom/lcf.h>
int main(void) {
  struct tm date = {0};
  date.tm_year=124; date.tm_mon=1; date.tm_mday=29; date.tm_hour=12;
  time_t epoch=mktime(&date);
  if (epoch != (time_t)1709208000) return 1;
  struct tm *round=localtime(&epoch);
  if (!round || round->tm_year!=124 || round->tm_mon!=1 || round->tm_mday!=29 || round->tm_hour!=12) return 2;
  return 0;
}
""")
        binary = self.root / "calendar client"
        compiled = subprocess.run([shutil.which("cc"), "-std=gnu11",
            "-I",str(sdk/"compat/include"),"-I",str(sdk/"include"),str(source),"-o",str(binary)],
            capture_output=True,text=True)
        self.assertEqual(compiled.returncode,0,compiled.stderr)
        import os
        result=subprocess.run([str(binary)],env={**os.environ,"TZ":"UTC"},capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)

    def test_text_video_generation_uses_old_memory_types_but_preserves_modern_lcf(self):
        # The course libvt contract predates LCF. Its local header plus an actual
        # old memory-range consumer must select the existing old header ABI.
        shutil.copytree(IMPORTER.parent / "include", self.sdk / "compat/include", dirs_exist_ok=True)
        shutil.copytree(IMPORTER.parents[1] / "include", self.sdk / "include", dirs_exist_ok=True)
        self.write(self.project / "vt_info.h", (IMPORTER.parent / "include/vt_info.h").read_text())
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\n")
        for modern in (False, True):
            with self.subTest(modern=modern):
                if modern:
                    source = '#include <lcom/lcf.h>\n#include "vt_info.h"\n#include <stdio.h>\nstruct mem_range *old_documented_range;\nint main(void){mmap_t region={0};region.phys=42;printf("%d\\n",(int)region.phys);return 0;}\n'
                else:
                    source = '#include <minix/drivers.h>\n#include <sys/mman.h>\n#include "vt_info.h"\n#include <stdio.h>\nint main(void){struct mem_range range;vt_info_t display={0};range.mr_base=0xb8000;range.mr_limit=range.mr_base+4000;display.scr_width=80;display.scr_lines=25;printf("%d\\n",(int)(range.mr_limit-range.mr_base)/100+2);return 0;}\n'
                self.write(self.project / "main.c", source)
                result = self.invoke()
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.run_binary()

    def test_legacy_umbrella_only_reserves_names_in_sources_that_import_it(self):
        shutil.copytree(IMPORTER.parent / "include", self.sdk / "compat/include", dirs_exist_ok=True)
        self.write(self.sdk / "compat/include/lcom/compat.h", '#pragma once\n#include <stdbool.h>\n#define OK 0\nstruct reg86u { int value; };\n')
        self.write(self.sdk / "compat/include/minix/drivers.h", '#include <lcom/compat.h>\n')
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c worker.c\n")
        self.write(self.project / "main.c", '#include <minix/drivers.h>\n#include <stdio.h>\nint worker(void);\nint main(void) { struct reg86u registers={0}; printf("%d\\n",worker()+OK+registers.value); return 0; }\n')
        self.write(self.project / "worker.c", 'const char OK=2;\nint bool=3;\nint worker(void) { return 40+OK+bool-3; }\n')
        original = {p:p.read_bytes() for p in self.project.iterdir()}
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()
        self.assertEqual(original, {p:p.read_bytes() for p in self.project.iterdir()})

    def test_legacy_mman_owns_guest_types_and_preserves_host_mapping(self):
        sdk = IMPORTER.parent.parent
        source = self.root / "legacy mapping client.c"
        source.write_text("""#include <sys/mman.h>
_Static_assert(sizeof(phys_bytes) == 4, "Minix guest physical addresses are 32-bit");
int main(void) {
  phys_bytes address = 0xe0000000u;
  void *page = mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if (page == MAP_FAILED || address != 0xe0000000u) return 1;
  ((volatile unsigned char *)page)[4095] = 42;
  if (((volatile unsigned char *)page)[4095] != 42) return 2;
  return munmap(page, 4096) != 0;
}
""")
        binary = self.root / "legacy mapping client"
        compiled = subprocess.run([shutil.which("cc"), "-std=gnu11", "-DLCOM_COMPAT_LEGACY=1",
                                   "-I", str(sdk/"compat/include"), "-I", str(sdk/"include"),
                                   str(source), "-o", str(binary)], capture_output=True, text=True)
        self.assertEqual(compiled.returncode, 0, compiled.stderr)
        self.assertEqual(subprocess.run([str(binary)]).returncode, 0)

    def test_minlib_conversion_name_belongs_to_explicit_minix_headers(self):
        sdk = IMPORTER.parent.parent
        # Original installed LCF headers permit a student-owned two-argument itoa.
        for header in ("lcom/lcf.h", "minix/driver.h", "minix/minlib.h", "minix/drivers.h"):
            with self.subTest(header=header):
                source = self.root / "conversion owner.c"
                if header in ("lcom/lcf.h", "minix/driver.h"):
                    contract = "char *itoa(int value,int base);\nchar *(*conversion)(int,int)=itoa;\n"
                else:
                    contract = "char *(*conversion)(int)=itoa;\n"
                source.write_text('#include <' + header + '>\n' + contract)
                compiled = subprocess.run([shutil.which("cc"), "-std=gnu11",
                    "-Werror=incompatible-pointer-types", "-Werror=implicit-function-declaration",
                    "-I", str(sdk/"compat/include"), "-I", str(sdk/"include"),
                    "-c", str(source), "-o", str(self.root/"owner.o")], capture_output=True, text=True)
                self.assertEqual(compiled.returncode, 0, compiled.stderr)

    def test_legacy_libc_prelude_keeps_minlib_conversion_prototype(self):
        sdk = IMPORTER.parent.parent
        source = self.root / "pure conversion client.c"
        source.write_text("""const char OK=0;
int bool=0;
char *(*conversion)(int) = itoa;
char *convert_score(int score) { return conversion(score+OK+bool); }
""")
        compiled = subprocess.run([shutil.which("cc"), "-std=gnu11", "-DLCOM_COMPAT_LEGACY=1",
                                   "-I", str(sdk/"compat/include"), "-I", str(sdk/"include"),
                                   "-include", "lcom/libc.h", "-Werror=implicit-function-declaration",
                                   "-Werror=incompatible-pointer-types", "-c", str(source),
                                   "-o", str(self.root/"conversion.o")], capture_output=True, text=True)
        self.assertEqual(compiled.returncode, 0, compiled.stderr)

    def test_project_callback_adapts_only_positive_zero_argument_definitions(self):
        shutil.copy2(IMPORTER.parent / "include/lcom/compat.h", self.sdk / "compat/include/lcom/compat.h")
        # A small owning header models LCF's fixed callback declaration. The
        # generated adapter must call each actual authored signature correctly.
        header = self.sdk / "compat/include/lcom/entry.h"
        header.write_text('#ifdef LCOM_COMPAT_PROJECT_NO_ARGS\nint proj_main_loop(void);\n#else\nint proj_main_loop(int,char **);\n#endif\n')
        dispatch = self.runtime / "dispatch.c"
        dispatch.write_text('int proj_main_loop(int,char **); int fixed_entry(void){return proj_main_loop(1,0);}')
        obj = self.runtime / "dispatch.o"
        subprocess.run([shutil.which("cc"), "-c", str(dispatch), "-o", str(obj)], check=True)
        subprocess.run(["ar", "rcs", str(self.runtime / "liblcom_minix.a"), str(obj)], check=True)
        for signature, result in (("()", "42"), ("(void)", "42"), ("(int argc,char **argv)", "argc+41")):
            with self.subTest(signature=signature):
                self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c game.c\n")
                self.write(self.project / "game.c", '#include <lcom/entry.h>\nint(proj_main_loop)' + signature + ' {return ' + result + ';}\n')
                self.write(self.project / "main.c", '#include <stdio.h>\nint fixed_entry(void);\nint main(void){printf("%d\\n",fixed_entry());return 0;}\n')
                before = (self.project / "game.c").read_bytes()
                built = self.invoke()
                self.assertEqual(built.returncode, 0, built.stderr)
                self.run_binary()
                self.assertEqual((self.project / "game.c").read_bytes(), before)

    def test_authored_legacy_timer_fields_keep_their_numeric_contract(self):
        self.write(self.sdk / "compat/include/lcom/timer.h", '#define __TIMER_H\nenum timer_status_field {tsf_all,tsf_initial,tsf_mode,tsf_base\n#ifdef LCOM_COMPAT_TIMER_LEGACY_FIELDS\n,all=tsf_all,initial=tsf_initial,mode=tsf_mode,base=tsf_base\n#endif\n};\n')
        self.write(self.project / "timer.h", '#ifndef __TIMER_H\n#define __TIMER_H\nenum timer_status_field {all,initial,mode,base};\n#endif\n')
        self.write(self.project / "Makefile", 'PROG=proj\nSRCS=main.c\n')
        self.write(self.project / "main.c", '#include <lcom/timer.h>\n#include "timer.h"\n#include <stdio.h>\n#include <assert.h>\nint main(void){assert(all==0 && initial==1 && mode==2 && base==3);printf("%d\\n",36+all+initial+mode+base);return 0;}\n')
        before = (self.project / "timer.h").read_bytes()
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()
        self.assertEqual((self.project / "timer.h").read_bytes(), before)

    def test_relative_program_target_stays_in_owned_output_directory(self):
        self.write(self.project / "main.c", '#include <stdio.h>\nint main(void) { puts("42"); return 0; }\n')
        self.write(self.project / "Makefile", "PROG=../bin/2048\nSRCS=main.c\n")
        before = {p: p.read_bytes() for p in self.project.rglob("*") if p.is_file()}
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        executable = self.output / "2048"
        result = subprocess.run([str(executable)], capture_output=True, text=True)
        self.assertEqual((result.returncode, result.stdout.strip()), (0, "42"))
        self.assertFalse((self.root / "bin").exists())
        self.assertEqual(before, {p: p.read_bytes() for p in self.project.rglob("*") if p.is_file()})
        self.write(self.project / "Makefile", "PROG=/tmp/2048\nSRCS=main.c\n")
        result = self.invoke()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("Invalid PROG", result.stderr)

    def test_minix_signed_char_default_and_authored_override(self):
        # ARM Android defaults to unsigned char. Simulate that toolchain on any
        # host, then compile and run the actual imported program.
        wrapper = self.root / "unsigned-default-cc"
        compiler = shutil.which("cc")
        wrapper.write_text("#!/usr/bin/env python3\nimport os,sys\nos.execv(" + repr(compiler) +
                           ",[" + repr(compiler) + ",'-funsigned-char',*sys.argv[1:]])\n")
        wrapper.chmod(0o755)
        self.write(self.project / "main.c", '#include <stdio.h>\nint main(void) { volatile char missing=-1; printf("%d\\n",(int)missing); return 0; }\n')
        for flags, expected in (("", "-1"), ("-funsigned-char", "255")):
            with self.subTest(flags=flags):
                self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\nCFLAGS=" + flags + "\n")
                result = self.invoke("--cc", shlex.quote(str(wrapper)))
                self.assertEqual(result.returncode, 0, result.stderr)
                result = subprocess.run([str(self.output / "proj")], capture_output=True, text=True)
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertEqual(result.stdout.strip(), expected)

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

    def test_wildcard_source_lists_use_project_directory_without_shell(self):
        self.write(self.project / 'main.c', '#include <stdio.h>\nint answer(void);\nint main(void) { printf("%d\\n",answer()); return 0; }\n')
        self.write(self.project / 'nested/answer.c', 'int answer(void) { return 42; }\n')
        self.write(self.project / 'makefile', 'PROG=proj\nSUBDIR=nested\nSRCS=$(wildcard *.c $(SUBDIR)/*.c)\n')
        result=self.invoke()
        self.assertEqual(result.returncode,0,result.stderr)
        self.run_binary()

    def test_declarative_find_source_lists_compile_nested_sources(self):
        self.write(self.project / 'src/main.c', '#include <stdio.h>\nint answer(void);\nint main(void) { printf("%d\\n",answer()); return 0; }\n')
        self.write(self.project / 'src/nested/answer.c', 'int answer(void) { return 42; }\n')
        self.write(self.project / 'src/ignored.h', '#error headers are not translation units\n')
        self.write(self.project / 'Makefile', "PROG=proj\nSRCS=$(shell find src -type f -name '*.c')\n")
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()

    def test_bsd_find_assignment_preserves_top_level_and_immediate_semantics(self):
        self.write(self.project / 'src/main.c', '#include <stdio.h>\nint answer(void);\nint main(void) { printf("%d\\n",answer()); return 0; }\n')
        self.write(self.project / 'src/answer.c', 'int answer(void) { return 42; }\n')
        self.write(self.project / 'src/nested/ignored.c', '#error maxdepth1 must exclude nested sources\n')
        self.write(self.project / 'other/ignored.c', '#error BSD shell assignment uses its current directory value\n')
        self.write(self.project / 'Makefile', "PROG=proj\nSRC_DIR=src\nSRCS != find $(SRC_DIR) -maxdepth 1 -type f -name '*.c'\nSRC_DIR=other\n")
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()

    def test_unused_bsd_object_directory_shell_assignment_is_not_a_source_query(self):
        self.write(self.project / 'main.c', '#include <stdio.h>\nint answer(void);int main(void){printf("%d\\n",answer());return 0;}\n')
        self.write(self.project / 'src/answer.c', 'int answer(void){return 42;}\n')
        # Bomberlab uses this exact BSD object-directory expression. The
        # exporter owns build outputs, so .OBJDIR is never a required input.
        bookkeeping = ('BUILD_DIR ?= build\n'
                       'BUILD_DIR_PATH != mkdir -p ${.CURDIR}/${BUILD_DIR} && echo ${.CURDIR}/${BUILD_DIR}\n'
                       '.OBJDIR: ${BUILD_DIR_PATH}\n')
        makefile = 'PROG=proj\n' + bookkeeping + '.PATH: src\nSRCS=main.c answer.c\n'
        self.write(self.project / 'Makefile', makefile)
        before = {path:path.read_bytes() for path in self.project.rglob('*') if path.is_file()}
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()
        self.assertFalse((self.project / 'build').exists(), 'Foreign object-directory shell code was executed')
        for path, content in before.items():
            self.assertEqual(path.read_bytes(), content)
        # An opaque computation must still be rejected if source discovery or
        # compiler flags actually require its result.
        for required in ('SRCS=${BUILD_DIR_PATH}\n', 'SRCS=main.c answer.c\nCFLAGS=${BUILD_DIR_PATH}\n'):
            with self.subTest(required=required):
                self.write(self.project / 'Makefile', 'PROG=proj\n' + bookkeeping + '.PATH: src\n' + required)
                result = self.invoke()
                self.assertNotEqual(result.returncode, 0)
                self.assertIn('Unsupported shell source discovery', result.stderr)
                self.assertFalse((self.project / 'build').exists())

    def test_declarative_find_rejects_shell_commands_and_escaping_paths(self):
        marker = self.root / 'executed'
        self.write(self.project / 'src/main.c', 'int main(void) { return 0; }\n')
        for command in ("find ../ -type f -name '*.c'", "find /tmp -type f -name '*.c'",
                        "find src -type f -name '*.c'; touch " + shlex.quote(str(marker)),
                        "printf main.c", "find src -type f -name '*.c' -exec echo {} \\;"):
            for assignment in ('SRCS=$(shell ' + command + ')', 'SRCS != ' + command):
                with self.subTest(assignment=assignment):
                    self.write(self.project / 'Makefile', 'PROG=proj\n' + assignment + '\n')
                    result = self.invoke()
                    self.assertNotEqual(result.returncode, 0)
                    self.assertFalse(marker.exists(), 'Source discovery executed shell code')

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
SRCS = main.c answer.c
unused.o: stale/missing.c
unused.o: CFLAGS += -DBROKEN_UNUSED_RULE
CFLAGS += -O2
main.o: main.c
\t${CC} ${CFLAGS} -c $< -o $@
answer.o: nested/answer.c
answer.o: CFLAGS += -D TARGET_VALUE=42
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

    def test_minix_archive_names_preserve_source_precedence_and_link_failures(self):
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\nLDADD=-llm liblm.a -llcf liblcf.a -lxpm libxpm.a\n")
        self.write(self.project / "main.c", '#include <stdio.h>\nint main(void){puts("42");return 0;}\n')
        built = self.invoke()
        self.assertEqual(built.returncode, 0, built.stderr)
        self.run_binary()
        # The SDK only supplies its implemented APIs, not arbitrary symbols.
        self.write(self.project / "main.c", '#include <stdio.h>\nint custom_lm_value(void);int main(void){printf("%d\\n",custom_lm_value());return 0;}\n')
        missing = self.invoke()
        self.assertNotEqual(missing.returncode, 0)
        self.assertIn("custom_lm_value", missing.stderr)
        self.assertNotIn("Cannot rebuild", missing.stderr)
        # Authored sources win over recognizing an SDK dependency spelling.
        self.write(self.project / "answer.c", 'int custom_lm_value(void){return 42;}\n')
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\nLIB_lm=liblm.a\nLIB_lm_SRCS=answer.c\nLDADD=-llm liblm.a -llcf liblcf.a -lxpm libxpm.a\n")
        local = self.invoke()
        self.assertEqual(local.returncode, 0, local.stderr)
        self.run_binary()
        self.write(self.project / "answer.c", 'int custom_lm_value(void){return -1;}\n')
        self.write(self.project / "mapped.c", 'int custom_lm_value(void){return 42;}\n')
        mapped = self.invoke("--library", "lm=mapped.c")
        self.assertEqual(mapped.returncode, 0, mapped.stderr)
        self.run_binary()

    def test_unused_course_vt_dependency_preserves_local_sources_and_link_errors(self):
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\nLDADD=-lvt -llcf\n")
        self.write(self.project / "main.c", '#include <stdio.h>\nint main(void){puts("42");return 0;}\n')
        original = (self.project / "main.c").read_bytes()
        built = self.invoke()
        self.assertEqual(built.returncode, 0, built.stderr)
        self.run_binary()
        self.assertEqual((self.project / "main.c").read_bytes(), original)

        # Recognizing a course dependency does not invent its video-text API.
        self.write(self.project / "main.c", '#include <stdio.h>\nvoid vt_info_get(void *);\nint main(void){int value=0;vt_info_get(&value);printf("%d\\n",value);return 0;}\n')
        built = self.invoke()
        self.assertNotEqual(built.returncode, 0)
        self.assertIn("vt_info_get", built.stderr)
        self.assertFalse((self.output / "proj").exists())

        # Both authored library sources and an explicit mapping still take
        # precedence over the system-dependency fallback.
        self.write(self.project / "vt.c", 'void vt_info_get(void *p){*(int *)p=42;}\n')
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\nLIB_vt=libvt.a\nLIB_vt_SRCS=vt.c\nLDADD=-lvt -llcf\n")
        built = self.invoke()
        self.assertEqual(built.returncode, 0, built.stderr)
        self.run_binary()
        self.write(self.project / "vt.c", 'void vt_info_get(void *p){*(int *)p=-1;}\n')
        self.write(self.project / "mapped.c", 'void vt_info_get(void *p){*(int *)p=42;}\n')
        built = self.invoke("--library", "vt=mapped.c")
        self.assertEqual(built.returncode, 0, built.stderr)
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

    def test_basename_functions_and_ordered_modifiers_build_local_library(self):
        self.write(self.project / "main.c", '#include <stdio.h>\nint answer(void); int main(void) { printf("%d\\n",answer()); return 0; }\n')
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\nLDADD=-L../drivers -lanswer\n")
        self.write(self.root / "drivers/nested/answer.c", "int answer(void) { return 42; }\n")
        for expression in ("${notdir ${LIB_SRCS:.c=.o}}", "$(notdir $(LIB_SRCS:.c=.o))",
                           "${LIB_SRCS:T:.c=.o}", "${LIB_SRCS:.c=.o:T}"):
            with self.subTest(expression=expression):
                self.write(self.root / "drivers/Makefile",
                           ".PATH: ${.CURDIR}/nested\nLIB_NAME=libanswer.a\nLIB_SRCS=nested/answer.c\n"
                           "LIB_OBJS=" + expression + "\nlib: ${LIB_OBJS}\n\tar rcs ${LIB_NAME} ${LIB_OBJS}\n")
                before = {p: p.read_bytes() for folder in (self.project, self.root / "drivers") for p in folder.rglob("*.c")}
                result = self.invoke()
                self.assertEqual(result.returncode, 0, result.stderr)
                self.run_binary()
                self.assertEqual(before, {p: p.read_bytes() for folder in (self.project, self.root / "drivers") for p in folder.rglob("*.c")})
        # The declared prefix deliberately does not exist. Basename extraction
        # and both suffix transforms must run, in order, to select answer.c.
        self.write(self.project / "nested/answer.c", "int answer(void) { return 42; }\n")
        self.write(self.project / "Makefile", ".PATH: ${.CURDIR}/nested\nPROG=proj\nINPUT=not-present/answer.c\n"
                   "SRCS=main.c ${INPUT:T:.c=.o:.o=.c}\n")
        result = self.invoke()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.run_binary()

    def test_basename_modifiers_keep_unknown_shell_forms_rejected(self):
        self.write(self.project / "main.c", 'int main(void) { return 0; }\n')
        marker = self.root / "executed"
        for expression in ("${INPUT:T:sh}", "${INPUT:T:H}",
                           "$(notdir $(shell touch " + shlex.quote(str(marker)) + "))"):
            with self.subTest(expression=expression):
                self.write(self.project / "Makefile", "PROG=proj\nINPUT=nested/main.c\nSRCS=" + expression + "\n")
                result = self.invoke()
                self.assertNotEqual(result.returncode, 0)
                self.assertIn("Unsupported", result.stderr)
                self.assertFalse(marker.exists())

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

    def test_minix_variadic_list_alias_uses_compiler_abi_in_external_clients(self):
        sdk = IMPORTER.parents[1]
        original = r"""#include <stdio.h>
#include <stdarg.h>
#include <stdarg.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
static int sentinel;
static void verify_arguments(int tag,...) {
    __va_list original,copy;
    va_start(original,tag);
    va_copy(copy,original);
    assert(va_arg(original,int)==17);
    assert(va_arg(original,double)==3.5);
    assert(va_arg(original,unsigned long long)==0x1122334455667788ULL);
    assert(strcmp(va_arg(original,const char*),"hello")==0);
    assert(va_arg(original,void*)==&sentinel);
    va_end(original);
    char formatted[80];
    assert(vsnprintf(formatted,sizeof formatted,"%d|%.1f|%llu|%s",copy)>0);
    va_end(copy);
    assert(strcmp(formatted,"17|3.5|1234605616436508552|hello")==0);
}
int main(void) { verify_arguments(0,17,3.5,0x1122334455667788ULL,"hello",(void*)&sentinel); return 0; }
"""
        for target in ("native", "web"):
            compiler = shutil.which("cc" if target == "native" else "emcc")
            node = shutil.which("node")
            if not compiler or (target == "web" and not node):
                continue
            for order in ("stdio-first", "stdarg-first"):
                with self.subTest(target=target, order=order):
                    text = original if order == "stdio-first" else original.replace(
                        "#include <stdio.h>\n#include <stdarg.h>",
                        "#include <stdarg.h>\n#include <stdio.h>", 1)
                    source = self.root / (target + "-" + order + ".c")
                    source.write_text(text)
                    output = self.root / (target + "-" + order + (".cjs" if target == "web" else ""))
                    command = [compiler, '-std=c11', '-Werror',
                               '-I', str(sdk/'compat/include'), '-I', str(sdk/'include'), str(source)]
                    if target == "web":
                        command += ['-sENVIRONMENT=node', '-sSINGLE_FILE']
                    compiled = subprocess.run(command + ['-o', str(output)], text=True, capture_output=True)
                    self.assertEqual(compiled.returncode, 0, compiled.stderr)
                    ran = subprocess.run(([node] if target == "web" else []) + [str(output)], text=True, capture_output=True)
                    self.assertEqual(ran.returncode, 0, ran.stdout + ran.stderr)

    def test_minix_internal_int16_alias_in_external_web_client(self):
        compiler = shutil.which("emcc")
        node = shutil.which("node")
        if not compiler or not node:
            self.skipTest("Emscripten and Node are required for the external web header client")
        sdk = IMPORTER.parents[1]
        source = self.root / "original cursor client.c"
        source.write_text("""#include <lcom/lcf.h>
#include <stdint.h>
#include <limits.h>
/* Minix sys/types.h and sys/stdint.h expose machine/int_types.h aliases. */
_Static_assert(sizeof(__uint8_t) * CHAR_BIT == 8, "Minix internal byte width");
_Static_assert((__uint8_t)-1 == 255, "Minix internal unsigned representation");
_Static_assert(_Generic((__uint8_t)0, uint8_t: 1, default: 0), "same public byte type");
static int original_lsb(uint16_t value, uint8_t *out) { *out = (__uint8_t)value; return 0; }
struct original_cursor { __int16_t x, y, new_x, new_y; };
_Static_assert(sizeof(__int16_t) * CHAR_BIT == 16, "Minix signed coordinate width");
_Static_assert((__int16_t)-1 < 0, "Minix signed coordinate representation");
_Static_assert(_Generic((__int16_t)0, int16_t: 1, default: 0), "same public integer type");
int main(void) {
    uint8_t byte = 0;
    if (original_lsb(0x1234, &byte) || byte != 0x34) return 1;
    struct original_cursor cursor = { -32768, 32767, -123, 456 };
    return cursor.x != INT16_MIN || cursor.y != INT16_MAX ||
           cursor.new_x + cursor.new_y != 333;
}
""")
        output = self.root / "original cursor client.js"
        compiled = subprocess.run([compiler, '-std=c11', '-Werror',
                                   '-I', str(sdk/'compat/include'), '-I', str(sdk/'include'),
                                   str(source), '-sEXIT_RUNTIME=1', '-sENVIRONMENT=node',
                                   '-o', str(output)], text=True, capture_output=True)
        self.assertEqual(compiled.returncode, 0, compiled.stderr)
        ran = subprocess.run([node, str(output)], text=True, capture_output=True)
        self.assertEqual(ran.returncode, 0, ran.stdout + ran.stderr)

    def test_historical_minix_types_header_is_usable_by_external_clients(self):
        sdk = IMPORTER.parents[1]
        source = self.root / "historical Minix client.c"
        source.write_text("""#include <minix/types.h>
#include <stdint.h>
_Static_assert(sizeof(u8_t)==1 && sizeof(i8_t)==1,"byte widths");
_Static_assert(sizeof(u16_t)==2 && sizeof(i16_t)==2,"word widths");
_Static_assert(sizeof(u32_t)==4 && sizeof(i32_t)==4,"dword widths");
int main(void){u8_t b=255;i16_t s=-123;u32_t w=0x89abcdefu;return b!=255 || s!=-123 || w!=0x89abcdefu;}
""")
        for target in ("native", "web"):
            compiler = shutil.which("cc" if target == "native" else "emcc")
            node = shutil.which("node")
            if not compiler or (target == "web" and not node):
                continue
            with self.subTest(target=target):
                binary = self.root / ("historical-client" + (".cjs" if target == "web" else ""))
                command = [compiler, '-std=gnu11', '-Werror', '-I', str(sdk/'compat/include'),
                           '-I', str(sdk/'include'), str(source)]
                if target == "web":
                    command += ['-sENVIRONMENT=node', '-sSINGLE_FILE']
                result = subprocess.run(command + ['-o', str(binary)], text=True, capture_output=True)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                result = subprocess.run(([node] if target == "web" else []) + [str(binary)], text=True, capture_output=True)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_canonical_timer_types_work_in_both_header_orders(self):
        # This is the original course header's public type/guard contract.
        local = """#ifndef __TIMER_H
#define __TIMER_H
#include <stdint.h>
#include <stdbool.h>
enum timer_init { INVAL_val, LSB_only, MSB_only, MSB_after_LSB };
enum timer_status_field { tsf_all, tsf_initial, tsf_mode, tsf_base };
union timer_status_field_val { uint8_t byte; enum timer_init in_mode; uint8_t count_mode; bool bcd; };
#endif
"""
        course = '#include <lcom/lcf.h>\n#include <lcom/timer.h>\n'
        sdk = IMPORTER.parent.parent
        for order, headers in [('local-first', local + course),
                               ('lcf-first', course + local),
                               ('timer-first', '#include <lcom/timer.h>\n' + course + local)]:
            with self.subTest(order=order):
                source = self.root / (order + '.c')
                source.write_text(headers + """
_Static_assert(tsf_all == 0 && tsf_initial == 1 && tsf_mode == 2 && tsf_base == 3, "course enum values");
int main(void) { union timer_status_field_val value = {.in_mode = MSB_after_LSB}; return value.in_mode != 3; }
""")
                binary = self.root / order
                compiled = subprocess.run([shutil.which('cc'), '-std=gnu11', '-I', str(sdk/'compat/include'),
                                           '-I', str(sdk/'include'), str(source), '-o', str(binary)],
                                          capture_output=True, text=True)
                self.assertEqual(compiled.returncode, 0, compiled.stderr)
                self.assertEqual(subprocess.run([str(binary)]).returncode, 0)

    def test_sanitizer_reports_memory_error_or_missing_host_runtime(self):
        self.write(self.project / "Makefile", "PROG=proj\nSRCS=main.c\nCFLAGS=-O2\n")
        source = self.project / "main.c"
        sanitizer = "undefined" if sys.platform == "darwin" else "address"
        self.write(source, """
#ifdef __OPTIMIZE__
#error Sanitizer must imply an unoptimized diagnostic build
#endif
#include <limits.h>
int main(void) { volatile int maximum = INT_MAX; return maximum + 1; }
""" if sanitizer == "undefined" else """
#ifdef __OPTIMIZE__
#error Sanitizer must imply an unoptimized diagnostic build
#endif
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
        # Probe the host compiler independently. MSYS GCC currently has no ASan
        # runtime, so its public contract is an explicit failed build, not a skip.
        probe_source = self.root / "healthy.c"
        probe_source.write_text("int main(void) { return 0; }\n")
        capability = subprocess.run([compiler, "-fsanitize=" + sanitizer,
                                     str(probe_source), "-o", str(self.root / "healthy")],
                                    capture_output=True, text=True, env=env, timeout=30)
        result = self.invoke("--cc", compiler, "--sanitize", sanitizer, env=env)
        if capability.returncode:
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("checking compiler sanitizer support", result.stderr)
            self.assertFalse((self.output / "proj").exists())
            self.assertEqual(source.read_bytes(), original)
            report = json.loads((self.output / "build.json").read_text())
            self.assertEqual(report["status"], "failed")
            return
        self.assertEqual(result.returncode, 0, result.stderr)
        crashed = subprocess.run([str(self.output / "proj")], capture_output=True, text=True, timeout=30, env=env)
        self.assertNotEqual(crashed.returncode, 0)
        self.assertIn("runtime error: signed integer overflow" if sanitizer == "undefined"
                      else "AddressSanitizer: heap-buffer-overflow", crashed.stderr)
        self.assertEqual(source.read_bytes(), original)


if __name__ == "__main__":
    unittest.main()
