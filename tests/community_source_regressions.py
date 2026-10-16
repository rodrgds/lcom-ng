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

    def test_space_shooter_cursor_stays_in_framebuffer(self):
        self.compile_and_run(COMMUNITY / 'SamBlack569--feup-lcom-2425/proj/src',
                             ['drivers/mouse.c','core/game.c'], r'''#include "drivers/mouse.h"
#include "core/game.h"
#include <assert.h>
unsigned hres=1152, vres=864;
extern int x_init,y_init;
extern struct packet pp;
void get_menu(void){abort();}
void draw_mouse(void){abort();}
void draw_help_menu(void){abort();}
void load_buffer(void){abort();}
void clear_all_buffers(void){abort();}
void load_game_background(void){abort();}
void get_game_background(void){abort();}
void draw_asteroid(int x,int y){abort();}
void draw_spaceship(int x){abort();}
void draw_score(int n,int x,int y){abort();}
void draw_time(int n){abort();}
int vg_exit(void){abort();}
extern game_state_t game_state;
int main(void) {
  pp.delta_x=-96; pp.delta_y=96;
  assert(track_mouse_movement()==0);
  assert(x_init==0 && y_init==0);
  pp.delta_x=96; pp.delta_y=-96;
  for(int i=0;i<20;i++) assert(track_mouse_movement()==0);
  assert(x_init==1151 && y_init==863);
  pp.delta_x=-10; pp.delta_y=10;
  assert(track_mouse_movement()==0);
  assert(x_init==1141 && y_init==853);
  pp.x_ov=true; pp.delta_x=50;
  assert(track_mouse_movement()!=0 && x_init==1141 && y_init==853);
  pp.x_ov=false; x_init=400;y_init=300;
  pp.delta_x=5;pp.delta_y=6;game_state=GAME_STATE;
  mouse_handler();assert(x_init==405 && y_init==294);
  game_state=HELP_STATE;mouse_handler();assert(x_init==410 && y_init==288);
  return 0;
}
''')


    def test_pixel_football_cursor_edges_preserve_direction(self):
        self.compile_and_run(COMMUNITY / 'gsilva00--LCOM-Project/proj/src/objects',
                             ['cursor.c'], r'''#include "cursor.h"
#include <assert.h>
uint8_t *xpm_load(xpm_map_t p,enum xpm_image_type t,xpm_image_t *i) {
  static uint8_t pixel; (void)p;(void)t;i->width=1;i->height=1;return &pixel;
}
int main(void) {
  Cursor *c=create_cursor(NULL);
  assert(c && c->x==400 && c->y==300);
  c->x=5;c->y=5;
  set_coordinates(c,10,10,true,true);
  assert(c->x==0 && c->y==0);
  set_coordinates(c,10,10,false,false);
  assert(c->x==10 && c->y==10);
  c->x=795;c->y=595;
  set_coordinates(c,10,10,false,false);
  assert(c->x==799 && c->y==599);
  set_coordinates(c,10,10,true,true);
  assert(c->x==789 && c->y==589);
  destroy_cursor(c);
  return 0;
}
''')


    def test_space_invaders_cursor_can_leave_top_margin_one_pixel_at_a_time(self):
        self.compile_and_run(COMMUNITY / 'Adriano-7--feup-lcom/proj/src/model',
                             ['cursor.c'], r'''#include "cursor.h"
#include <assert.h>
vbe_mode_info_t mode_info={.XResolution=800,.YResolution=600};
struct packet pkt;
int video_draw_rectangle(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint32_t c) {return 0;}
int main(void) {
  DrawableObject_t object={.x=100,.y=40,.img={.width=16,.height=16}};
  Cursor_t cursor={.drawableObject=&object};
  pkt.delta_y=-1;
  for(int i=0;i<20;i++) {updateCursor(&cursor);assert(object.y==41+i);}
  pkt.delta_y=96; updateCursor(&cursor);assert(object.y==40);
  pkt.delta_y=-96; updateCursor(&cursor);assert(object.y==136);
  return 0;
}
''')


    def test_star_defender_cursor_clamps_without_edge_recoil(self):
        self.compile_and_run(COMMUNITY / 'luanalima27--LCOM-FEUP/proj/src',
                             ['mouse.c'], r'''#include "mouse.h"
#include <assert.h>
vbe_mode_info_t vmi_p={.XResolution=800,.YResolution=600};
extern int x_mouse,y_mouse;
extern struct packet packet0;
int main(void) {
  packet0.delta_x=-1;packet0.delta_y=1;
  move_cursor();assert(x_mouse==0 && y_mouse==0);
  x_mouse=784;y_mouse=584;
  packet0.delta_x=1;packet0.delta_y=-1;
  move_cursor();assert(x_mouse==784 && y_mouse==584);
  packet0.delta_x=-1;packet0.delta_y=1;
  move_cursor();assert(x_mouse==783 && y_mouse==583);
  assert(packet0.delta_x==0 && packet0.delta_y==0);
  return 0;
}
''')


    def test_lcomaze_consumes_complete_mouse_packets_once(self):
        # The real IRQ loop must advance its original half-speed cursor once,
        # including when the next packet is neutral.
        cli = Path(os.environ.get('LCOM_CLI', ROOT / 'build/machinelab')).resolve()
        project = COMMUNITY / 'HenriqueSFernandes--LCOMaze/src'
        with tempfile.TemporaryDirectory(prefix='lcom-maze-cursor-') as temporary:
            folder = Path(temporary)
            subprocess.run([str(cli), 'compat', 'build', str(project), '--output', str(folder / 'native')],
                           check=True, capture_output=True, text=True, timeout=120)
            replay = folder / 'cursor.mlabscript'
            replay.write_text('at 180 mouse 20 -10 0\nat 200 mouse 0 0 0\n')
            frame = folder / 'menu.ppm'
            result = subprocess.run([str(cli), 'run', '--headless', '--script', str(replay),
                                     '--max-ticks', '240', '--timeout', '30', '--dump-presented',
                                     '--dump-frame', str(frame), '--', str(folder / 'native/proj'), 'host'],
                                    cwd=folder, capture_output=True, text=True, timeout=45)
            self.assertEqual(result.returncode, 1, result.stdout + result.stderr)
            self.assertIn('max virtual ticks reached (240)', result.stdout + result.stderr)
            magic, dimensions, depth, pixels = frame.read_bytes().split(b'\n', 3)
            self.assertEqual((magic, dimensions, depth), (b'P6', b'1152 864', b'255'))
            # Only the cursor has white pixels in the top-left 80x60 region.
            white = [(x,y) for y in range(60) for x in range(80)
                     if pixels[(y*1152+x)*3:(y*1152+x)*3+3] == b'\xff\xff\xff']
            self.assertGreater(len(white), 100, 'The original white cursor must be rendered')
            self.assertEqual((min(x for x,y in white), min(y for x,y in white)), (10,5),
                             'A complete movement packet advances the half-speed cursor once')


    def test_memory_match_background_erases_cursor_on_uncovered_edge(self):
        # Original background art is one row shorter than the framebuffer.
        # Redrawing must erase a cursor that occupied that last row.
        self.compile_and_run(COMMUNITY / 'iamaccosta--FEUP-LCOM/proj/src', ['game.c'], r'''
#include <lcom/lcf.h>
#include "game.h"
#include "video.h"
#include <assert.h>
extern xpm_image_t imgBackground;
extern uint8_t *spriteBackground;
vbe_mode_info_t modeinfo;
xpm_image_t imgDeck, imgCardBack;
uint8_t *spriteDeck, *spriteCardBack;
static uint32_t framebuffer[6];
uint8_t *xpm_load(xpm_map_t map, enum xpm_image_type type, xpm_image_t *img) { abort(); }
void greenPartOfScreen(int x, int y, int width, int height) { abort(); }
void vg_draw_card(int x, int y, int column, int row) { abort(); }
int my_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
  assert(x < 2 && y < 3); framebuffer[y*2+x] = color & 0xffffff; return 0;
}
int main(void) {
  uint8_t pixels[] = {1,2,3,4,5,6,7,8,9,10,11,12};
  spriteBackground=pixels; imgBackground=(xpm_image_t){.width=2,.height=2};
  modeinfo=(vbe_mode_info_t){.XResolution=2,.YResolution=3,.BitsPerPixel=24};
  framebuffer[4]=framebuffer[5]=0x00a2e8;
  drawBackground();
  assert(framebuffer[0] == 0x030201 && framebuffer[1] == 0x060504);
  assert(framebuffer[2] == 0x090807 && framebuffer[3] == 0x0c0b0a);
  if (framebuffer[4] != 0x090807 || framebuffer[5] != 0x0c0b0a) {
    fprintf(stderr,"Background redraw must erase the last-row cursor using the art edge\n"); return 1;
  }
  return 0;
}
''')


    def aerial_ambush_cursor(self, raw_bytes):
        # Movement must use one complete X/Y pair rather than mixed bytes.
        cli = Path(os.environ.get('LCOM_CLI', ROOT / 'build/machinelab')).resolve()
        project = COMMUNITY / 'alexandre2004costa--Lcom-project'
        with tempfile.TemporaryDirectory(prefix='lcom-aerial-cursor-') as temporary:
            folder = Path(temporary)
            subprocess.run([str(cli), 'compat', 'build', str(project), '--output', str(folder / 'native')],
                           check=True, capture_output=True, text=True, timeout=120)
            replay = folder / 'cursor.mlabscript'
            replay.write_text('at 180 ps2 mouse 0x28 ok\nat 181 ps2 mouse 20 ok\nat 182 ps2 mouse 246 ok\nat 200 ps2 mouse 8 ok\nat 201 ps2 mouse 0 ok\nat 202 ps2 mouse 0 ok\n' if raw_bytes else 'at 180 mouse 20 -10 0\nat 200 mouse 0 0 0\n')
            frame = folder / 'menu.ppm'
            result = subprocess.run([str(cli), 'run', '--headless', '--script', str(replay),
                                     '--max-ticks', '240', '--timeout', '30', '--dump-presented',
                                     '--dump-frame', str(frame), '--', str(folder / 'native/proj')],
                                    cwd=folder, capture_output=True, text=True, timeout=45)
            self.assertEqual(result.returncode, 1, result.stdout + result.stderr)
            self.assertIn('max virtual ticks reached (240)', result.stdout + result.stderr)
            magic, dimensions, depth, pixels = frame.read_bytes().split(b'\n', 3)
            self.assertEqual((magic, dimensions, depth), (b'P6', b'1152 864', b'255'))
            # The cursor outline is RGB17,17,17. Sky in this region is colored.
            outline = [(x,y) for y in range(100) for x in range(150)
                       if pixels[(y*1152+x)*3:(y*1152+x)*3+3] == b'\x11\x11\x11']
            self.assertGreater(len(outline), 100, 'The original cursor outline must render')
            self.assertEqual((min(x for x,y in outline), min(y for x,y in outline)), (20,10),
                             'Only one complete X/Y packet moves the cursor')


    def test_aerial_ambush_consumes_complete_mouse_packets_once(self):
        self.aerial_ambush_cursor(raw_bytes=True)


    def test_aerial_ambush_enables_mouse_reporting(self):
        self.aerial_ambush_cursor(raw_bytes=False)


    def test_memory_match_consumes_complete_mouse_packets_once(self):
        # A neutral packet after movement must not replay its preceding delta.
        # Exercise the real IRQ loop and independently locate the blue cursor.
        cli = Path(os.environ.get('LCOM_CLI', ROOT / 'build/machinelab')).resolve()
        project = COMMUNITY / 'iamaccosta--FEUP-LCOM/proj/src'
        with tempfile.TemporaryDirectory(prefix='lcom-memory-cursor-') as temporary:
            folder = Path(temporary)
            subprocess.run([str(cli), 'compat', 'build', str(project), '--output', str(folder / 'native')],
                           check=True, capture_output=True, text=True, timeout=120)
            replay = folder / 'cursor.mlabscript'
            replay.write_text('at 180 mouse 20 -10 0\nat 200 mouse 0 0 0\n')
            frame = folder / 'menu.ppm'
            result = subprocess.run([str(cli), 'run', '--headless', '--script', str(replay),
                                     '--max-ticks', '240', '--timeout', '30', '--dump-presented',
                                     '--dump-frame', str(frame), '--', str(folder / 'native/proj')],
                                    cwd=folder, capture_output=True, text=True, timeout=45)
            self.assertEqual(result.returncode, 1, result.stdout + result.stderr)
            self.assertIn('max virtual ticks reached (240)', result.stdout + result.stderr)
            magic, dimensions, depth, pixels = frame.read_bytes().split(b'\n', 3)
            self.assertEqual((magic, dimensions, depth), (b'P6', b'800 600', b'255'))
            blue = [(i // 3 % 800, i // 3 // 800) for i in range(0, len(pixels), 3)
                    if pixels[i:i+3] == bytes((0, 162, 232))]
            self.assertGreater(len(blue), 100, 'The original blue cursor must be rendered')
            self.assertEqual((min(x for x, y in blue), min(y for x, y in blue)), (10, 5),
                             'Only the complete movement packet advances the original half-speed cursor')

    def test_memory_match_clamps_scaled_motion_and_clips_cursor_edges(self):
        # Keep the original per-packet half sensitivity, including odd truncation.
        # The renderer must never send offscreen cursor pixels to its framebuffer.
        self.compile_and_run(COMMUNITY / 'iamaccosta--FEUP-LCOM/proj/src', ['game.c'], r'''
#include <lcom/lcf.h>
#include "game.h"
#include "video.h"
#include <assert.h>
extern Mouse mouse;
extern xpm_image_t imgMouse;
extern uint8_t *spriteMouse;
vbe_mode_info_t modeinfo;
xpm_image_t imgDeck, imgCardBack;
uint8_t *spriteDeck, *spriteCardBack;
static int invalid_pixels, valid_pixels;
static uint32_t colors[2];
uint8_t *xpm_load(xpm_map_t map, enum xpm_image_type type, xpm_image_t *img) { abort(); }
void greenPartOfScreen(int x, int y, int width, int height) { abort(); }
void vg_draw_card(int x, int y, int column, int row) { abort(); }
int my_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= modeinfo.XResolution || y >= modeinfo.YResolution) invalid_pixels++;
  else { assert(x == 3 && y >= 1 && y <= 2); colors[y-1] = color & 0xffffff; valid_pixels++; }
  return 0;
}
int main(void) {
  modeinfo = (vbe_mode_info_t){.XResolution=800, .YResolution=600, .BitsPerPixel=24};
  create_mouse(); assert(mouse.x == 0 && mouse.y == 0);
  update_mouse_position(20, -10); assert(mouse.x == 10 && mouse.y == 5);
  update_mouse_position(1, -1); assert(mouse.x == 10 && mouse.y == 5);
  update_mouse_position(-2, 2); assert(mouse.x == 9 && mouse.y == 4);
  mouse.x=798; mouse.y=598; update_mouse_position(2, -2);
  int failures=0;
  if (mouse.x != 799 || mouse.y != 599) { fprintf(stderr,"Scaled delta must reach the last framebuffer pixel\n"); failures++; }
  mouse.x=799; mouse.y=599; update_mouse_position(20, -20);
  if (mouse.x != 799 || mouse.y != 599) { fprintf(stderr,"Scaled positive overflow must clamp at framebuffer bounds\n"); failures++; }
  mouse.x=1; mouse.y=1; update_mouse_position(-20, 20);
  if (mouse.x != 0 || mouse.y != 0) { fprintf(stderr,"Negative movement must clamp to zero\n"); failures++; }
  uint8_t sprite[] = {1,2,3,4,5,6,7,8,9,10,11,12};
  spriteMouse=sprite; imgMouse=(xpm_image_t){.width=2,.height=2};
  modeinfo=(vbe_mode_info_t){.XResolution=4,.YResolution=3,.BitsPerPixel=24};
  mouse.x=3; mouse.y=1; draw_mouse();
  if (invalid_pixels || valid_pixels != 2 || colors[0] != 0x030201 || colors[1] != 0x090807) {
    fprintf(stderr,"Cursor must clip the right edge without losing source row stride\n"); failures++;
  }
  invalid_pixels=valid_pixels=0; mouse.y=2; draw_mouse();
  if (invalid_pixels || valid_pixels != 1) { fprintf(stderr,"Cursor must clip the bottom edge\n"); failures++; }
  return failures ? 1 : 0;
}
''')

    def test_grand_prix_releases_driving_controls_while_paused(self):
        # Public keyboard input must consume driving releases even during pause.
        # Both ESC and the real mouse Resume button must return neutral controls.
        self.compile_and_run(COMMUNITY / 'm1ke26--MINIX-Grand-Prix/project/include',
                             ['../src/state.c', '../src/race.c', '../src/pause_menu.c', '../src/button.c'], r'''
#include "state.h"
#include "finish_menu.h"
#include "start_menu.h"
#include "car.h"
#include "track.h"
#include "camera.h"
#include "vehicles.h"
#include "kbc.h"
#include "pause_menu.h"
#include "hud.h"
#include "cursor.h"
#include "rtc.h"
#include "tracks_pixmaps.h"
#include "leaderboard.h"
#include "leaderboard_menu.h"
#include "minimap.h"
#include "speedometer.h"
#include <assert.h>
/* Unrelated drawing, resource and menu-transition paths are not exercised.
   Typed abort stubs also let PE linkers retain the whole original source unit. */
bool running;
xpm_row_t * const track_xpms[TRACK_XPM_COUNT] = {0};
xpm_row_t * const collision_xpms[TRACK_XPM_COUNT] = {0};
const vehicle_def_t *vehicle_get(int index) { abort(); }
int car_sprite_index(car_t *car) { abort(); }
camera_t *create_camera(int map_width, int map_height) { abort(); }
car_t* create_car(double x, double y,double angle, xpm_map_t xpms[], int num_sprites) { abort(); }
cursor_t* create_cursor(sprite_t *sprite, int x, int y, int scale) { abort(); }
track_t *create_track(xpm_map_t track, xpm_map_t collision) { abort(); }
void destroy_camera(camera_t *camera) { abort(); }
void destroy_car(car_t *car) { abort(); }
void destroy_cursor(cursor_t *cursor) { abort(); }
void destroy_track(track_t *track) { abort(); }
void draw_car(car_t *car, int cam_x, int cam_y, int sprite_idx) { abort(); }
void draw_cursor(cursor_t *cursor) { abort(); }
void draw_string(font_t *font, const char *str, int x, int y, uint32_t color) { abort(); }
void draw_string_scaled(font_t *font, const char *str, int x, int y, int scale, uint32_t color) { abort(); }
void draw_track(track_t *track, int cam_x, int cam_y) { abort(); }
finish_menu_t *finish_menu_create(font_t *font, const race_t *race) { abort(); }
void finish_menu_destroy(finish_menu_t *menu) { abort(); }
void finish_menu_draw(const finish_menu_t *menu) { abort(); }
bool finish_menu_handle_key(const finish_menu_t *menu, uint8_t scancode) { abort(); }
void follow_camera(camera_t *camera, int car_x, int car_y) { abort(); }
void hud_draw_boost_indicator(car_t *car) { abort(); }
void hud_draw_controls_panel(font_t *font) { abort(); }
void hud_draw_lap_counter(font_t *font, int current_lap, int total_laps) { abort(); }
void hud_draw_race_countdown(font_t *font, int seconds_left) { abort(); }
void hud_draw_race_start_prompt(font_t *font) { abort(); }
void hud_draw_timer(font_t *font, unsigned time_elapsed) { abort(); }
char kbd_scancode_to_char(uint8_t scancode) { abort(); }
leaderboard_menu_t *leaderboard_menu_create(font_t *font, const char *username) { abort(); }
void leaderboard_menu_destroy(leaderboard_menu_t *menu) { abort(); }
void leaderboard_menu_draw(const leaderboard_menu_t *menu) { abort(); }
bool leaderboard_menu_handle_key(const leaderboard_menu_t *menu, uint8_t scancode) { abort(); }
void leaderboard_save_time(const char *username, unsigned time, int track_index, rtc_date date) { abort(); }
void minimap_destroy() { abort(); }
void minimap_draw(int car_x, int car_y) { abort(); }
void minimap_init(int track_index, track_t *track) { abort(); }
int rtc_read_date(rtc_date *date) { abort(); }
void speedometer_draw(float speed) { abort(); }
void speedometer_init() { abort(); }
void start_menu_change_car(start_menu_t *sm, int delta) { abort(); }
void start_menu_change_track(start_menu_t *sm, int delta) { abort(); }
start_menu_t* start_menu_create(font_t *font, const char *username) { abort(); }
void start_menu_destroy(start_menu_t *sm) { abort(); }
void start_menu_draw(start_menu_t *sm, int cursor_x, int cursor_y) { abort(); }
start_menu_action_t start_menu_handle_click(start_menu_t *sm, int x, int y) { abort(); }
void start_menu_handle_key(start_menu_t *sm, char key) { abort(); }
bool terrain_is_race_checkpoint(terrain_type_t t) { abort(); }
terrain_type_t track_car_checkpoint(track_t *track, int car_x, int car_y, int car_w, int car_h) { abort(); }
bool track_find_start_spawn(track_t *track, int car_w, int car_h, int *spawn_x, int *spawn_y) { abort(); }
void update_car_physics(car_t *car, const game_input_t *input, track_t *track) { abort(); }
void update_cursor_sprite(cursor_t *cursor, sprite_t *new_sprite) { abort(); }
void vg_buf_clear(void) { abort(); }
void vg_buf_desaturate(void) { abort(); }
void vg_buf_draw_rect(int x, int y, int w, int h, uint32_t color) { abort(); }

int main(void) {
  const uint8_t down[] = {0x11, 0x1f, 0x1e, 0x20, 0x39, 0x2a};
  const char *keys[] = {"W", "S", "A", "D", "Space", "Shift"};
  int failures = 0;
  for (int mouse_resume = 0; mouse_resume < 2; mouse_resume++) {
    state_t state = {.tag = STATE_IN_GAME};
    bool *held[] = {&state.data.in_game.input.accelerate, &state.data.in_game.input.brake,
                   &state.data.in_game.input.turn_left, &state.data.in_game.input.turn_right,
                   &state.data.in_game.input.handbrake, &state.data.in_game.input.boost};
    race_init(&state.data.in_game.race);
    handle_kbd_event(&state, 0x1c);
    for (int tick = 0; tick < 180; tick++) race_update(&state.data.in_game.race);
    assert(race_has_started(&state.data.in_game.race));
    state.data.in_game.pause_menu = pause_menu_create(NULL);
    assert(state.data.in_game.pause_menu);
    for (int i = 0; i < 6; i++) {
      handle_kbd_event(&state, down[i]); assert(*held[i]);
    }
    handle_kbd_event(&state, 0x01); handle_kbd_event(&state, 0x81);
    assert(state.data.in_game.pause);
    for (int i = 0; i < 6; i++) handle_kbd_event(&state, down[i] | 0x80);
    if (mouse_resume) {
      struct packet click = {.lb = true};
      handle_mouse_event(&state, &click, 400, 280);
    } else {
      handle_kbd_event(&state, 0x01); handle_kbd_event(&state, 0x81);
    }
    assert(!state.data.in_game.pause);
    for (int i = 0; i < 6; i++) {
      if (*held[i]) {
        fprintf(stderr, "%s remains latched after release while paused and %s resume\n",
                keys[i], mouse_resume ? "mouse" : "ESC");
        failures++;
      }
      /* Restore a neutral state through real events for the next scenario. */
      handle_kbd_event(&state, down[i] | 0x80); assert(!*held[i]);
    }
    handle_kbd_event(&state, 0x01); handle_kbd_event(&state, 0x81);
    for (int i = 0; i < 6; i++) { handle_kbd_event(&state, down[i]); assert(!*held[i]); }
    handle_kbd_event(&state, 0x01); handle_kbd_event(&state, 0x81);
    for (int i = 0; i < 6; i++) {
      handle_kbd_event(&state, down[i]); assert(*held[i]);
      /* A key physically held across pause still belongs to the driver. */
      handle_kbd_event(&state, 0x01); handle_kbd_event(&state, 0x81);
      handle_kbd_event(&state, 0x01); handle_kbd_event(&state, 0x81);
      assert(*held[i]);
      handle_kbd_event(&state, down[i] | 0x80); assert(!*held[i]);
    }
    pause_menu_destroy(state.data.in_game.pause_menu);
  }
  return failures ? 1 : 0;
}
''')

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
