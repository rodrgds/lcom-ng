#!/usr/bin/env python3
"""Compiled regressions for corrected student packet handling and pixel clipping."""
import os
from pathlib import Path
import shlex
import shutil
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
                result = subprocess.run([*compiler, *flags, *source_flags, '-c', str(project / source),
                                         '-o', str(obj)], capture_output=True, text=True)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                objects.append(str(obj))
            main = folder / 'main.c'
            main.write_text(harness)
            binary = folder / 'regression'
            result = subprocess.run([*compiler, *flags, str(main), *objects, linker, '-o', str(binary)],
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
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
/* PE linkers retain unused source functions. Supply their unrelated device
   dependencies explicitly and fail if the mouse test ever reaches one. */
uint8_t scancode, remote_scancode;
int timer_count;
void kbc_ih(void) { abort(); }
void timer_int_handler(void) { abort(); }
void drawGameState(GameState *game) { abort(); }
int sp_send_byte(uint8_t byte) { abort(); }
int util_sys_inb(int port, uint8_t *value) { abort(); }
int sys_irqsetpolicy(int irq, int policy, int *hook) { abort(); }
int sys_irqrmpolicy(int *hook) { abort(); }
int KBC_write_command(uint8_t port, uint8_t command) { abort(); }
int KBC_read_status(uint8_t *status) { abort(); }
int KBC_read_output(uint8_t *output) { abort(); }
uint32_t micros_to_ticks(uint32_t micros) { abort(); }
int tickdelay(clock_t ticks) { abort(); }
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
/* These unused mode/sprite entry points must never be needed by pixel writes. */
int sys_int86(reg86_t *regs) { abort(); }
int vg_exit(void) { abort(); }
uint8_t *xpm_load(xpm_map_t map, enum xpm_image_type type, xpm_image_t *img) { abort(); }
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

    def test_bounce_normal_enter_opens_playfield_and_launches_a_scoring_ball(self):
        # Both menu and launch advertise normal Enter. Replaying those real key
        # events catches stale-menu repainting and rejecting one-byte Enter.
        cli = Path(os.environ.get('LCOM_CLI', ROOT / 'build/machinelab')).resolve()
        project = COMMUNITY / 'leticia-003--2ndYear_FEUP_LCom/proj/src'
        replay = ROOT / 'scripts/collection-replays/leticia-bounce-enter.mlabscript'
        with tempfile.TemporaryDirectory(prefix='lcom-bounce-regression-') as temporary:
            folder = Path(temporary)
            (folder / 'proj').mkdir()
            subprocess.run([str(cli), 'compat', 'build', str(project), '--output', str(folder / 'native')],
                           check=True, capture_output=True, text=True, timeout=120)
            frames = []
            for tick in (145, 360):
                frame = folder / f'{tick}.ppm'
                result = subprocess.run([str(cli), 'run', '--headless', '--max-ticks', str(tick),
                                         '--timeout', '60', '--script', str(replay),
                                         '--dump-presented', '--dump-frame', str(frame), '--',
                                         str(folder / 'native/proj')], cwd=folder,
                                        env={**os.environ, 'LCOM_COMPAT_PROJECT_ROOT': str(folder)},
                                        capture_output=True, text=True, timeout=90)
                self.assertEqual(result.returncode, 1, result.stdout + result.stderr)
                self.assertIn('max virtual ticks reached', result.stdout + result.stderr)
                magic, dimensions, depth, pixels = frame.read_bytes().split(b'\n', 3)
                self.assertEqual((magic, dimensions, depth), (b'P6', b'1280 1024', b'255'))
                self.assertEqual(len(pixels), 1280 * 1024 * 3)
                frames.append(pixels)
            pixel = lambda frame, x, y: tuple(frame[(y * 1280 + x) * 3:(y * 1280 + x) * 3 + 3])
            # The unoccupied field must show the game's pale background after
            # opening Play, rather than retaining the blue menu artwork.
            self.assertEqual(pixel(frames[0], 1000, 500), (222, 239, 245))
            red_ball = lambda frame: sum(r > 150 and g < 120 and b < 120
                for y in range(550, 850) for x in range(251, 1280)
                for r, g, b in [pixel(frame, x, y)])
            self.assertEqual(red_ball(frames[0]), 0)
            self.assertGreater(red_ball(frames[1]), 50, 'Enter must launch the ball away from its paddle')
            score = lambda frame: tuple(pixel(frame, x, y)
                for y in range(350, 440) for x in range(70, 190))
            self.assertNotEqual(score(frames[0]), score(frames[1]),
                                'The launched ball must hit a brick and change the displayed score')

    def test_portal_first_level_loads_and_survives_return_and_restart(self):
        # The original four-byte filename overflows when adding ".txt". Enter
        # Play through real mouse packets and inspect independently chosen walls.
        cli = Path(os.environ.get('LCOM_CLI', ROOT / 'build/machinelab')).resolve()
        project = COMMUNITY / 'BrunoRosendo--Portal2D/proj/Src'
        replay = ROOT / 'scripts/collection-replays/BrunoRosendo--Portal2D-gameplay.mlabscript'
        with tempfile.TemporaryDirectory(prefix='lcom-portal-regression-') as temporary:
            folder = Path(temporary)
            subprocess.run([str(cli), 'compat', 'build', str(project), '--output', str(folder / 'native')],
                           check=True, capture_output=True, text=True, timeout=120)
            shutil.copytree(project / 'Resources', folder / 'Resources')
            (folder / 'proj').mkdir()
            frame = folder / 'gameplay.ppm'
            # Virtual ticks and frame assertions define this functional test.
            # Allow slower Intel CI enough wall time to reach those same ticks.
            result = subprocess.run([str(cli), 'run', '--headless', '--script', str(replay),
                                     '--max-ticks', '360', '--timeout', '60', '--dump-presented',
                                     '--dump-frame', str(frame), '--', str(folder / 'native/proj'), 'Resources'],
                                    cwd=folder, env={**os.environ, 'LCOM_COMPAT_PROJECT_ROOT': str(folder)},
                                    capture_output=True, text=True, timeout=90)
            self.assertEqual(result.returncode, 1, result.stdout + result.stderr)
            self.assertIn('max virtual ticks reached (360)', result.stdout + result.stderr)
            magic, dimensions, depth, pixels = frame.read_bytes().split(b'\n', 3)
            self.assertEqual((magic, dimensions, depth), (b'P6', b'1280 1024', b'255'))
            self.assertEqual(len(pixels), 1280 * 1024 * 3)
            pixel = lambda x, y: tuple(pixels[(y * 1280 + x) * 3:(y * 1280 + x) * 3 + 3])
            self.assertEqual(pixel(100, 200), (0, 0, 0))
            self.assertGreaterEqual(min(pixel(720, 400)), 200)
            # Returning after animated portals previously freed global Sprite
            # values. Re-entering a level also checks that cleanup leaves no
            # dangling level owners for the next session.
            restart = folder / 'return-restart.mlabscript'
            restart.write_text(replay.read_text() + '\n' + """at 400 key ESC down
at 401 key ESC up
at 450 mouse -100 100 0
at 455 mouse -100 100 0
at 460 mouse -100 100 0
at 465 mouse -100 0 0
at 470 mouse -100 0 0
at 480 mouse 0 0 1
at 485 mouse 0 0 0
""")
            result = subprocess.run([str(cli), 'run', '--headless', '--script', str(restart),
                                     '--max-ticks', '540', '--timeout', '60', '--dump-presented',
                                     '--dump-frame', str(frame), '--', str(folder / 'native/proj'), 'Resources'],
                                    cwd=folder, env={**os.environ, 'LCOM_COMPAT_PROJECT_ROOT': str(folder)},
                                    capture_output=True, text=True, timeout=90)
            self.assertEqual(result.returncode, 1, result.stdout + result.stderr)
            self.assertIn('max virtual ticks reached (540)', result.stdout + result.stderr)
            pixels = frame.read_bytes().split(b'\n', 3)[3]
            self.assertEqual(pixel(100, 200), (0, 0, 0))
            self.assertGreaterEqual(min(pixel(720, 400)), 200)


if __name__ == '__main__':
    unittest.main()
