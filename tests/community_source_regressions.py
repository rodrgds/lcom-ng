#!/usr/bin/env python3
"""Compiled regressions for corrected student packet handling and pixel clipping."""
import os
from pathlib import Path
import shlex
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMMUNITY = ROOT / 'examples/community'


class StudentSourceRegressions(unittest.TestCase):
    def compile_and_run(self, project, sources, harness, source_flags=()):
        compiler = shlex.split(os.environ.get('CC', 'cc'))
        flags = ['-std=gnu11', '-fcommon', '-ffunction-sections', '-fdata-sections',
                 '-Wno-deprecated-non-prototype', '-I', str(ROOT / 'sdk/compat/include'),
                 '-I', str(ROOT / 'sdk/include'), '-I', str(project)]
        linker = '-Wl,-dead_strip' if sys.platform == 'darwin' else '-Wl,--gc-sections'
        with tempfile.TemporaryDirectory(prefix='lcom-student-regression-') as temporary:
            folder = Path(temporary)
            objects = []
            for index, source in enumerate(sources):
                obj = folder / f'{index}.o'
                subprocess.run([*compiler, *flags, *source_flags, '-c', str(project / source),
                                '-o', str(obj)], check=True, capture_output=True, text=True)
                objects.append(str(obj))
            main = folder / 'main.c'
            main.write_text(harness)
            binary = folder / 'regression'
            subprocess.run([*compiler, *flags, str(main), *objects, linker, '-o', str(binary)],
                           check=True, capture_output=True, text=True)
            result = subprocess.run([str(binary)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_minhoca_gameplay_consumes_a_complete_mouse_packet(self):
        # Feed the real mouse controller, then observe the public gameplay handler.
        # Adjacent nonzero packet bytes expose an incorrectly widened extern read.
        self.compile_and_run(COMMUNITY / 'Forreca05--LCOM-Snake-Game-Projeto-2025/proj/src',
                             ['input/input.c', 'controllers/KBC/mouse/mouse.c'], r'''
#include "input/input.h"
#include "controllers/KBC/mouse/mouse.h"
#include <assert.h>
extern int mouse_x, mouse_y;
static uint8_t bytes[] = {0x08, 5, 7, 0x38, 0xfd, 0xfe};
static int next_byte;
int KBC_get_byte(uint8_t *byte) { *byte = bytes[next_byte++]; return 0; }
int main(void) {
  Sprite cursor = {0}; GameState game = {0}; game.cursorSprite = &cursor;
  int x = mouse_x, y = mouse_y;
  for (int i = 0; i < 3; i++) handleMouseInputSinglePlayerClassic(&game);
  assert(cursor.x == x + 5 && cursor.y == y - 7);
  for (int i = 0; i < 3; i++) handleMouseInputSinglePlayerClassic(&game);
  assert(cursor.x == x + 2 && cursor.y == y - 5);
  return 0;
}
''')

    def test_pettycats_clips_both_screen_edges_without_corrupting_pixels_or_memory(self):
        # A guarded allocator and a tiny physical framebuffer expose right-edge row
        # aliasing and bottom-edge writes through the real GPU public functions.
        self.compile_and_run(COMMUNITY / 'teixdavide--FEUP_LCOM_PettyCats/src',
                             ['drivers/gpu.c'], r'''
#include <lcom/lcf.h>
#include "drivers/gpu.h"
#include <assert.h>
static uint8_t front[12], allocation[64];
void *game_allocate(size_t size) {
  assert(size == 12); memset(allocation, 0xcd, sizeof allocation); return allocation;
}
int vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *info) {
  *info = (vbe_mode_info_t){.XResolution = 4, .YResolution = 3, .BitsPerPixel = 8};
  return 0;
}
int sys_privctl(endpoint_t endpoint, int request, void *arg) { return 0; }
void *vm_map_phys(endpoint_t endpoint, void *phys, size_t length) { return front; }
int main(void) {
  assert(create_vram_buffer(0x105) == 0);
  assert(set_background_color(0x105, 0) == 0);
  assert(draw_pixel(0x105, 3, 2, 9) == 0);
  assert(draw_pixel(0x105, 4, 0, 7) == 0);
  assert(draw_pixel(0x105, 0, 3, 7) == 0);
  assert(draw_pixel(0x105, UINT16_MAX, UINT16_MAX, 7) == 0);
  assert(update_front_buffer() == 0);
  for (int i = 0; i < 12; i++) assert(front[i] == (i == 11 ? 9 : 0));
  for (int i = 12; i < 64; i++) assert(allocation[i] == 0xcd);
  return 0;
}
''', source_flags=['-Dmalloc=game_allocate'])


if __name__ == '__main__':
    unittest.main()
