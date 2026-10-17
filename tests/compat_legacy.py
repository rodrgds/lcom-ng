#!/usr/bin/env python3
"""Compile untouched pre-LCF headers and execute their BIOS call through the CLI."""
import argparse
import hashlib
from pathlib import Path
import subprocess
import sys
import tempfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', type=Path, required=True)
    parser.add_argument('--runtime-build', type=Path, required=True)
    parser.add_argument('--runtime', type=Path, required=True)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='lcom-legacy-') as directory:
        root = Path(directory)
        project = root / 'project'
        project.mkdir()
        (project / 'Makefile').write_text('PROG=proj\nSRCS=main.c\n.include <bsd.prog.mk>\n')
        # The old lmlib declaration differs from LCF in return type, constness
        # and the missing lm_init(bool) argument. Games own these declarations.
        (project / 'lmlib.h').write_text('''typedef struct { phys_bytes phys; void *virtual; unsigned long size; } mmap_t;
void *lm_init(void);
void *lm_alloc(unsigned long size, mmap_t *mapping);
void lm_free(mmap_t *mapping);
''')
        (project / 'main.c').write_text('''#include <minix/drivers.h>
#include <minix/minlib.h>
#include <machine/int86.h>
#include <sys/video.h>
#include "lmlib.h"
_Static_assert(MONO_BASE == 0xB0000L && COLOR_BASE == 0xB8000L, "Minix console memory addresses");
_Static_assert(M_6845 == 0x3B4 && C_6845 == 0x3D4 && CURSOR == 14, "Minix console controller registers");
/* Student-owned VBE wrapper must not override the BIOS implementation itself. */
int vbe_get_mode_info(unsigned short mode, void *output) {
  mmap_t memory;
  struct reg86u registers = {0};
  if (!lm_init() || !lm_alloc(256, &memory)) return 1;
  registers.u.w.ax = 0x4f01;
  registers.u.w.cx = mode;
  registers.u.w.es = PB2BASE(memory.phys);
  registers.u.w.di = PB2OFF(memory.phys);
  registers.u.b.intno = 0x10;
  int result = sys_int86(&registers);
  if (registers.u.w.ax != 0x004f) return 2;
  memcpy(output, memory.virtual, 256);
  lm_free(&memory);
  return result;
}
int main(void) {
  /* Minix minlib owns signed decimal itoa and retains static result storage. */
  if (INT_MIN != (-2147483647 - 1) || INT_MAX != 2147483647) return 20;
  if (strcmp(itoa(0), "0") || strcmp(itoa(7), "7") ||
      strcmp(itoa(-1), "-1") || strcmp(itoa(32767), "32767") ||
      strcmp(itoa(-32768), "-32768") || strcmp(itoa(1000000), "1000000") ||
      strcmp(itoa(INT_MIN), "-2147483648") || strcmp(itoa(INT_MAX), "2147483647")) return 21;
  /* The pointer must remain valid after return and be writable static storage. */
  char *decimal = itoa(123);
  decimal[0] = '9';
  if (strcmp(decimal, "923") || strcmp(itoa(42), "42")) return 22;
  struct __attribute__((packed)) old_mode {
    unsigned char prefix[12]; phys_bytes window_function;
    uint16_t pitch, width, height; unsigned char chars[3], bpp, reserved[230];
  } decoded = {0};
  unsigned char bytes[256] = {0};
  if (vbe_get_mode_info(0x115, bytes)) return 3;
  unsigned width = bytes[18] | (bytes[19] << 8);
  unsigned height = bytes[20] | (bytes[21] << 8);
  if (width != 800 || height != 600 || bytes[25] != 24) return 4;
  memcpy(&decoded, bytes, sizeof(decoded));
  if (decoded.width != 800 || decoded.height != 600 || decoded.bpp != 24) return 9;
  struct reg86u font = {0};
  font.u.b.intno = 0x10; font.u.w.ax = 0x1130; font.u.b.bh = 6;
  if (sys_int86(&font) || font.u.w.cx != 16) return 5;
  unsigned char letter[16];
  phys_bytes address = font.u.w.es * 16u + font.u.w.bp;
  if (sys_readbios(address + 65 * 16, letter, sizeof(letter))) return 6;
  /* A's first rows: ..XX.... and .XXXX..., with two rows per scanline. */
  if (letter[0] != 0x30 || letter[1] != 0x30 || letter[2] != 0x78 || letter[3] != 0x78) return 7;
  if (sys_readbios((phys_bytes)-1, letter, sizeof(letter)) != EINVAL) return 8;
  struct reg86u scan = {0};
  scan.u.b.intno = 0x10; scan.u.w.ax = 0x4f02; scan.u.w.bx = 0x4115;
  if (sys_int86(&scan)) return 9;
  scan.u.w.ax = 0x4f06; scan.u.b.bl = 1;
  if (sys_int86(&scan) || scan.u.w.ax != 0x004f || scan.u.w.bx != 2400 ||
      scan.u.w.cx != 800 || scan.u.w.dx != 6990) return 10;
  puts("legacy VBE 800x600x24 passed");
  return 0;
}
''')
        before = {p: hashlib.sha256(p.read_bytes()).digest() for p in project.iterdir()}
        result = subprocess.run([sys.executable, str(args.sdk / 'compat/build_project.py'), str(project),
                                 '--sdk', str(args.sdk), '--runtime-build', str(args.runtime_build),
                                 '--output', str(root / 'output')], capture_output=True, text=True)
        if result.returncode:
            raise RuntimeError(result.stderr)
        result = subprocess.run([str(args.runtime), 'run', '--headless', '--max-ticks', '120', '--',
                                 str(root / 'output/proj')], capture_output=True, text=True, timeout=15)
        if result.returncode or 'legacy VBE 800x600x24 passed' not in result.stdout:
            raise RuntimeError(result.stdout + result.stderr)
        assert all(hashlib.sha256(p.read_bytes()).digest() == digest for p, digest in before.items())
        print(result.stdout.strip())
        # Darwin's stdlib historically includes sys/wait.h as an extension.
        # Minix programs may own a local variable named wait until they
        # explicitly import the process-wait API.
        namespace = root / 'namespace'
        namespace.mkdir()
        (namespace / 'main.c').write_text('''#include <stdlib.h>
#include <lcom/lcf.h>
#ifdef EXPLICIT_WAIT
#include <sys/wait.h>
#else
static bool wait;
#endif
int main(void) {
#ifdef EXPLICIT_WAIT
  errno = 0;
  if (wait(NULL) != -1 || errno != ECHILD) return 1;
#else
  wait = true;
  if (!wait) return 2;
#endif
  puts("header namespace passed");
  return 0;
}
''')
        for explicit in (False, True):
            (namespace / 'Makefile').write_text('PROG=proj\nSRCS=main.c\n' +
                ('CFLAGS=-DEXPLICIT_WAIT\n' if explicit else ''))
            result = subprocess.run([sys.executable, str(args.sdk / 'compat/build_project.py'), str(namespace),
                                     '--sdk', str(args.sdk), '--runtime-build', str(args.runtime_build),
                                     '--output', str(root / 'namespace-output')], capture_output=True, text=True)
            if result.returncode:
                raise RuntimeError(result.stderr)
            result = subprocess.run([str(root / 'namespace-output/proj')], capture_output=True, text=True)
            if result.returncode or result.stdout.strip() != 'header namespace passed':
                raise RuntimeError(result.stdout + result.stderr)


        time_project = Path(__file__).resolve().parent / 'fixtures/legacy-time'
        time_sources = {p: hashlib.sha256(p.read_bytes()).digest() for p in time_project.iterdir() if p.is_file()}
        result = subprocess.run([sys.executable, str(args.sdk / 'compat/build_project.py'), str(time_project),
                                 '--sdk', str(args.sdk), '--runtime-build', str(args.runtime_build),
                                 '--output', str(root / 'time-output')], capture_output=True, text=True)
        if result.returncode:
            raise RuntimeError(result.stderr)
        result = subprocess.run([str(root / 'time-output/proj')], capture_output=True, text=True)
        if result.returncode or result.stdout.strip() != 'legacy time:64 pointer-write namespace function-address passed':
            raise RuntimeError(result.stdout + result.stderr)
        assert all(hashlib.sha256(p.read_bytes()).digest() == digest for p, digest in time_sources.items())
        print(result.stdout.strip())


        # Minix driver headers preserve old student typedef/local ownership.
        # Modern LCF and an explicit stdbool import still own the C99 macros.
        boolean_fixture = Path(__file__).resolve().parent / 'fixtures/legacy-bool.c'
        for name, legacy, defines in (
                ('legacy', True, []),
                ('stdbool-before', True, ['-DEXPLICIT_BEFORE']),
                ('stdbool-after', True, ['-DEXPLICIT_AFTER']),
                ('modern-lcf', False, ['-DMODERN']),
                ('cpp-legacy', True, []),
                ('cpp-modern', False, ['-DMODERN'])):
            cpp = name.startswith('cpp')
            binary = root / ('boolean-' + name)
            command = ['c++' if cpp else 'cc', '-x', 'c++' if cpp else 'c',
                       '-std=gnu++17' if cpp else '-std=gnu11', '-O2', '-Werror',
                       '-Wno-missing-field-initializers',
                       *(['-DLCOM_COMPAT_LEGACY=1'] if legacy else []), *defines,
                       '-I', str(args.sdk / 'compat/include'), '-I', str(args.sdk / 'include'),
                       str(boolean_fixture), '-o', str(binary)]
            result = subprocess.run(command, capture_output=True, text=True)
            if result.returncode:
                raise RuntimeError(name + ': ' + result.stdout + result.stderr)
            result = subprocess.run([str(binary)], capture_output=True, text=True)
            if result.returncode:
                raise RuntimeError(name + ': ' + result.stdout + result.stderr)
        print('legacy/modern/explicit stdbool/C++ boolean namespace and ABI passed')


if __name__ == '__main__':
    main()
