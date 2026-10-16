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
    def compile_and_run(self, project, sources, harness, source_flags=(), include_dirs=()):
        compiler = shlex.split(os.environ.get('CC', 'cc'))
        flags = ['-std=gnu11', '-fcommon', '-ffunction-sections', '-fdata-sections',
                 '-Wno-deprecated-non-prototype', '-I', str(ROOT / 'sdk/compat/include'),
                 '-I', str(ROOT / 'sdk/include'), '-I', str(project)]
        for include_dir in include_dirs:
            flags.extend(['-I',str(include_dir)])
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
        import re
        source = COMMUNITY / 'SamBlack569--feup-lcom-2425/proj/src'
        asset = (source / 'graphics/xpms/cursor.h').read_text()
        width,height = map(int,re.search(r'"(\d+) (\d+) \d+ \d+ ',asset).groups())
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
  assert(x_init==$MAX_X$ && y_init==$MAX_Y$);
  pp.delta_x=-10; pp.delta_y=10;
  assert(track_mouse_movement()==0);
  assert(x_init==$INNER_X$ && y_init==$INNER_Y$);
  pp.x_ov=true; pp.delta_x=50;
  assert(track_mouse_movement()!=0 && x_init==$INNER_X$ && y_init==$INNER_Y$);
  pp.x_ov=false; x_init=400;y_init=300;
  pp.delta_x=5;pp.delta_y=6;game_state=GAME_STATE;
  mouse_handler();assert(x_init==405 && y_init==294);
  game_state=HELP_STATE;mouse_handler();assert(x_init==410 && y_init==288);
  return 0;
}
'''.replace('$MAX_X$',str(1152-width)).replace('$MAX_Y$',str(864-height))
           .replace('$INNER_X$',str(1142-width)).replace('$INNER_Y$',str(854-height)))


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


    def test_minix_legends_cursor_sprite_remains_inside_framebuffer(self):
        # Cursor dimensions come from the rendered asset, independently of input code.
        import re
        project = COMMUNITY / 'Cast-10--LCOMProject/proj'
        header = (project / 'assets/cursor/cursor_xpms.h').read_text()
        match = re.search(r'"(\d+) (\d+) \d+ \d+"', header)
        width,height = map(int, match.groups())
        self.compile_and_run(project / 'input', ['MouseInterpreter.c'], r'''#include "MouseInterpreter.h"
#include <assert.h>
int main(void) {
  MouseState state;mouseStateInit(&state,512,384);
  struct packet packet={.delta_x=96,.delta_y=-96};
  for(int i=0;i<20;i++)mouseInterpretPacket(&state,&packet,1024,768);
  assert(state.x==$MAX_X$ && state.y==$MAX_Y$);
  packet.delta_x=-96;packet.delta_y=96;
  for(int i=0;i<20;i++)mouseInterpretPacket(&state,&packet,1024,768);
  assert(state.x==0 && state.y==0);
  packet.delta_x=3;packet.delta_y=-5;packet.lb=true;
  mouseInterpretPacket(&state,&packet,1024,768);
  assert(state.x==3 && state.y==5 && state.p1.attack && state.p1.attack_pressed);
  return 0;
}
'''.replace('$MAX_X$',str(1024-width)).replace('$MAX_Y$',str(768-height)))


    def test_pettycats_cursor_clamps_its_whole_sprite_inside_the_frame(self):
        # Original mouse art is16x16, so a1024x768 screen ends at1008,752.
        self.compile_and_run(COMMUNITY / 'teixdavide--FEUP_LCOM_PettyCats/src',
                             ['controller/controllerMouse.c'], r'''
#include <lcom/lcf.h>
#include "controller/controllerMouse.h"
#include <assert.h>
struct packet mouse_packet;
int main(void) {
  int x=1007,y=751;
  mouse_packet=(struct packet){.delta_x=2,.delta_y=-2}; moveMouse(&x,&y);
  if(x!=1008 || y!=752) {fprintf(stderr,"Whole16px cursor must remain within the framebuffer\n");return 1;}
  mouse_packet=(struct packet){.delta_x=200,.delta_y=-200}; moveMouse(&x,&y);
  assert(x==1008 && y==752);
  mouse_packet=(struct packet){.delta_x=-3,.delta_y=4}; moveMouse(&x,&y);
  assert(x==1005 && y==748);
  x=y=1; mouse_packet=(struct packet){.delta_x=-20,.delta_y=20};moveMouse(&x,&y);
  assert(x==0 && y==0);
  mouse_packet=(struct packet){.delta_x=10,.delta_y=-20,.lb=true};moveMouse(&x,&y);
  assert(x==10 && y==20);
  return 0;
}
''')


    def snake_orchard_cursor(self, raw_bytes):
        # Actual menu IRQ processing must keep the whole original cursor visible.
        cli = Path(os.environ.get('LCOM_CLI', ROOT / 'build/machinelab')).resolve()
        project = COMMUNITY / 'Joanaigs--Lcom-Snake-Game/proj/src'
        with tempfile.TemporaryDirectory(prefix='lcom-orchard-cursor-') as temporary:
            folder = Path(temporary)
            subprocess.run([str(cli), 'compat', 'build', str(project), '--output', str(folder / 'native')],
                           check=True, capture_output=True, text=True, timeout=120)
            for name, moves, expected in [('lower-right', [(200,-200)]*4, (760,560)),
                                           ('upper-left', [(-200,200)]*4, (0,0))]:
                with self.subTest(edge=name):
                    replay = folder / 'cursor.mlabscript'
                    lines=[]
                    for i,(x,y) in enumerate(moves):
                        header=8 | (16 if x<0 else 0) | (32 if y<0 else 0)
                        for j,byte in enumerate((header,x & 255,y & 255)):
                            lines.append(f'at {180+i*10+j} ps2 mouse {byte} ok\n')
                    replay.write_text(''.join(lines) if raw_bytes else ''.join(f'at {180+i*10} mouse {x} {y} 0\n' for i,(x,y) in enumerate(moves)))
                    frame = folder / 'menu.ppm'
                    result = subprocess.run([str(cli), 'run', '--headless', '--script', str(replay),
                                             '--max-ticks', '260', '--timeout', '30', '--dump-presented',
                                             '--dump-frame', str(frame), '--', str(folder / 'native/proj')],
                                            cwd=folder, capture_output=True, text=True, timeout=45)
                    self.assertEqual(result.returncode, 1, result.stdout + result.stderr)
                    self.assertIn('max virtual ticks reached (260)', result.stdout + result.stderr)
                    magic, dimensions, depth, pixels = frame.read_bytes().split(b'\n', 3)
                    self.assertEqual((magic, dimensions, depth), (b'P6', b'800 600', b'255'))
                    # Independent authored points in the original40x40 cursor:
                    # top black outline, near-black antialias and white interior.
                    checks = [(7,0,(0,0,0)),(8,2,(2,2,2)),(9,4,(255,255,255)),
                              (12,10,(255,255,255)),(16,16,(255,255,255))]
                    for x,y,color in checks:
                        offset=((expected[1]+y)*800+expected[0]+x)*3
                        self.assertEqual(tuple(pixels[offset:offset+3]),color,
                                         f'{name}: original cursor must remain inside the framebuffer')


    def test_snake_orchard_cursor_remains_visible_after_large_mouse_motion(self):
        self.snake_orchard_cursor(raw_bytes=True)


    def test_snake_orchard_enables_mouse_reporting(self):
        self.snake_orchard_cursor(raw_bytes=False)


    def test_lcomaze_cursor_keeps_its_whole_sprite_inside_menu_and_game(self):
        self.compile_and_run(COMMUNITY / 'HenriqueSFernandes--LCOMaze/src',
                             ['game/menu.c', 'game/game.c'], r'''
#include <lcom/lcf.h>
#include "game/menu.h"
#include "game/game.h"
#include <assert.h>
vbe_mode_info_t mode_info;
struct packet mouse_packet;
bool host, running;
int state;
double atan2(double y, double x) { abort(); }
int main(void) {
  mode_info=(vbe_mode_info_t){.XResolution=1152,.YResolution=864};
  x_mouse=1132; y_mouse=833;
  mouse_packet=(struct packet){.delta_x=20,.delta_y=-20};menu_mouse_handler();
  if(x_mouse!=1133 || y_mouse!=834) {fprintf(stderr,"Menu must retain whole19x30 cursor at the edge\n");return 1;}
  x_mouse=1132;y_mouse=833;game_mouse_handler();
  if(x_mouse!=1133 || y_mouse!=834) {fprintf(stderr,"Game must retain whole19x30 cursor at the edge\n");return 1;}
  x_mouse=y_mouse=0;mouse_packet=(struct packet){.delta_x=1,.delta_y=-1};menu_mouse_handler();
  assert(x_mouse==0.5 && y_mouse==0.5);
  mouse_packet=(struct packet){.delta_x=-20,.delta_y=20};game_mouse_handler();
  assert(x_mouse==0 && y_mouse==0);
  return 0;
}
''')


    def test_labyrinth_cursor_draw_stays_visible_without_moving_aim(self):
        self.compile_and_run(COMMUNITY / 'Cata-24--minix3-game/proj/src', ['drivers/video.c'], r'''
#include "drivers/video.h"
#include <assert.h>
struct Sprite *mouse;
extern vbe_mode_info_t mode_info;
extern uint8_t *temp_mem;
int main(void) {
  uint32_t pixels[50]={0}, colors[]={0xff000000,0x112233,0x223344,0x334455,0x445566,0x556677};
  pixels[48]=pixels[49]=0xdeadbeef;
  temp_mem=(uint8_t*)pixels;mode_info=(vbe_mode_info_t){.XResolution=8,.YResolution=6,.BitsPerPixel=32};
  struct Sprite cursor={.x_pos=7,.y_pos=5,.width=3,.height=2,.colors=colors};mouse=&cursor;
  assert(drawSprite(mouse,false)==0);
  assert(pixels[4*8+6]==0x112233 && pixels[5*8+7]==0x556677);
  assert(cursor.x_pos==7 && cursor.y_pos==5);
  assert(pixels[48]==0xdeadbeef && pixels[49]==0xdeadbeef);
  assert(vg_draw_pixel(8,5,0xabcdef)!=0 && vg_draw_pixel(7,6,0xabcdef)!=0);
  assert(pixels[48]==0xdeadbeef && pixels[49]==0xdeadbeef);
  return 0;
}
''')


    def test_castle_cursor_draw_preserves_fullscreen_hit_coordinates(self):
        self.compile_and_run(COMMUNITY / 'francis802--Tower-Clash/src', ['view/view.c'], r'''
#include "view/view.h"
#include <assert.h>
Sprite *mouse,*mouse_click;
extern vbe_mode_info_t mode_info;
static int count,invalid;static uint32_t frame[48];
int draw_pixel(uint16_t x,uint16_t y,uint32_t color,uint8_t *buffer) {
  if(x>=8 || y>=6) {invalid++;return 1;} frame[y*8+x]=color;count++;return 0;
}
int main(void) {
  mode_info=(vbe_mode_info_t){.XResolution=8,.YResolution=6};
  uint32_t colors[]={TRANSPARENT,0x112233,0x223344,0x334455,0x445566,0x556677};
  Sprite cursor={.width=3,.height=2,.colors=colors};mouse=&cursor;
  assert(draw_sprite_xpm(mouse,8,6)==0);
  assert(!invalid && count==5 && frame[4*8+6]==0x112233 && frame[5*8+7]==0x556677);
  count=0;mouse_click=&cursor;mouse=NULL;
  assert(draw_sprite_xpm(mouse_click,8,6)==0 && !invalid && count==5);
  Sprite actor=cursor;count=0;
  assert(draw_sprite_xpm(&actor,2,1)==0 && frame[1*8+3]==0x112233 && count==5);
  return 0;
}
''')


    def test_portal_cursor_draw_preserves_aim_at_the_level_edge(self):
        self.compile_and_run(COMMUNITY / 'BrunoRosendo--Portal2D/proj/Src', ['sprites.c'], r'''
#include "sprites.h"
#include <assert.h>
static int count,invalid;static uint16_t frame[48];
unsigned get_h_res(void){return 8;} unsigned get_v_res(void){return 6;}
enum xpm_image_type get_type(void){return XPM_5_6_5;}
uint32_t xpm_transparency_color(enum xpm_image_type type){return 0xffff;}
int vg_draw_pixel(uint16_t x,uint16_t y,uint16_t color) {
  if(x>=8 || y>=6) {invalid++;return 1;} frame[y*8+x]=color;count++;return 0;
}
int main(void) {
  uint16_t colors[]={0xffff,0x1234,0x2345,0x3456,0x4567,0x5678};
  Sprite pointer={.x=7,.y=5,.width=3,.height=2,.map=(uint8_t*)colors};
  draw_cursor(&pointer);
  assert(!invalid && count==5 && frame[4*8+6]==0x1234 && frame[5*8+7]==0x5678);
  assert(pointer.x==7 && pointer.y==5);
  Sprite actor=pointer;actor.x=2;actor.y=1;count=0;
  draw_sprite(&actor);assert(!invalid && count==5 && frame[1*8+3]==0x1234);
  return 0;
}
''')


    def test_euler_cursor_render_keeps_centered_art_inside_frame(self):
        self.compile_and_run(COMMUNITY / 'MurielPinho--FEUP-LCOM-1920/proj/src', ['mouse.c'], r'''
#include "sprite.h"
#include "mouse.h"
#include <assert.h>
static int drawn_x,drawn_y;
int draw_sprite(Sprite *sprite){drawn_x=sprite->x;drawn_y=sprite->y;return 0;}
int main(void) {
  Sprite sprite={.x=800,.y=600,.width=40,.height=40};Mouse mouse={.sprite=&sprite};
  draw_mouse(&mouse);assert(drawn_x==780 && drawn_y==580);
  assert(sprite.x==800 && sprite.y==600);
  sprite.x=sprite.y=0;draw_mouse(&mouse);assert(drawn_x==20 && drawn_y==20);
  assert(sprite.x==0 && sprite.y==0);
  sprite.x=400;sprite.y=300;draw_mouse(&mouse);assert(drawn_x==400 && drawn_y==300);
  return 0;
}
''')


    def test_sushi_cursor_edges_keep_the_original_sprite_visible(self):
        import re
        project=COMMUNITY/'teixeiraa05--lcom-sushi-slash'
        asset=(project/'proj/assets/cursor_point.xpm').read_text()
        width,height=map(int,re.search(r'"(\d+) (\d+) \d+ \d+"',asset).groups())
        self.compile_and_run(project,['lab4/mouse.c'],r'''#include "lab4/mouse.h"
#include <lcom/lcf.h>
#include <assert.h>
int main(void){
 MouseGameState state={.x=400,.y=300};struct packet p={.delta_x=96,.delta_y=-96,.lb=true};
 for(int i=0;i<12;i++)mouse_update_state(&state,&p,800,600);
 assert(state.x==$MAX_X$ && state.y==$MAX_Y$ && state.lb_pressed && state.is_slashing);
 p.delta_x=-1;p.delta_y=1;mouse_update_state(&state,&p,800,600);
 assert(state.x==$MAX_X$-1 && state.y==$MAX_Y$-1);
 p.delta_x=-96;p.delta_y=96;for(int i=0;i<12;i++)mouse_update_state(&state,&p,800,600);
 assert(state.x==0 && state.y==0);
 /* The rightmost exit/button targets and slash area remain reachable. */
 p.delta_x=96;p.delta_y=-96;for(int i=0;i<8;i++)mouse_update_state(&state,&p,800,600);
 assert(state.x>=700 && state.y>=500);return 0;
}'''.replace('$MAX_X$',str(800-width)).replace('$MAX_Y$',str(600-height)))


    def test_feup_hero_menu_cursor_remains_visible_at_right_bottom(self):
        project=COMMUNITY/'martimcferreira--lcom-project/Project'
        self.compile_and_run(project,[],r'''#define main unused_student_main
#include "$STUDENT_MAIN$"
#undef main
#include <assert.h>
int main(void){
 mouse_x=900;mouse_y=900;clamp_mouse_position();
 assert(mouse_x==788 && mouse_y==585);
 mouse_x=-100;mouse_y=-100;clamp_mouse_position();assert(mouse_x==0 && mouse_y==0);
 /* Bottom/right Restart button remains fully reachable. */
 mouse_x=680;mouse_y=570;clamp_mouse_position();assert(mouse_x==680 && mouse_y==570);
 return 0;
}'''.replace('$STUDENT_MAIN$',str(project/'main.c')),include_dirs=[project/'structures/includes',project/'structures/includes/drivers',project/'devices/video',project/'devices/uart'])


    def test_skess_cursor_draw_keeps_canvas_and_edge_hit_coordinates(self):
        project=COMMUNITY/'Yuka-Sky--LCOM/Project/src'
        self.compile_and_run(project,['drawScreens.c','mousekb/mouse.c'],r'''#include "drawScreens.h"
#include "mousekb/mouse.h"
#include <assert.h>
static int draws;
int vg_draw_xpm(xpm_image_t img,uint8_t *map,uint16_t x,uint16_t y){
 assert(x>=0 && y>=0 && x+img.width<=1152 && y+img.height<=864);draws++;return 0;
}
int main(void){
 xpm_image_t img={.width=25,.height=39};uint8_t pixels[25*39*4]={0};
 assert(draw_cursor(1152,864,pixels,img)==0 && draws==1);
 assert(draw_cursor(0,0,pixels,img)==0 && draws==2);
 /* Existing drawing-area coordinates, including Clear, must not change. */
 int x=700,y=820;struct packet p={.delta_x=96,.delta_y=-96};
 calculate_placement(&x,&y,750,864,50,50,p);assert(x==750 && y==864);
 assert(draw_cursor(x,y,pixels,img)==0 && draws==3);return 0;
}''')


    def test_fruit_defense_cursor_display_preserves_edge_placement(self):
        project=COMMUNITY/'guystavo7--feup-lcom-project'
        self.compile_and_run(project,[],r'''#include "project/src/cursorHandler.c"
#include "lab5/spriteXMP.c"
#include <assert.h>
static int opaque_draws,invalid_draws;
uint32_t xpm_transparency_color(enum xpm_image_type type){return 0x00FEFEFE;}
uint8_t *xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t *img){
 int colors,cpp;sscanf(map[0],"%hu %hu %d %d",&img->width,&img->height,&colors,&cpp);
 assert(cpp==1);uint32_t palette[256]={0};
 for(int i=0;i<colors;i++){char *hex=strchr(map[1+i],'#');palette[(unsigned char)map[1+i][0]]=hex?strtoul(hex+1,NULL,16):0xFEFEFE;}
 uint8_t *bytes=malloc(img->width*img->height*3);
 for(int y=0;y<img->height;y++)for(int x=0;x<img->width;x++){uint32_t c=palette[(unsigned char)map[1+colors+y][x]];int at=(y*img->width+x)*3;bytes[at]=c;bytes[at+1]=c>>8;bytes[at+2]=c>>16;}return bytes;
}
unsigned int getBytesPerPixel(Terminal *t){return 3;}
void drawPixel(Terminal *t,int x,int y,uint32_t c){if(x<0||y<0||x>=800||y>=600)invalid_draws++;else opaque_draws++;}
void drawPixelInEmptyBuffer(Terminal *t,int x,int y,uint32_t c){abort();}
int main(void){
 Cursor *c=getCursor();cursorUpdateData(c,1000,1000,false,false,false);cursorUpdateCoordinates(c);
 assert(cursorGetX(c)==799 && cursorGetY(c)==599);
 /* Placement still receives the actual rightmost/bottommost game coordinates. */
 for(int mode=0;mode<3;mode++){
  cursorSetConstructionMode(c,mode!=0);cursorSetPlacementHoverValid(c,mode!=2);
  opaque_draws=invalid_draws=0;cursorUpdate(c,NULL,0);
  assert(opaque_draws>0 && invalid_draws==0 && cursorGetX(c)==799 && cursorGetY(c)==599);
 }
 cursorUpdateData(c,0,0,true,false,false);assert(cursorIsLeftButtonPressedDown(c));
 assert(cursorGetX(c)==799 && cursorGetY(c)==599);cursorFree(c);return 0;
}''')


    def test_whack_hammer_stays_visible_and_can_hit_bottom_right_mole(self):
        import re
        project=COMMUNITY/'Duarte-Pombo--MINIX-Whack-a-Mole/proj/src'
        images=[]
        for name in ['hammerCursor','hammerCursorClick']:
            strings=re.findall(r'"([^"\n]*)"',(project/'sprites'/f'{name}.xpm').read_text())
            width,height,colors,cpp=map(int,strings[0].split())
            transparent={row[:cpp] for row in strings[1:colors+1] if '#000000' in row}
            data=[0 if row[x*cpp:(x+1)*cpp] in transparent else 1
                  for row in strings[colors+1:colors+1+height] for x in range(width)]
            images.append((width,height,','.join(map(str,data))))
        harness=r'''#include "uiElements/cursor.h"
#include "uiElements/mole.h"
#include "settings.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
extern unsigned h_res,v_res;extern uint8_t *video_mem;
extern uint8_t *hammerCursor_pixmap,*hammerCursorClick_pixmap;
extern xpm_image_t hammerCursor_img,hammerCursorClick_img;
static const uint8_t original_mask[]={$MASK$},click_mask[]={$CLICK_MASK$};
static int visible_pixels(void){int total=0;for(int i=0;i<1024*768*3;i+=3)total+=video_mem[i]!=0;return total;}
int main(void){
 h_res=1024;v_res=768;game_settings.mouse_sensitivity=1;
 video_mem=calloc(1024*768,3);hammerCursor_pixmap=calloc($WIDTH$*$HEIGHT$,3);hammerCursorClick_pixmap=calloc($CLICK_WIDTH$*$CLICK_HEIGHT$,3);
 hammerCursor_img.width=$WIDTH$;hammerCursor_img.height=$HEIGHT$;hammerCursorClick_img.width=$CLICK_WIDTH$;hammerCursorClick_img.height=$CLICK_HEIGHT$;
 for(int i=0;i<$WIDTH$*$HEIGHT$;i++)memset(hammerCursor_pixmap+i*3,original_mask[i],3);
 for(int i=0;i<$CLICK_WIDTH$*$CLICK_HEIGHT$;i++)memset(hammerCursorClick_pixmap+i*3,click_mask[i],3);
 assert(cursor_init()==0);cursor_handle_movement(2000,-2000);
 assert(_cursor.x==1024-$WIDTH$ && _cursor.y==768-$HEIGHT$);
 cursor_draw();assert(visible_pixels()>0);set_click_state(true);memset(video_mem,0,1024*768*3);cursor_draw();assert(visible_pixels()>0);
 /* Source bottom/right hole650,400,256x256 remains selectable at its centre. */
 cursor_handle_movement(778-_cursor.x,_cursor.y-528);assert(_cursor.x==778 && _cursor.y==528);
 moles[5].x=mole_positions[5].x;moles[5].y=mole_positions[5].y;moles[5].visible=true;moles[5].hammered=false;
 assert(mole_check_collision(_cursor.x,_cursor.y));assert(moles[5].hammered && !moles[5].visible);
 cursor_cleanup();free(video_mem);free(hammerCursor_pixmap);free(hammerCursorClick_pixmap);return 0;
}'''
        for token,value in zip(['$WIDTH$','$HEIGHT$','$MASK$','$CLICK_WIDTH$','$CLICK_HEIGHT$','$CLICK_MASK$'],[*images[0],*images[1]]):harness=harness.replace(token,str(value))
        self.compile_and_run(project,['uiElements/cursor.c','uiElements/mole.c','labDrivers/video/video_gr.c','settings.c'],harness)


    def test_not_cuphead_rejects_motion_that_hides_its_resolution_cursor(self):
        # The actual default-mode cursor is the original opaque4x4 square.
        cli = Path(os.environ.get('LCOM_CLI', ROOT / 'build/machinelab')).resolve()
        project = COMMUNITY / 'leonardomagalhaes21--feup-LCOM/proj/src'
        with tempfile.TemporaryDirectory(prefix='lcom-cuphead-cursor-') as temporary:
            folder = Path(temporary)
            subprocess.run([str(cli), 'compat', 'build', str(project), '--output', str(folder / 'native')],
                           check=True, capture_output=True, text=True, timeout=120)
            replay = folder / 'cursor.mlabscript'
            replay.write_text('at 180 mouse 0 -240 0\nat 200 mouse 0 -46 0\n')
            frame = folder / 'resolution.ppm'
            result = subprocess.run([str(cli), 'run', '--headless', '--script', str(replay),
                                     '--max-ticks', '240', '--timeout', '30', '--dump-presented',
                                     '--dump-frame', str(frame), '--', str(folder / 'native/proj')],
                                    cwd=folder, capture_output=True, text=True, timeout=45)
            self.assertEqual(result.returncode, 1, result.stdout + result.stderr)
            self.assertIn('max virtual ticks reached (240)', result.stdout + result.stderr)
            magic, dimensions, depth, pixels = frame.read_bytes().split(b'\n', 3)
            self.assertEqual((magic, dimensions, depth), (b'P6', b'640 480', b'255'))
            for y in range(434,438):
                for x in range(222,226):
                    offset=(y*640+x)*3
                    self.assertEqual(pixels[offset:offset+3],b'\xff\xff\xff',
                                     'The invalid second motion must leave the original cursor visible at222,434')


    def test_robot_arena_keeps_crosshair_draw_inside_frame_without_changing_aim(self):
        # The real unbounded pixel renderer would write past its allocation if
        # any part of the14x13 crosshair is submitted beyond the bottom edge.
        self.compile_and_run(COMMUNITY / 'DiogoSilva11--LCOM/proj/src', ['game.c'], r'''
#include <lcom/lcf.h>
#include "game.h"
#include <assert.h>
int leave(void) { abort(); }
static int draws;
int vg_draw_element(xpm_row_t *pixmap, uint16_t x, uint16_t y) {
  int width,height;assert(sscanf(pixmap[0],"%d %d",&width,&height)==2);
  assert(width==14 && height==13);draws++;
  if(x+width>1024 || y+height>768) {
    fprintf(stderr,"Crosshair draw at%u,%u writes outside1024x768 framebuffer\n",x,y);return 1;
  }
  return 0;
}
int main(void) {
  moveCrosshair(32767,-32767);
  assert(crosshairCenterX()==1030 && crosshairCenterY()==773);
  if(drawCrosshair())return 1;
  moveCrosshair(-32767,32767);
  assert(crosshairCenterX()==7 && crosshairCenterY()==6);
  if(drawCrosshair())return 1;
  assert(draws==2);return 0;
}
''')


    def test_star_defender_xpm_clips_framebuffer_edges(self):
        self.compile_and_run(COMMUNITY/'luanalima27--LCOM-FEUP/proj/src',['video.c'],r'''#include "video.h"
#include <assert.h>
extern char *buff;
extern vbe_mode_info_t vmi_p;
uint32_t xpm_transparency_color(enum xpm_image_type t){return 0xfffffe;}
int main(void){
 uint8_t guarded[4*3*3+16],colors[]={1,2,3,4,5,6,7,8,9,10,11,12};
 memset(guarded,0xa5,sizeof guarded);buff=(char*)guarded+8;
 vmi_p=(vbe_mode_info_t){.XResolution=4,.YResolution=3,.BitsPerPixel=24};
 xpm_image_t img={.width=2,.height=2};
 assert(draw_xpm(colors,img,3,2)==0);
 assert(!memcmp(buff+33,colors,3));
 for(int i=0;i<8;i++)assert(guarded[i]==0xa5 && guarded[44+i]==0xa5);
 for(int i=0;i<33;i++)assert((uint8_t)buff[i]==0xa5);
 assert(draw_xpm(colors,img,4,3)==0);
 assert(vg_draw_pixel(4,0,colors)!=0 && vg_draw_pixel(0,3,colors)!=0);
 for(int i=0;i<8;i++)assert(guarded[i]==0xa5 && guarded[44+i]==0xa5);
 memset(buff,0,36);assert(draw_xpm(colors,img,0,0)==0);
 assert(!memcmp(buff,colors,6) && !memcmp(buff+12,colors+6,6));
 return 0;
}''')


    def test_teclas_draws_complete_cursor_without_changing_hotspot(self):
        self.compile_and_run(COMMUNITY/'aishend--TECLAS-The-Typing-Game/teclas',['src/cursor.c'],r'''#include "src/cursor.h"
#include "src/sprite.h"
#include <assert.h>
uint16_t screen_width=800,screen_height=600;
static Sprite cursor={.img={.width=7,.height=5}};
static int drawn_x,drawn_y;
Sprite *sprite_load(xpm_map_t x){return &cursor;}
void sprite_destroy(Sprite *s){}
void sprite_draw(Sprite *s,uint16_t x,uint16_t y){assert(x+7<=800 && y+5<=600);drawn_x=x;drawn_y=y;}
int main(void){cursor_init();cursor_move(2000,-2000);assert(cursor_x==799 && cursor_y==599);cursor_draw();assert(drawn_x==793 && drawn_y==595);assert(cursor_x==799 && cursor_y==599);cursor_move(-2000,2000);cursor_draw();assert(drawn_x==0 && drawn_y==0);cursor_destroy();return 0;}
''')


    def test_tower_defense_draws_cursor_inside_screen_without_moving_slot_hotspot(self):
        self.compile_and_run(COMMUNITY/'DavidGustavo6--LCOM-game/src',['sprites/sprite.c'],r'''#include "sprites/sprite.h"
#include <assert.h>
uint16_t x_res=800,y_res=600;
static int pixels;
int draw_pixel(uint16_t x,uint16_t y,uint32_t color){assert(x<800 && y<600);pixels++;return 0;}
int main(void){uint32_t colors[6]={1,2,3,4,5,6};Sprite cursor={.width=3,.height=2,.x=800,.y=600,.colors=colors};mouse=&cursor;assert(draw_sprite(mouse)==0 && pixels==6);assert(mouse->x==800 && mouse->y==600);cursor.x=0;cursor.y=0;pixels=0;assert(draw_sprite(mouse)==0 && pixels==6);return 0;}
''')


    def test_memory_quest_draws_complete_cursor_in_every_phase(self):
        self.compile_and_run(COMMUNITY/'luizhqueiroz--LCOM-MemoryGame/src',['images/sprite/sprite.c','game/menu/menu.c','game/main_game/main_game.c','game/record/record.c'],r'''#include "game/menu/menu.h"
#include "game/main_game/main_game.h"
#include "game/record/record.h"
#include <assert.h>
unsigned int mode=0;
static int count;
int vg_draw_pixel(uint16_t x,uint16_t y,uint32_t color){assert(x<800 && y<600);count++;return 0;}
int main(void){uint32_t colors[6]={1,2,3,4,5,6};Sprite sprite={.width=3,.height=2,.map=colors};mouse_cursor=select_mouse_cursor=&sprite;mode_info=(vbe_mode_info_t){.XResolution=800,.YResolution=600};int(*draw[])(int,int)={menu_display_mouse,main_game_display_mouse,winner_display_mouse,record_display_mouse};for(int i=0;i<4;i++){count=0;assert(draw[i](799,599)==0 && count==6);count=0;assert(draw[i](0,0)==0 && count==6);}return 0;}
''')


    def test_leiria_cursor_is_visible_at_both_logical_corners(self):
        import re
        cli=Path(os.environ.get('LCOM_CLI',ROOT/'build/machinelab')).resolve()
        project=COMMUNITY/'RodrigoPombo1--projeto-lcom/proj/src'
        rows=re.findall(r'"([^"]*)"',(project/'xpm_files/LCOM_cursor.xpm').read_text())[4:]
        self.assertEqual(len(rows),32)
        with tempfile.TemporaryDirectory(prefix='lcom-leiria-cursor-') as temporary:
            folder=Path(temporary)
            build=subprocess.run([str(cli),'compat','build',str(project),'--output',str(folder/'native')],capture_output=True,text=True,timeout=120)
            self.assertEqual(build.returncode,0,build.stdout+build.stderr)
            for corner in ['top-left','bottom-right']:
                script=folder/(corner+'.mlabscript')
                script.write_text(''.join(f'at {30+i*5} mouse 96 -96 0\n' for i in range(10)) if corner=='bottom-right' else '')
                frame=folder/(corner+'.ppm')
                result=subprocess.run([str(cli),'run','--headless','--script',str(script),'--max-ticks','120','--timeout','30','--dump-presented','--dump-frame',str(frame),'--',str(folder/'native/proj')],cwd=folder,capture_output=True,text=True,timeout=45)
                self.assertEqual(result.returncode,1,result.stdout+result.stderr)
                self.assertIn('max virtual ticks reached (120)',result.stdout+result.stderr)
                magic,dimensions,depth,pixels=frame.read_bytes().split(b'\n',3)
                self.assertEqual((magic,dimensions,depth),(b'P6',b'800 600',b'255'))
                origin=(0,0) if corner=='top-left' else (768,568)
                for y,row in enumerate(rows):
                    for x,value in enumerate(row):
                        if value!='.':continue
                        offset=((origin[1]+y)*800+origin[0]+x)*3
                        self.assertEqual(pixels[offset:offset+3],b'\xff\xff\xff',f'Whole original cursor must remain visible at {corner}: {x},{y}')


    def test_knight_quest_pixel_edges_cannot_wrap_or_overwrite_framebuffer(self):
        self.compile_and_run(COMMUNITY/'antoniocrego--lcom-t01-g3/proj/src',
                             ['controllers/video/video_gr.c'],r'''#include "controllers/video/video_gr.h"
#include <assert.h>
#include <string.h>
vbe_mode_info_t mode_info={.XResolution=800,.YResolution=600,.BitsPerPixel=24};
static uint8_t guarded[32+800*600*3+4096];
uint8_t *second_frame_buffer=guarded+32;
int vg_exit(void){abort();}
int main(void){
 memset(guarded,0xA5,sizeof(guarded));
 paint_pixel(800,0,0x123456);paint_pixel(0,600,0x123456);paint_pixel(800,600,0x123456);
 for(unsigned i=0;i<sizeof(guarded);i++)assert(guarded[i]==0xA5);
 paint_pixel(799,599,0x123456);
 unsigned last=32+(800*600-1)*3;
 assert(guarded[last]==0x56 && guarded[last+1]==0x34 && guarded[last+2]==0x12);
 for(unsigned i=last+3;i<sizeof(guarded);i++)assert(guarded[i]==0xA5);
 return 0;
}''')


    def test_minhoca_pixel_writes_clip_coordinates_and_preserve_24bit_guard_bytes(self):
        # Exercise the actual GPU public API and allocation-sized framebuffer.
        # The original cursor can submit pixels beyond its logical clamp edge.
        self.compile_and_run(COMMUNITY / 'Forreca05--LCOM-Snake-Game-Projeto-2025/proj/src',
                             ['controllers/gpu/gpu.c'], r'''
#include <lcom/lcf.h>
#include "controllers/gpu/gpu.h"
#include <assert.h>
static unsigned char buffer[36+8], front[36];
void *regression_alloc(size_t n) {assert(n==36);memset(buffer,0xa5,sizeof(buffer));return buffer;}
int vbe_get_mode_info(uint16_t mode,vbe_mode_info_t *info) {
  *info=(vbe_mode_info_t){.XResolution=4,.YResolution=3,.BitsPerPixel=24,
    .RedMaskSize=8,.RedFieldPosition=16,.GreenMaskSize=8,.GreenFieldPosition=8,.BlueMaskSize=8,.BlueFieldPosition=0};return 0;
}
int sys_privctl(int who,int request,void *arg) {return 0;}
void *vm_map_phys(int who,void *base,size_t length) {assert(length==36);return front;}
int sys_int86(reg86_t *reg) {return 0;}
void panic(const char *format,...) {abort();}
int main(void) {
  assert(vg_init(0x14c)==front);
  vg_draw_pixel(3,2,0x123456);
  assert(buffer[33]==0x56 && buffer[34]==0x34 && buffer[35]==0x12);
  for(int i=36;i<44;i++)if(buffer[i]!=0xa5) {
    fprintf(stderr,"24-bit final pixel must write exactly3bytes, preserving the allocation guard\n");return 1;
  }
  unsigned char before[44];memcpy(before,buffer,sizeof(buffer));
  vg_draw_pixel(4,0,0xffffff);vg_draw_pixel(0,3,0xffffff);
  if(memcmp(before,buffer,sizeof(buffer))) {
    fprintf(stderr,"Offscreen cursor pixels must never alter framebuffer or guard bytes\n");return 1;
  }
  return 0;
}
''', source_flags=('-Dmalloc=regression_alloc',))


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
