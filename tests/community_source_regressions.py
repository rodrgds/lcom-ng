#!/usr/bin/env python3
"""Compiled regressions for corrected student packet handling and pixel clipping."""
import os
import json
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
    def test_falling_blocks_dispatches_only_completed_mouse_packets(self):
        self.compile_and_run(COMMUNITY / 'tiagorspires--LCOM-Tetris/proj/src', ['main.c', 'mouse.c'], '#include <lcom/lcf.h>\n#include <assert.h>\n#include "game.h"\n#include "mouse.h"\nextern int x,y;\nint counter;uint8_t scancode;static unsigned event,byte_index,acks;\nstatic const uint8_t bytes[]={0x08,20,0,0x08,30,0};\nint set_frame_buffer(uint16_t m){assert(m==0x105);return 0;}\nint set_video_mode(uint16_t m){assert(m==0x105);return 0;}\nint timer_set_frequency(uint8_t t,uint32_t f){assert(t==0&&f==30);return 0;}\nint timer_subscribe_int(uint8_t*b){*b=1;return 0;}\nint keyboard_subscribe(uint8_t*b){*b=2;return 0;}\nint timer_unsubscribe_int(void){return 0;}\nint keyboard_unsubscribe(void){return 0;}\nint(sys_irqsetpolicy)(int irq,int policy,int*hook){assert(irq==12);(void)policy;*hook=3;return 0;}\nint sys_irqrmpolicy(int*hook){assert(*hook==3);return 0;}\nint(sys_outb)(int p,uint32_t v){(void)p;(void)v;return 0;}\nint util_sys_inb(int p,uint8_t*v){if(p==0x64)*v=0x21;else{assert(p==0x60);if(event==0||event==7){*v=0xfa;acks++;}else{assert(byte_index<sizeof bytes);*v=bytes[byte_index++];}}return 0;}\nint driver_receive(endpoint_t who,message*m,int*status){(void)who;if(event==4)assert(x==30&&"partial second mouse packet must not repeat the previous20px movement");if(event==6)assert(x==60&&y==10);assert(event<=6);m->m_source=HARDWARE;m->m_notify.interrupts=event<6?4:2;*status=IPC_STATUS_NOTIFY;event++;return 0;}\nvoid kbc_ih_keyboard(void){scancode=0x81;}\nvoid timer_int_handler(void){counter++;}\nint escape_key(void){return 0;}\nint vg_exit(void){return 0;}\nint clean_buffer(void){return 0;}\nint swap_buffer(void){return 0;}\nint draw_xpm(uint16_t a,uint16_t b,xpm_map_t c){(void)a;(void)b;(void)c;return 0;}\nint draw_centered_rectangles(int a,int b){assert(a==1024&&b==768);return 0;}\nvoid draw_word_in_rectangle(int a,int b,const xpm_row_t*c[],int d){(void)a;(void)b;(void)c;(void)d;}\nbool is_piece_at_bottom(TetrisPiece*p,char s[24][32],int c[24][32]){(void)p;(void)s;(void)c;abort();}\nvoid check_and_clear_full_lines(char s[24][32],int c[24][32],TetrisPiece*p,int n){(void)s;(void)c;(void)p;(void)n;abort();}\nbool game_over(char s[24][32]){(void)s;return false;}\nvoid handle_game_over(GameState*g,char s[24][32],int c[24][32],TetrisPiece*p,int*n){(void)g;(void)s;(void)c;(void)p;(void)n;abort();}\nTetrisPiece generate_random_piece(void){abort();}\nvoid move_piece(TetrisPiece*p,int x,int y,char s[24][32],int c[24][32]){(void)p;(void)x;(void)y;(void)s;(void)c;abort();}\nvoid rotate_piece(TetrisPiece*p,char s[24][32],int c[24][32]){(void)p;(void)s;(void)c;abort();}\nvoid drop_until_bottom(TetrisPiece*p,char s[24][32],int c[24][32]){(void)p;(void)s;(void)c;abort();}\nbool is_within_rectangle(int x,int y,int a,int b,int w,int h){(void)x;(void)y;(void)a;(void)b;(void)w;(void)h;return false;}\nvoid reset_screen(char s[24][32],int c[24][32]){(void)s;(void)c;abort();}\nvoid draw(char s[24][32],int c[24][32],TetrisPiece*p,int n){(void)s;(void)c;(void)p;(void)n;abort();}\nint main(void){assert(proj_main_loop(0,NULL)==0);assert(event==7&&byte_index==6&&x==60&&y==10);return 0;}\n', source_flags=['-O2', '-Dmain=tetris_program_entry'])
    def test_falling_blocks_renderer_clips_edges_and_releases_loaded_art(self):
        self.compile_and_run(COMMUNITY / 'tiagorspires--LCOM-Tetris/proj/src', ['video.c'], '#include <lcom/lcf.h>\n#include <assert.h>\n#include "video.h"\nstatic uint8_t*loaded;static unsigned releases;\nvoid tetris_checked_free(void*p){assert(p==loaded);releases++;free(p);loaded=NULL;}\nuint8_t*xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*image){(void)map;assert(type==XPM_INDEXED);loaded=malloc(4);assert(loaded);memcpy(loaded,(uint8_t[]){1,2,3,4},4);*image=(xpm_image_t){.width=2,.height=2,.size=4,.bytes=loaded};return loaded;}\nint main(void){mode_info=(vbe_mode_info_t){.XResolution=4,.YResolution=3,.BitsPerPixel=8};uint8_t page[44];memset(page,0xcc,sizeof page);double_buffer=page+16;assert(set_pixel_color(1,1,7)==0&&page[21]==7);assert(set_pixel_color(3,2,9)==0&&page[27]==9);uint8_t before[44];memcpy(before,page,sizeof page);assert(set_pixel_color(4,0,1)==1&&"x equal to width must be rejected");assert(set_pixel_color(0,3,1)==1&&"y equal to height must be rejected");assert(memcmp(before,page,sizeof page)==0);assert(draw_xpm(1,1,NULL)==0);assert(page[21]==1&&page[22]==2&&page[25]==3&&page[26]==4&&releases==1);assert(draw_xpm(3,2,NULL)==1&&releases==2);for(unsigned i=0;i<16;i++)assert(page[i]==0xcc&&page[28+i]==0xcc);return 0;}\n', source_flags=['-O2', '-Dfree=tetris_checked_free'])
    def test_doodle_first_normal_enter_starts_round_with_defined_keyboard_packet(self):
        self.compile_and_run(COMMUNITY / 'pedrorc12--LCOM-proj', ['game.c'], '#include <lcom/lcf.h>\n#include <assert.h>\n#include "game.h"\nextern uint8_t timer_irq,kbc_irq;\nuint8_t scancode;uint32_t counter;uint16_t hres=800,vres=600;static unsigned event,moves;\nstatic const uint8_t keys[]={0x1c,0,0x81,0xe0,0x50,0x1c,0xe0,0x50,0x1c,0xe0,0x4d,0x1c};\nSprite*create_sprite(xpm_map_t pic,int x,int y,int xspeed,int yspeed){(void)pic;Sprite*s=calloc(1,sizeof *s);s->x=x;s->y=y;s->width=s->height=8;s->xspeed=xspeed;s->yspeed=yspeed;return s;}\nvoid destroy_sprite(Sprite*s){free(s);}\nvoid draw_sprite(Sprite*s){(void)s;}\nvoid clear_sprite(Sprite*s){(void)s;}\nvoid move_sprite(Sprite*s){(void)s;moves++;}\nvoid create_background(xpm_map_t p){(void)p;}\nvoid draw_background(void){}\nvoid page_flip(void){}\nvoid timer_int_handler(void){counter++;}\nvoid kbc_ih(void){scancode=keys[event-1];}\nint driver_receive(endpoint_t who,message*m,int*status){(void)who;assert(event<sizeof(keys));if(event==2)assert(moves>0&&"the first ordinary Enter must start the playable round");m->m_source=HARDWARE;*status=IPC_STATUS_NOTIFY;m->m_notify.interrupts=event==1?2:1;event++;return 0;}\nint main(void){timer_irq=2;kbc_irq=1;assert(game_main_loop()==0);assert(moves>0);return 0;}\n', source_flags=['-O2', '-ftrivial-auto-var-init=pattern'])
    def test_doodle_maps_two_byte_sized_pages_with_correct_display_spacing(self):
        self.compile_and_run(COMMUNITY / 'pedrorc12--LCOM-proj', ['video.c'], '#include <lcom/lcf.h>\n#include <assert.h>\n#include "video.h"\nuint8_t*background;static unsigned mappings,flips;\nint sys_privctl(endpoint_t who,int req,void*arg){(void)who;(void)req;struct minix_mem_range*r=arg;assert(r->mr_limit-r->mr_base==1440000&&"each800x600RGB24 page contains1.44MB, not11.52MB");return 0;}\nvoid*vm_map_phys(endpoint_t who,void*address,size_t bytes){(void)who;assert(mappings<2&&bytes==1440000);assert((uintptr_t)address==0xe0000000u+mappings*1440000u);return(void*)(uintptr_t)(0x1000u+(mappings++)*0x1000u);}\nint(sys_int86)(struct reg86*r){if(r->ax==0x4f06)r->bx=2400;else{assert(r->ax==0x4f07);assert(r->dx==(flips++?600:0));}return 0;}\nvoid panic(const char*fmt,...){(void)fmt;abort();}\nint main(void){vbe_mode_info_t mode={.XResolution=800,.YResolution=600,.BitsPerPixel=24,.BytesPerScanLine=2400,.PhysBasePtr=0xe0000000};assert(video_map(&mode)==0&&mappings==2);page_flip();page_flip();assert(flips==2);return 0;}\n', source_flags=['-O2'])
    def test_doodle_pixel_writers_update_every_channel_and_clip_both_pages(self):
        self.compile_and_run(COMMUNITY / 'pedrorc12--LCOM-proj', ['video.c'], '#include <lcom/lcf.h>\n#include <assert.h>\n#include "video.h"\nuint8_t*background;static uint8_t pages[2][176];static unsigned mapped;\nint sys_privctl(endpoint_t who,int req,void*arg){(void)who;(void)req;(void)arg;return 0;}\nvoid*vm_map_phys(endpoint_t who,void*address,size_t bytes){(void)who;(void)address;(void)bytes;assert(mapped<2);return pages[mapped++]+16;}\nint(sys_int86)(struct reg86*regs){if(regs->ax==0x4f06)regs->bx=24;else assert(regs->ax==0x4f07);return 0;}\nvoid panic(const char*fmt,...){(void)fmt;abort();}\nstatic void color(unsigned page,unsigned offset,uint8_t b,uint8_t g,uint8_t r){assert(pages[page][16+offset]==b&&pages[page][17+offset]==g&&pages[page][18+offset]==r);}\nint main(void){memset(pages,0xa5,sizeof pages);vbe_mode_info_t info={.XResolution=8,.YResolution=6,.BitsPerPixel=24,.BytesPerScanLine=24,.PhysBasePtr=0xe0000000};assert(video_map(&info)==0);draw_pixel(0,0,0x123456);draw_pixel(0,0,0x112256);color(0,0,0x56,0x22,0x11);draw_pixel(7,5,0xfedcba);page_flip();draw_pixel(0,0,0x987654);draw_pixel(7,5,0xfedcba);uint8_t before[2][176];memcpy(before,pages,sizeof pages);draw_pixel(8,0,0x112233);draw_pixel(0,6,0x112233);page_flip();draw_pixel(8,0,0x112233);draw_pixel(0,6,0x112233);assert(memcmp(before,pages,sizeof pages)==0&&"edge pixels must be clipped before writing either video page");color(0,141,0xba,0xdc,0xfe);color(1,141,0xba,0xdc,0xfe);return 0;}\n', source_flags=['-O2'])
    def test_doodle_game_over_clear_clips_original_background_reads(self):
        self.compile_and_run(COMMUNITY / 'pedrorc12--LCOM-proj', ['sprite.c'], '#include <lcom/lcf.h>\n#include <assert.h>\n#include <sys/mman.h>\n#include <unistd.h>\n#include "sprite.h"\nuint16_t hres=8,vres=6;static uint8_t* pixels;static unsigned calls;static bool expected[48];\nuint8_t*xpm_load(xpm_map_t pic,enum xpm_image_type type,xpm_image_t*image){(void)pic;(void)type;image->width=8;image->height=6;image->size=144;return pixels;}\nvoid draw_pixel(uint16_t x,uint16_t y,uint32_t color){assert(x<8&&y<6);unsigned i=y*8+x;assert(expected[i]);assert(color==(0x300000u|((i+1)<<8)|(i+2)));expected[i]=false;calls++;}\nstatic void check(int x,int y,int w,int h){calls=0;unsigned total=0;memset(expected,0,sizeof expected);for(int yy=0;yy<6;yy++)for(int xx=0;xx<8;xx++)if(xx>=x&&xx<x+w&&yy>=y&&yy<y+h){expected[yy*8+xx]=true;total++;}Sprite sprite={.x=x,.y=y,.width=w,.height=h};clear_sprite(&sprite);assert(calls==total);}\nint main(void){size_t page=sysconf(_SC_PAGESIZE);uint8_t*region=mmap(NULL,page*2,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);assert(region!=MAP_FAILED);assert(mprotect(region+page,page,PROT_NONE)==0);pixels=region+page-144;for(unsigned i=0;i<48;i++){pixels[i*3]=i+2;pixels[i*3+1]=i+1;pixels[i*3+2]=0x30;}create_background(NULL);check(2,1,2,2);check(7,5,3,3);check(-1,-1,2,2);assert(munmap(region,page*2)==0);return 0;}\n', source_flags=['-O2'])
    def test_mario_original_mode_information_unit_compiles_and_returns_the_bios_mode(self):
        self.compile_and_run(COMMUNITY/'FranciscaCerquinho--feup-LCOM/src',['vbe.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "vbe.h"
static vbe_mode_info_t bios;
void*lm_init(void){return &bios;}
void*lm_alloc(size_t size,mmap_t*map){assert(size==256);map->phys=0x10000;map->virtual=&bios;map->size=size;return &bios;}
void lm_free(mmap_t*map){assert(map->virtual==&bios);}
int lcom_compat_legacy_int86(struct reg86u*r){assert(r->u.b.intno==0x10&&r->u.w.ax==0x4f01&&r->u.w.cx==0x117&&r->u.w.es==0x1000&&r->u.w.di==0);bios.XResolution=1024;bios.YResolution=768;bios.BitsPerPixel=16;bios.PhysBasePtr=0xe0000000u;r->u.w.ax=0x004f;return 0;}
int main(void){vbe_mode_info_t info={0};assert(vbe_get_mode_info(0x117,&info)==0);assert(info.XResolution==1024&&info.YResolution==768&&info.BitsPerPixel==16&&info.PhysBasePtr==0xe0000000u);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])


    def test_mario_rtc_reads_the_callers_value_instead_of_its_local_pointer(self):
        self.compile_and_run(COMMUNITY/'FranciscaCerquinho--feup-LCOM/src',['rtc.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "rtc.h"
#include <assert.h>
static long*destination;static unsigned address;
int (sys_outb)(int port,uint32_t value){assert(port==0x70);address=value;return 0;}
int(sys_inb)(int port,uint32_t*value){assert(!"readDate must pass the actual caller long buffer, not a pointer-to-pointer");return 1;}
int lcom_compat_sys_inb_long(int port,long*value){assert(port==0x71&&value==destination);*value=address==9?0x26:0x59;return 0;}
int main(void){struct{long before,value,after;}buffer={0x1122,0,0x3344};destination=&buffer.value;assert(readDate(9,&buffer.value)==0x26&&buffer.value==0x26);assert(readDate(0,&buffer.value)==0x59&&buffer.value==0x59);assert(buffer.before==0x1122&&buffer.after==0x3344);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Wno-int-conversion','-fstack-protector-strong'])


    def test_mario_void_irq_cleanup_calls_each_original_device_owner(self):
        self.compile_and_run(COMMUNITY/'FranciscaCerquinho--feup-LCOM/src',['IRQ.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include "IRQ.h"
#include <assert.h>
static int calls;
int timer_unsubscribe_int(void){assert(calls++==0);return 0;}
int keyboard_unsubscribe_int(void){assert(calls++==1);return 0;}
int mouse_unsubscribe_int(void){assert(calls++==2);return 0;}
int main(void){Unsubscribe();assert(calls==3);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Werror=return-type'])


    def test_mario_pipe_collision_returns_false_for_running_and_airborne_safe_positions(self):
        self.compile_and_run(COMMUNITY/'FranciscaCerquinho--feup-LCOM/src',['Colision.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include "Colision.h"
#include <assert.h>
int main(void){Bitmap bitmap={0};bitmap.bitmapInfoHeader.height=100;Pipe pipe={0};pipe.image=&bitmap;pipe.x=900;pipe.y=599;mario player={0};player.x=100;player.y=601;int(*volatile collision)(mario*,Pipe*)=ColisionPipe;assert(collision(&player,&pipe)==0&&"a distant pipe must not end the original running game");pipe.x=125;assert(collision(&player,&pipe)==1);player.y=401;assert(collision(&player,&pipe)==0&&"a jump clears the pipe");pipe.x=100;player.y=601;assert(collision(&player,&pipe)==0);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])


    def test_candy_maze_player_constructor_preserves_owned_pixel_allocation(self):
        self.compile_and_run(COMMUNITY/'manuelcralves--FEUP-LCOM/proj/src',['player.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "player.h"
uint8_t*xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*img){assert(map&&type==XPM_8_8_8_8);uint8_t*p=malloc(24);assert(p);memset(p,0x34,24);*img=(xpm_image_t){.width=3,.height=2,.size=24,.bytes=p};return p;}
int main(void){Player*p=createPlayer();assert(p&&p->x==0&&p->y==0&&p->speed==4&&p->candy==0);assert(p->img.width==3&&p->img.height==2&&p->img.bytes[23]==0x34);free(p->img.bytes);free(p);return 0;}
''',source_flags=['-O2'])


    def test_candy_maze_dimension_consumers_match_graphics_owner(self):
        project=COMMUNITY/'manuelcralves--FEUP-LCOM/proj/src'
        for source in ['player.c','level.c','button.c','controllers/mouse/mouse.c']:
            with self.subTest(source=source):
                if source=='player.c':
                    harness=r'''#include <lcom/lcf.h>
#include <assert.h>
#include "controllers/graphics/graphics.c"
#include "player.c"
uint32_t xpm_transparency_color(enum xpm_image_type t){assert(t==XPM_8_8_8_8);return 0xFF000000;}
Level*level;
uint8_t scancode;
GameState gameState;
void cleanCandy(Candy*c){(void)c;abort();}
int main(void){xRes=800;yRes=600;bytesPerPixel=4;mode_info.BitsPerPixel=32;mode_info.XResolution=800;mode_info.YResolution=600;video_mem=calloc(800*600,4);assert(video_mem);Level map={0};map.level_back=calloc(800*600,4);assert(map.level_back);level=&map;uint8_t sprite[40*38*4];memset(sprite,0x34,sizeof sprite);Player player={.x=60,.y=540,.speed=4,.img={.width=40,.height=38,.bytes=sprite}};checkMove(&player,false,false,false,true);assert(player.x==64&&player.y==540);checkMove(&player,true,false,false,false);assert(player.x==64&&player.y==536);assert(((uint32_t*)video_mem)[536*800+64]==0x34343434);free(map.level_back);free(video_mem);return 0;}
'''
                else:
                    harness='#include <lcom/lcf.h>\n#include <assert.h>\n#include "controllers/graphics/graphics.c"\n#include "'+source+'"\nint main(void){xRes=800;yRes=600;assert(sizeof xRes==2&&sizeof yRes==2&&xRes==800&&yRes==600);return 0;}\n'
                self.compile_and_run(project,[],harness)


    def test_number_grid_native_pixel_writes_keep_high_framebuffer_addresses(self):
        project=COMMUNITY/'nunoMeloCastro--FEUP-LCOM'
        self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdint.h>
static uint8_t*regions[3];static unsigned writes;
static void* checked_copy(void*dst,const void*src,size_t bytes){bool valid=false;for(unsigned i=0;i<3;i++)if(regions[i]&&(uintptr_t)dst>=(uintptr_t)regions[i]&&(uintptr_t)dst+bytes<=(uintptr_t)regions[i]+64)valid=true;assert(valid);writes++;return memcpy(dst,src,bytes);}
#define memcpy checked_copy
#include "libs/vbe/src/vbe_functions.c"
#undef memcpy
uint8_t*xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*img){assert(map);uint8_t*p=malloc(3);assert(p);p[0]=0x12;p[1]=0x34;p[2]=0x56;*img=(xpm_image_t){.width=1,.height=1,.size=3,.bytes=p};return p;}
int main(void){vbe_mode_info.XResolution=4;vbe_mode_info.YResolution=3;vbe_mode_info.BitsPerPixel=24;for(unsigned i=0;i<3;i++){regions[i]=malloc(64);assert(regions[i]&&(uintptr_t)regions[i]>UINT32_MAX);memset(regions[i],0xcc,64);}video_mem=regions[0];buff2=regions[1];buff3=regions[2];assert(vg_set_pixel(1,1,0x563412)==0);assert(vg_set_pixel_b2(1,1,0x563412)==0);uint8_t mapbytes[3]={0x12,0x34,0x56};xpm_image_t img={.width=1,.height=1,.size=3,.bytes=mapbytes};assert(vg_set_pixmap_mi(1,1,mapbytes,img)==0);assert(writes==4);for(unsigned i=0;i<3;i++){assert(regions[i][14]==0xcc&&regions[i][15]==0x12&&regions[i][16]==0x34&&regions[i][17]==0x56&&regions[i][18]==0xcc);free(regions[i]);}return 0;}
''')

    def test_remote_arena_native_pixel_writes_keep_high_framebuffer_address(self):
        project=COMMUNITY/'TiagoBranquinho--LCOM_Bomberman/proj/src'
        self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdint.h>
static uint8_t*regions[3];static unsigned writes;
static void* checked_copy(void*dst,const void*src,size_t bytes){bool valid=false;for(unsigned i=0;i<3;i++)if(regions[i]&&(uintptr_t)dst>=(uintptr_t)regions[i]&&(uintptr_t)dst+bytes<=(uintptr_t)regions[i]+64)valid=true;assert(valid);writes++;return memcpy(dst,src,bytes);}
#define memcpy checked_copy
#include "libs/graph/src/videocard.c"
#undef memcpy
int main(void){vbe_mem_info.XResolution=4;vbe_mem_info.YResolution=3;vbe_mem_info.BytesPerScanLine=12;bytes_per_pixel=3;bytes_per_line=12;regions[0]=malloc(64);assert(regions[0]&&(uintptr_t)regions[0]>UINT32_MAX);memset(regions[0],0xcc,64);video_buf=regions[0];assert(vg_draw_pixel(1,1,0x563412)==0);assert(writes==1);assert(regions[0][14]==0xcc&&regions[0][15]==0x12&&regions[0][16]==0x34&&regions[0][17]==0x56&&regions[0][18]==0xcc);free(regions[0]);return 0;}
''',include_dirs=[project/'libs/graph/include'])

    def test_remote_arena_font_table_retains_bomb_glyph_and_destroys_owned_glyphs(self):
        project=COMMUNITY/'TiagoBranquinho--LCOM_Bomberman/proj/src'
        self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdint.h>
static void*allocs[8];static size_t sizes[8];static unsigned count,loads,frees;static uintptr_t glyph_tokens[64];
static void*checked_malloc(size_t bytes){uint8_t*p=malloc(bytes+16);assert(p&&count<8);memset(p,0xa5,bytes+16);allocs[count]=p;sizes[count++]=bytes;return p;}
static void*checked_calloc(size_t n,size_t bytes){void*p=checked_malloc(n*bytes);memset(p,0,n*bytes);return p;}
#define malloc checked_malloc
#define calloc checked_calloc
#include "libs/graph/src/font.c"
#undef malloc
#undef calloc
sprite_t*sprite_constructor(const char*const*xpm){assert(xpm&&loads<64);return (sprite_t*)&glyph_tokens[loads++];}
void sprite_destructor(sprite_t*p){assert(p);bool known=false;for(unsigned i=0;i<loads;i++)if(p==(sprite_t*)&glyph_tokens[i]){assert(glyph_tokens[i]==0);glyph_tokens[i]=1;known=true;break;}assert(known);frees++;}
int main(void){font_t*f=font_ctor();assert(f&&loads==41);for(unsigned i=0;i<count;i++)for(unsigned j=0;j<16;j++)assert(((uint8_t*)allocs[i])[sizes[i]+j]==0xa5);assert(f->sz>=99&&f->glyphs[98]);for(size_t i=0;i<f->sz;i++){bool supported=(i==44||i==46||i==47||(i>=48&&i<=58)||(i>=65&&i<=90)||i==98);assert((f->glyphs[i]!=NULL)==supported);}font_dtor(f);assert(frees==loads);free(f);return 0;}
''',include_dirs=[project/'libs/graph/include',project/'project/include',project/'libs/kbc/include',project/'media'])

    def test_remote_arena_menu_startup_allocates_all_three_buttons(self):
        project=COMMUNITY/'TiagoBranquinho--LCOM_Bomberman/proj/src'
        source=(project/'project/src/game.c').read_text()
        start=source.index('  menu_t *main_menu = menu_ctor(')
        end=source.index('  leaderboard_t *leaderboard =',start)
        setup=source[start:end]
        self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdint.h>
static void*allocs[4];static size_t sizes[4];static unsigned count,created;
static void*checked_malloc(size_t bytes){uint8_t*p=malloc(bytes+16);assert(p&&count<4);memset(p,0xa5,bytes+16);allocs[count]=p;sizes[count++]=bytes;return p;}
#define malloc checked_malloc
#include "libs/graph/src/menu.c"
#undef malloc
static rectangle_t rects[3];
rectangle_t*rectangle_ctor(int16_t x,int16_t y,uint16_t w,uint16_t h,font_t*f,char*phrase){assert(created<3);rects[created]=(rectangle_t){.x=x,.y=y,.w=w,.h=h,.font=f,.phrase=phrase};return &rects[created++];}
int main(void){font_t*font=(font_t*)(uintptr_t)1;
'''+setup+r'''
assert(created==3&&main_menu->sz==3);for(unsigned i=0;i<count;i++)for(unsigned j=0;j<16;j++)assert(((uint8_t*)allocs[i])[sizes[i]+j]==0xa5);assert(!strcmp(main_menu->buttons[0]->phrase,"PLAY THE GAME"));assert(!strcmp(main_menu->buttons[1]->phrase,"LEADERBOARD"));assert(!strcmp(main_menu->buttons[2]->phrase,"EXIT"));for(unsigned i=0;i<count;i++)free(allocs[i]);return 0;}
''',include_dirs=[project/'libs/graph/include',project/'project/include',project/'libs/kbc/include',project/'media',project/'libs/rtc/include'])

    def test_remote_arena_scores_preserve_caller_storage_and_sort_original_rows(self):
        project=COMMUNITY/'TiagoBranquinho--LCOM_Bomberman/proj/src'
        self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <unistd.h>
#undef fopen
#include "project/src/leaderboard.c"
static char drawn[3][100];static unsigned lines;
void font_draw_string(font_t*font,char*text,uint16_t x,uint16_t y){assert(font);assert(lines<3);snprintf(drawn[lines++],100,"%s",text);}
void rtc_get_date(char*date){strcpy(date,"17/10/2026");}
int main(void){char path[]="/tmp/lcom-bomber-score-XXXXXX";int fd=mkstemp(path);assert(fd>=0);close(fd);leaderboard_t*lb=leaderboard_constructor((font_t*)(uintptr_t)1);assert(lb);lb->file_location=path;struct{char before[4];char name[15];char after[4];}caller={.before="ABC",.name="ALEX",.after="XYZ"};leaderboard_save_file(lb,caller.name,120);assert(!strcmp(caller.name,"ALEX")&&!strcmp(caller.before,"ABC")&&!strcmp(caller.after,"XYZ"));char second[15]="BETA";leaderboard_save_file(lb,second,60);assert(!strcmp(second,"BETA"));leaderboard_sort_entries(lb);leaderboard_draw(lb);assert(lines==3&&!strcmp(drawn[0],"LEADERBOARD"));assert(!strcmp(drawn[1],"BETA            1 S       17/10/2026"));assert(!strcmp(drawn[2],"ALEX            2 S       17/10/2026"));leaderboard_destructor(lb);assert(unlink(path)==0);return 0;}
''',include_dirs=[project/'libs',project/'libs/timer/include',project/'libs/utils/include',project/'libs/graph/include',project/'project/include',project/'libs/kbc/include',project/'libs/rtc/include',project/'media'])

    def test_remote_arena_score_read_does_not_free_automatic_line_storage(self):
        project=COMMUNITY/'TiagoBranquinho--LCOM_Bomberman/proj/src'
        self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <unistd.h>
#undef fopen
#include "project/src/leaderboard.c"
static char drawn[3][100];static unsigned lines;
void font_draw_string(font_t*font,char*text,uint16_t x,uint16_t y){assert(font);assert(lines<3);snprintf(drawn[lines++],100,"%s",text);}
void rtc_get_date(char*date){strcpy(date,"17/10/2026");}
int main(void){char path[]="/tmp/lcom-bomber-read-XXXXXX";int fd=mkstemp(path);assert(fd>=0);FILE*f=fdopen(fd,"w");assert(f);fputs("BETA            1 S       17/10/2026\nALEX            2 S       17/10/2026\n",f);fclose(f);leaderboard_t*lb=leaderboard_constructor((font_t*)(uintptr_t)1);lb->file_location=path;leaderboard_draw(lb);assert(lines==3&&!strcmp(drawn[1],"BETA            1 S       17/10/2026")&&!strcmp(drawn[2],"ALEX            2 S       17/10/2026"));leaderboard_destructor(lb);assert(unlink(path)==0);return 0;}''',include_dirs=[project/'libs',project/'libs/timer/include',project/'libs/utils/include',project/'libs/graph/include',project/'project/include',project/'libs/kbc/include',project/'libs/rtc/include',project/'media'])

    def test_naval_tactics_rtc_reads_preserve_adjacent_bytes(self):
        project=COMMUNITY/'apilifogladif--LCOM2324_Project/proj/src'
        self.compile_and_run(project,['controllers/rtc/rtc.c', 'controllers/utils/utils.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "controllers/rtc/rtc.h"
#undef sys_inb
static uint32_t selected;
int sys_outb(int port,uint32_t value){assert(port==0x70);selected=value;return 0;}
int sys_inb(int port,uint32_t*value){assert(port==0x71&&value);*value=selected==0x0a?0:selected==0x0b?0:0x23;return 0;}
int main(void){struct {uint8_t before,hour,after[3];} guarded={0x67,0xff,{0x89,0xab,0xcd}};assert(readRTC(&guarded.hour,4)==0);assert(guarded.hour==23);assert(guarded.before==0x67&&guarded.after[0]==0x89&&guarded.after[1]==0xab&&guarded.after[2]==0xcd);assert(timeRTC()==1);return 0;}
''',source_flags=['-O1'])

    def test_naval_tactics_local_clicks_ignore_outside_grid(self):
        project=COMMUNITY/'apilifogladif--LCOM2324_Project/proj/src'
        self.compile_and_run(project,['handler/gameLocalController.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "handler/gameLocalController.h"
bool cursor_active=true,changeTurn=false;int global_counter=123;enum GAMESTATE gameState=TURN1;Player p1,p2;static Sprite target;Sprite*crosshair=&target;static unsigned damages,renders,swaps;
void drawGameLocal(void){renders++;}void updateDamage(int taken,int player){assert(taken==2&&player==2);damages++;}bool drawPlayerSecretBoard(int player){assert(player==2);return false;}void swap_buffers(void){swaps++;}unsigned lcom_compat_sleep(unsigned seconds){assert(seconds==1);return 0;}
int main(void){int outside[][2]={{733,96},{54,775},{-16,96},{54,26}};for(unsigned i=0;i<4;i++){memset(&p2,0,sizeof p2);p2.matrix[1][0].taken=2;Player before=p2;target.x=outside[i][0];target.y=outside[i][1];checkGameLocalClick();assert(!damages&&!renders&&!swaps&&cursor_active&&global_counter==123&&!memcmp(&before,&p2,sizeof p2));}memset(&p2,0,sizeof p2);p2.matrix[9][9].taken=2;target.x=732;target.y=774;checkGameLocalClick();assert(damages==1&&renders==1&&swaps==1&&p2.matrix[9][9].attacked==1&&global_counter==0&&cursor_active);return 0;}
''')

    def test_naval_tactics_remote_clicks_ignore_outside_grid(self):
        project=COMMUNITY/'apilifogladif--LCOM2324_Project/proj/src'
        self.compile_and_run(project,['handler/gameRemoteController.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "handler/gameRemoteController.h"
#include "model/game.h"
extern bool cursorIsActive;extern enum THISPLAYER player_status;extern int c_attack,l_attack;Player p1;static Sprite target;Sprite*crosshair=&target;static unsigned sent;
int send(uint8_t byte){assert(byte==0xbb);sent++;return 0;}
int main(void){player_status=MYTURN;int outside[][2]={{733,96},{54,775},{-16,96},{54,26}};for(unsigned i=0;i<4;i++){target.x=outside[i][0];target.y=outside[i][1];checkGameRemoteClick();assert(!sent&&c_attack==-1&&l_attack==-1&&cursorIsActive);}target.x=732;target.y=774;checkGameRemoteClick();assert(sent==1&&c_attack==9&&l_attack==9&&cursorIsActive);return 0;}
''')

    def test_sentence_sprint_native_pixel_writes_preserve_high_framebuffer_addresses(self):
        self.compile_and_run(COMMUNITY/'RodolfoF31--FEUP_LCOM/proj/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdint.h>
static uint8_t*region;static unsigned writes;
static void*checked_copy(void*dst,const void*src,size_t bytes){assert((uintptr_t)dst>=(uintptr_t)region&&(uintptr_t)dst+bytes<=(uintptr_t)region+64);writes++;return memcpy(dst,src,bytes);}
#define memcpy checked_copy
#include "Modules/vg.c"
#undef memcpy
int main(void){region=malloc(64);assert(region&&(uintptr_t)region>UINT32_MAX);video_buffer=region;modeInfo.XResolution=4;modeInfo.YResolution=3;modeInfo.BitsPerPixel=8;memset(region,0xcc,64);assert(draw_pixel(1,1,0x12)==0);assert(writes==1&&region[4]==0xcc&&region[5]==0x12&&region[6]==0xcc);modeInfo.BitsPerPixel=24;memset(region,0xcc,64);assert(draw_pixel(1,1,0x563412)==0);assert(writes==2&&region[14]==0xcc&&region[15]==0x12&&region[16]==0x34&&region[17]==0x56&&region[18]==0xcc);free(region);return 0;}
''')

    def test_bomberman_relocated_pixmap_header_supports_actual_sprite_constructor(self):
        self.compile_and_run((COMMUNITY/'AmandaTartarotti--bomberman_c'/'.'),['src/sprite.c','src/utils.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "inc/sprite.h"
static const char*const picture[]={"3 2 2 1",". c #000000","x c #ffffff","x.x",".x."};static int fail;
uint8_t*xpm_load(xpm_map_t xpm,enum xpm_image_type type,xpm_image_t*image){assert(xpm==(xpm_map_t)picture&&type==XPM_8_8_8_8);if(fail)return NULL;uint32_t*p=malloc(6*sizeof*p);for(unsigned i=0;i<6;i++)p[i]=i%2?0:0xffffff;*image=(xpm_image_t){.width=3,.height=2,.bytes=(uint8_t*)p,.size=24};return (uint8_t*)p;}
int main(void){uint8_t low,high;assert(util_get_LSB(0x1234,&low)==0&&low==0x34&&util_get_MSB(0x1234,&high)==0&&high==0x12);Sprite*s=create_sprite((xpm_map_t)picture);assert(s&&s->width==3&&s->height==2&&s->colors[0]==0xffffff&&s->colors[1]==0);destroy_sprite(s);fail=1;assert(create_sprite((xpm_map_t)picture)==NULL);destroy_sprite(NULL);return 0;}
''')
    def test_violetchi_actual_scene_definition_matches_caller_and_dispatches_rendering(self):
        text=((COMMUNITY/'caroucintra--FEUP-LCOM'/'proj/src')/'utils/helpers.c').read_text();entry=text[text.index('int start_scene('):text.index('void mouse_handle_pos(',text.index('int start_scene('))]
        with tempfile.TemporaryDirectory(prefix='violetchi-scene-') as temp:
            project=Path(temp);(project/'entry.c').write_text('#include "utils/helpers.h"\nextern Game game;\n'+entry)
            self.compile_and_run(project,['entry.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "utils/helpers.h"
Game game;static unsigned bg,clock_draws,sprites,mice,menus,ends;static Sprite pet;static int fail;
int draw_bg(void){bg++;return 0;}int draw_clock(void){clock_draws++;return 0;}int draw_sprite(Sprite*s){assert(s==&pet);sprites++;return 0;}int draw_mouse(void){mice++;return 0;}int draw_menu(void){menus++;return fail;}int draw_game_over(void){ends++;return 0;}
int main(void){game.state=END;Game scene={.state=MENU};assert(start_scene(scene)==0&&menus==1&&!ends);scene.state=GAME;scene.violetchi.sprite=&pet;assert(start_scene(scene)==0&&bg==1&&clock_draws==1&&sprites==1&&mice==1);scene.state=END;assert(start_scene(scene)==0&&ends==1);scene.state=MENU;fail=1;assert(start_scene(scene)==1&&menus==2);return 0;}
''',include_dirs=[COMMUNITY/'caroucintra--FEUP-LCOM'/'proj/src'])
    def test_claustro_device_subscriptions_report_zero_for_success_and_keep_errors(self):
        self.compile_and_run((COMMUNITY/'Wetsao--LCOM'/'proj/src'),['timer.c','kbd.c','rtc.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
int kbd_subscribe_int(uint8_t*);int rtc_subscribe_int(uint8_t*);
static unsigned calls;static int fail;
int sys_irqsetpolicy(int irq,int policy,int*hook){assert(irq==0||irq==1||irq==8);assert(*hook==irq);calls++;return fail;}
int main(void){int(*subscribe[])(uint8_t*)={timer_subscribe_int,kbd_subscribe_int,rtc_subscribe_int};unsigned irqs[]={0,1,8};int result[3];for(unsigned i=0;i<3;i++){uint8_t mask=255;result[i]=subscribe[i](&mask);assert(mask==irqs[i]);}printf("successful policy return statuses: %d %d %d\n",result[0],result[1],result[2]);assert(result[0]==0&&result[1]==0&&result[2]==0&&"successful device setup must return zero so the original caller enters gameplay");assert(calls==3);fail=1;for(unsigned i=0;i<3;i++){uint8_t mask=255;assert(subscribe[i](&mask)!=0&&mask==255);assert(subscribe[i](NULL)!=0);}assert(calls==6);return 0;}
''')

    def test_shark_patrol_video_test_failures_return_null(self):
        self.compile_and_run(COMMUNITY/'joaodealmeida--LCOM/code',['graphics.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "graphics.h"
#include "Utilities.h"
#include "video_gr.h"
#include "timer.h"
#include <assert.h>
static int scenario,initialized,closed;
void resetIrqSet(void){}int timer_subscribe_int(void){return scenario?0:-1;}void addIrq(unsigned short irq){assert(irq==0);}
void*vg_init(unsigned short mode){assert(mode==0x105);initialized++;return &initialized;}int vg_exit(void){closed++;return 0;}
void timer_setCounter(unsigned long seconds){assert(seconds==1);}unsigned long timer_getCounter(void){return 0;}
void timer_int_handler(unsigned int mask){abort();}void interruptHandler(void){abort();}void initBuffer(void){}
int timer_unsubscribe_int(void){return 1;}
int main(void){assert(test_init(0x105,1)==NULL&&initialized==0&&closed==0);scenario=1;assert(test_init(0x105,1)==NULL&&initialized==1&&closed==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu89','-Wno-implicit-function-declaration','-Werror=int-conversion'])

    def test_shark_patrol_entry_uses_declared_graphics_and_buffer_owners(self):
        self.compile_and_run(COMMUNITY/'joaodealmeida--LCOM/code',['proj.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "GameController.h"
#include "graphics.h"
#include "video_gr.h"
#include "buffer.h"
#include <assert.h>
int shark_entry(int argc,char**argv);
static GameController game;static int phase;static unsigned char pixels[1024*768];
void sef_startup(void){}void initDevices(void){assert(phase++==0);}char gameSeedInit(void){assert(phase++==1);return 0;}
void*vg_init(unsigned short mode){assert(phase++==2&&mode==0x105);return pixels;}
void set_vramAdress(void*p){assert(phase++==3&&p==pixels);}void initBuffer(void){assert(phase++==4);}
GameController*initGameController(void){assert(phase++==5);game.state=GAME_QUIT;return &game;}
void updateGame(void){abort();}void drawGame(void){abort();}void stopGame(GameController*g){assert(phase++==6&&g==&game);}
void freeBuffer(void){assert(phase++==7);}int vg_exit(void){assert(phase++==8);return 0;}void stopDevices(void){assert(phase++==9);}
void multiplayer_defineUserType(unsigned char type,GameController**g){abort();}
int main(void){char*argv[]={"./proj",NULL};assert(shark_entry(1,argv)==0&&phase==10);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu89','-Dmain=shark_entry','-Werror=implicit-function-declaration'])

    def test_shark_patrol_timer_receive_failure_returns_failure_status(self):
        self.compile_and_run(COMMUNITY/'joaodealmeida--LCOM/code',['timer.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "timer.h"
#include <assert.h>
static int policies,receives;
int sys_irqsetpolicy(int irq,int flags,int*hook){assert(irq==0&&flags==IRQ_REENABLE);*hook=7;policies++;return 0;}
int sys_irqdisable(int*hook){assert(*hook==7);return 0;}
int driver_receive(endpoint_t from,message*msg,int*status){assert(from==ANY);receives++;return 5;}
int main(void){assert(timer_test_int(1)==1);assert(policies==1&&receives==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu89','-Wno-implicit-function-declaration','-Werror=return-type'])

    def test_star_fleet_creates_one_distinct_bullet_per_slot_and_bounds_capacity(self):
     self.compile_and_run(COMMUNITY/'matilde-silva-21--LCOM/proj/src',['shipBullet.c'],r'''#include <lcom/lcf.h>
   #include <assert.h>
   #include "shipBullet.h"
   static unsigned allocations;void*regression_alloc(size_t n){allocations++;return malloc(n);}
   int main(void){initShipBullets();xpm_image_t image={0};for(unsigned shot=0;shot<7;shot++){createShipBullet(100+shot,40,4,image);unsigned occupied=0;for(unsigned i=0;i<7;i++){if(shipBullets[i]){occupied++;for(unsigned j=0;j<i;j++)assert(shipBullets[j]!=shipBullets[i]&&"one shot must own exactly one slot, not seven aliases");}}assert(occupied==shot+1&&allocations==shot+1);}createShipBullet(200,40,4,image);assert(allocations==7&&"full seven-bullet capacity must not allocate an unowned shot");updateShipBulletPosition();for(unsigned i=0;i<7;i++)assert(shipBullets[i]->y==36&&"each shot advances exactly once per timer frame");return 0;}
   ''',source_flags=['-Dmalloc=regression_alloc'])
    def test_star_fleet_retires_ceiling_and_collision_bullets_before_free(self):
     self.compile_and_run(COMMUNITY/'matilde-silva-21--LCOM/proj/src',['shipBullet.c'],r'''#include <lcom/lcf.h>
   #include <assert.h>
   #include "shipBullet.h"
   static unsigned frees,scores;
   void regression_free(void*p){if(!p)return;for(unsigned i=0;i<7;i++)assert(shipBullets[i]!=p&&"a freed projectile must be removed from the owning table before later timer/collision scans");frees++;free(p);}
   void increaseScore(int value,Ship*s){s->score+=value;scores++;}
   int main(void){initShipBullets();shipBullets[3]=calloc(1,sizeof(ShipBullet));shipBullets[3]->speed=4;shipBullets[3]->y=0;updateShipBulletPosition();assert(!shipBullets[3]&&frees==1);updateShipBulletPosition();assert(frees==1);shipBullets[5]=calloc(1,sizeof(ShipBullet));shipBullets[5]->x=15;shipBullets[5]->y=15;Alien a={.x=10,.y=10,.width=20,.height=20,.alive=true,.value=10};Ship s={0};int kills=0;verifyAlienAndBulletCollision(&a,&kills,&s);assert(!a.alive&&kills==1&&s.score==10&&scores==1&&!shipBullets[5]&&frees==2);verifyAlienAndBulletCollision(&a,&kills,&s);updateShipBulletPosition();assert(kills==1&&scores==1&&frees==2);return 0;}
   ''',source_flags=['-Dfree=regression_free'])
    def test_star_fleet_shutdown_visits_only_the_seven_owned_bullet_slots(self):
     self.compile_and_run(COMMUNITY/'matilde-silva-21--LCOM/proj/src',['shipBullet.c'],r'''#include <lcom/lcf.h>
   #include <assert.h>
   #include "shipBullet.h"
   static unsigned frees;void regression_free(void*p){if(p){for(unsigned i=0;i<7;i++)assert(shipBullets[i]!=p);frees++;free(p);}}
   int main(void){initShipBullets();removeAllShipBullets();assert(frees==0);for(unsigned i=0;i<7;i++)shipBullets[i]=calloc(1,sizeof(ShipBullet));removeAllShipBullets();assert(frees==7);for(unsigned i=0;i<7;i++)assert(!shipBullets[i]);removeAllShipBullets();assert(frees==7);return 0;}
   ''',source_flags=['-Dfree=regression_free','-fsanitize=bounds','-fsanitize-undefined-trap-on-error'])

    def test_minix_minesweeper_first_solo_board_has_no_uninitialized_cells(self):
        project=COMMUNITY/'tantch--Minix-MineSweeper/code'
        # Compile the exact entry/clear_fields closure. Unrelated event loops are
        # driver mocks here so the assertion observes the board before first input.
        entry=(project/'game.c').read_text().split('void set_fields(',1)[0]
        with tempfile.TemporaryDirectory(prefix='minesweeper-entry-') as temporary:
            folder=Path(temporary);(folder/'entry.c').write_text(entry)
            self.compile_and_run(folder,['entry.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
int getLado(void);
#include "game.h"
#include "field.h"
#include "vbe.h"
#include "porta.h"
#include <assert.h>
int minesweeper_entry(void);static int menus,played,side,mines;static unsigned char video;
void setLado(int value){side=value;}int getLado(void){return side;}
void setMines(int value){mines=value;}void setAddr(unsigned long address){assert(address==0x2f8);}
time_t lcom_compat_time(time_t*value){if(value)*value=0;return 0;}
void sef_startup(void){}void*vg_init(unsigned short mode){assert(mode==0x103);return &video;}int vg_exit(void){return 0;}
int subscribe_kbc(void){return BIT(1);}int subscribe_mouse(void){return BIT(12);}int subscribe_timer(void){return BIT(0);}
int unsubscribe_mouse(void){return 0;}int unsubscribe_timer(void){return 0;}
int menu(void){return menus++?4:1;}
int single_play(Campo_t board[][getLado()]){assert(side==11&&mines==15);for(int row=0;row<side;row++)for(int col=0;col<side;col++){assert(board[row][col].flag==0&&"First reveal must not read an indeterminate flag");assert(board[row][col].revealed==0&&"First flag must not read an indeterminate revealed bit");assert(board[row][col].mine==0&&board[row][col].number==0);}played++;return 0;}
int multiplayer(int host,Campo_t board[][getLado()]){abort();}
int getFlags(void){return 0;}int getFlagsCertas(void){return 0;}
int main(void){assert(minesweeper_entry()==0&&menus==2&&played==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmain=minesweeper_entry','-Wno-implicit-function-declaration','-O2','-ftrivial-auto-var-init=pattern'],include_dirs=[project])

    def test_space_patrol_hud_formats_all_int_digits_and_terminators(self):
        self.compile_and_run(COMMUNITY/'MiguelMarinho02--LCOM22-23/proj/src',['lib/draw_elements.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include "lib/draw_elements.h"
static uint8_t glyphs[65536];Sprite font;static char observed[3][32];static unsigned counts[3];
int vg_draw_vline(uint16_t x,uint16_t y,uint16_t len,uint32_t color){return 0;}
int getxRes(void){return 1152;}int getyRes(void){return 864;}
int vg_draw_char(xpm_image_t image,uint16_t x,uint16_t y,uint8_t*p){int field=y==0&&x>=192&&x<700?0:y==2&&x>=1082?1:y==40&&x>=80?2:-1;if(field>=0){ptrdiff_t offset=p-image.bytes-513;assert(offset>=0&&offset%32==0&&offset/32<=9);assert(counts[field]<31);observed[field][counts[field]++]='0'+offset/32;}return 0;}
int main(void){font.map=(xpm_image_t){.bytes=glyphs,.width=512,.height=128,.size=65536};int values[][3]={{0,180,3},{2147483647,100,10},{999,0,0}};for(unsigned n=0;n<3;n++){memset(observed,0,sizeof observed);memset(counts,0,sizeof counts);assert(draw_arena(values[n][0],values[n][1],values[n][2])==0);for(unsigned k=0;k<3;k++){char expected[32];snprintf(expected,sizeof expected,"%d",values[n][k]);assert(!strcmp(observed[k],expected));}}return 0;}

''',source_flags=['-O2','-D_FORTIFY_SOURCE=2','-fstack-protector-all'])
    def test_classic_garden_startup_enables_mouse_reporting_before_game_loop(self):
        self.compile_and_run(COMMUNITY/'NunoFer998--LCOM/proj/src',['proj.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
int subscribe_int(void);static int enabled,fail_enable;
int timer_subscribe_int(uint8_t*mask){*mask=1;return 0;}
int sub_mouse_int(uint8_t*mask){*mask=4;return 0;}
int kb_sub_int(uint8_t*mask){*mask=2;return 0;}
int write_mouse_command(uint8_t command){assert(command==0xf4);enabled++;return fail_enable;}
int main(void){assert(subscribe_int()==0);assert(enabled==1&&"the original game must enable reporting before expecting any mouse packet");fail_enable=1;assert(subscribe_int()!=0&&enabled==2&&"failed device initialization cannot enter a dead game loop");return 0;}
''',source_flags=['-Dmain=student_main','-O2'])

    def test_grass_garden_double_buffer_uses_bytes_instead_of_bits(self):
        self.compile_and_run(COMMUNITY/'tommygomez25--LCOM/proj/src',['video_gr.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "video_gr.h"
static vbe_mode_info_t info;static uint8_t frames[224];static unsigned mapped;uint32_t COUNTER;
void panic(const char*message,...){(void)message;abort();}
void*lm_alloc(size_t size,mmap_t*m){assert(size==sizeof info);m->phys=0x10000;m->virt=&info;m->size=size;return &info;}bool lm_free(const mmap_t*m){return true;}
int (sys_int86)(reg86_t*r){assert(r->ax==0x4f01||r->ax==0x4f02);r->ax=0x004f;return 0;}
int sys_privctl(endpoint_t ep,int req,void*arg){struct minix_mem_range*m=arg;assert(m->mr_base==0xe0000000u&&m->mr_limit-m->mr_base==192&&"two 8x4x24bpp pages need192 bytes, not1536bits");return 0;}
void*vm_map_phys(endpoint_t ep,void*base,size_t size){assert(size==96&&(uintptr_t)base==0xe0000000u+mapped*96);return frames+96*mapped++;}
int main(void){info=(vbe_mode_info_t){.XResolution=8,.YResolution=4,.BitsPerPixel=24,.BytesPerScanLine=24,.PhysBasePtr=0xe0000000u};memset(frames,0x7b,sizeof frames);vg_init(0x115);assert(mapped==2);memset(frames+96,0x42,96);double_buffer();for(unsigned i=0;i<192;i++)assert(frames[i]==0x42);for(unsigned i=192;i<224;i++)assert(frames[i]==0x7b);swap_buffer();for(unsigned i=0;i<96;i++)assert(frames[i]==0x42&&frames[i+96]==0);for(unsigned i=192;i<224;i++)assert(frames[i]==0x7b);return 0;}
''')

    def test_power_court_timer_retains_frame_storage_until_game_shutdown(self):
        self.compile_and_run(COMMUNITY/'tomaspm98--LCOM/proj/src',['interrupts.c','graphics/video_gr.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "interrupts.h"
extern uint8_t*imBuffer,*vram;extern vbe_mode_info_t vbe;extern int bytes;
uint8_t scancode;int timer_irq_counter;bool powerup,catched_left=true,catched_right;int powerup_number,piece_1_x,piece_1_y=200,piece_2_x,piece_2_y=200,goal_left,goal_right;
static unsigned notifications,frees;static bool stopped;static uint8_t front[800*600*3];
void regression_free(void*p){assert(stopped&&"timer redraw must not free the frame allocation still used by the next frame");assert(p==imBuffer);frees++;free(p);}
int driver_receive(endpoint_t src,message*m,int*status){assert(notifications<3);if(notifications>=1){assert(front[(300*800+300+notifications)*3]==0x33);assert(imBuffer[(300*800+300+notifications)*3]==0);}memset(m,0,sizeof*m);m->m_source=HARDWARE;m->NOTIFY_ARG=notifications++<2?BIT(0):BIT(1);*status=NOTIFY;return 0;}
void kbc_ih(void){scancode=0x81;}void timer_int_handler(void){timer_irq_counter++;}
int score(void){return 0;}int move_piece1_up(void){return 0;}int move_piece1_down(void){return 0;}int move_piece2_up(void){return 0;}int move_piece2_down(void){return 0;}int draw_piece_1(void){return 0;}int draw_piece_2(void){return 0;}int ball_movement(void){return 0;}int draw_powerup(void){return 0;}int catch_powerup(void){return 0;}
int draw_ball(void){return vg_drawpixel(300+notifications,300,0x112233);}
int main(void){vbe=(vbe_mode_info_t){.XResolution=800,.YResolution=600,.BitsPerPixel=24};bytes=3;vram=front;allocate_img_buffer();uint8_t*owned=imBuffer;memset(imBuffer,0,sizeof front);assert(interrupts()==0&&notifications==3&&imBuffer==owned&&frees==0);stopped=true;free_im_buffer();assert(frees==1);return 0;}
''',source_flags=['-Dfree=regression_free'])

    def test_space_patrol_keyboard_header_matches_its_actual_byte_getter(self):
        self.compile_and_run(COMMUNITY/'MiguelMarinho02--LCOM22-23/proj/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "lib/keyboard.h"
#include "lib/keyboard.c"
int main(void){uint8_t values[]={0,1,127,128,255};for(unsigned i=0;i<sizeof values;i++){setData(values[i]);assert(getData()==values[i]);}return 0;}
''')

    def test_classic_garden_sidebar_click_cannot_deploy_or_index_the_board(self):
        self.compile_and_run(COMMUNITY/'NunoFer998--LCOM/proj/src',['Controller/MouseController/mouseController.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "Controller/MouseController/mouseController.h"
bool firstMove=true,startedExplosionTimer;uint8_t flags=35;static unsigned deployments,digs;
void deployBombs(uint8_t x,uint8_t y){assert(x<15&&y<15&&"sidebar is outside the playable board");deployments++;}void dig(uint8_t x,uint8_t y){assert(x<15&&y<15);digs++;}
void DrawMine(uint8_t x,uint8_t y){}void drawFlag(uint8_t x,uint8_t y){}int drawRectangleToBackground(uint32_t c,uint16_t x,uint16_t y,uint16_t w,uint16_t h){return 0;}
int main(void){Mouse mouse={0};mouse.pos.x=600;mouse.pos.y=100;handleLeftClick(&mouse);assert(firstMove&&deployments==0&&digs==0);mouse.pos.x=40;mouse.pos.y=40;handleLeftClick(&mouse);assert(!firstMove&&deployments==1&&digs==1);mouse.pos.x=799;handleLeftClick(&mouse);assert(!startedExplosionTimer&&deployments==1&&digs==1);mouse.pos.x=599;mouse.pos.y=599;handleRightClick(&mouse);assert(flags==34&&gameBoard[14][14].isFlagged);mouse.pos.y=600;handleRightClick(&mouse);assert(flags==34&&gameBoard[14][14].isFlagged);return 0;}
''')



    def test_classic_garden_dispatches_the_current_keyboard_byte_before_menu_selection(self):
        self.compile_and_run(COMMUNITY/'NunoFer998--LCOM/proj/src',['proj.c','Controller/MainMenuController/mainMenuController.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <setjmp.h>
#include "Controller/MainMenuController/mainMenuController.h"
extern game_state state;extern bool stateChanged;extern uint8_t keyboard_mask,time_mask,mouse_mask;int handle_interrupt_loop(void);
bool packet_complete,startedExplosionTimer;int game_counter;static jmp_buf done;static unsigned events,reads;
int driver_receive(endpoint_t src,message*m,int*status){if(events++)longjmp(done,1);memset(m,0,sizeof*m);m->m_source=HARDWARE;m->NOTIFY_ARG=2;*status=NOTIFY;return 0;}
void kbc_ih(void){reads++;scan_code=0x1c;}
/* Only a keyboard notification is delivered. Any rendering/device call is unexpected. */
void DrawFlagCounter(void){abort();}
void DrawTime(void){abort();}
void buildGameBoard(void){abort();}
void createInstructionScreen(void){abort();}
void drawGameBoardToBackground(void){abort();}
void drawGameSideBar(void){abort();}
void drawMouse(void){abort();}
void drawSMine(void){abort();}
int draw_background(void){abort();}
int flip_page(void){abort();}
void handlePacket(void){abort();}
void handleScanCodeGameOver(void){abort();}
void loadGameOverSprites(void){abort();}
void loadGameSprites(void){abort();}
void loadMainMenuSprites(void){abort();}
void mouse_ih(void){abort();}
void setGameOverBackgorund(void){abort();}
void setInstructionBackgorund(void){abort();}
void setMainMenuBackgorund(void){abort();}
void timer_int_handler(void){abort();}
void setSMinePosition(SMine*m,uint16_t x,uint16_t y){abort();}

int main(void){keyboard_mask=2;time_mask=1;mouse_mask=4;scan_code=0;state=MAIN_MEU;stateChanged=false;if(!setjmp(done))handle_interrupt_loop();assert(reads==1);assert(state==GAME&&stateChanged&&"a menu must process the byte delivered by this notification, not the previous byte");return 0;}
''',source_flags=['-Dmain=student_main','-O2'])


    def test_classic_garden_device_handlers_leave_other_device_bytes_queued(self):
        self.compile_and_run(COMMUNITY/'NunoFer998--LCOM/proj/src',['Controller/Devices/Mouse_Keyboard/Mouse/mouse.c','Controller/Devices/Mouse_Keyboard/Keyboard/keyboard.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "Controller/Devices/Mouse_Keyboard/Mouse/mouse.h"
#include "Controller/Devices/Mouse_Keyboard/Keyboard/keyboard.h"
extern int c;static bool mouse_data;static unsigned reads;static uint8_t value;
int aux_sys_inb(int port,uint8_t*data){if(port==0x64){*data=1|(mouse_data?32:0);return 0;}assert(port==0x60);reads++;*data=value;return 0;}
int tickdelay(clock_t ticks){return 0;}uint32_t micros_to_ticks(uint32_t micros){return 1;}
int main(void){mouse_data=false;value=0x1c;c=1;assert(read_packet_byte()!=0);assert(reads==0&&"mouse IRQ handler must not consume a pending keyboard byte");mouse_data=true;value=8;assert(read_packet_byte()==0&&reads==1&&p.bytes[0]==8);reads=0;value=0x28;assert(get_scancode()!=0);assert(reads==0&&"keyboard IRQ handler must not consume a pending mouse header");mouse_data=false;value=0x1c;assert(get_scancode()==0&&reads==1&&scan_code==0x1c);return 0;}
''')

    def test_three_hunters_byte_port_read_preserves_the_callers_adjacent_bytes(self):
        self.compile_and_run(COMMUNITY/'sirkotal--feup-lcom/proj/src',['devices/utils.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
static int failure;
int (sys_inb)(int port,uint32_t*value){assert(port==0x60);if(failure)return 5;*value=0xfa;return 0;}
int main(void){struct{uint8_t before,value,after[5];}guard;memset(&guard,0x7b,sizeof guard);assert(util_sys_inb(0x60,&guard.value)==0&&guard.value==0xfa);assert(guard.before==0x7b);for(unsigned i=0;i<5;i++)assert(guard.after[i]==0x7b&&"a byte read must not let sys_inb write32bits into a one-byte caller");failure=1;guard.value=0x42;assert(util_sys_inb(0x60,&guard.value)==5&&guard.value==0x42);return 0;}
''',source_flags=['-O2','-fstack-protector-all'])

    def test_grass_garden_direction_make_survives_release_before_the_four_tick_update(self):
        self.compile_and_run(COMMUNITY/'tommygomez25--LCOM/proj/src',['game.c','snake.c','keyboard.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "game.h"
#include "snake.h"
extern uint8_t last;extern snakepart*snake;extern int size;apple*apple1;uint32_t COUNTER;uint8_t rtc_time[3];static uint8_t port_byte;static uint8_t pixels[1200];
int util_sys_inb(int port,uint8_t*value){*value=port==0x64?1:port_byte;return 0;}
int kbd_print_scancode(bool make,uint8_t size,uint8_t*bytes){return 0;}
uint8_t*xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*image){*image=(xpm_image_t){.width=20,.height=20,.bytes=pixels,.size=sizeof pixels,.type=type};return pixels;}
void draw_xpm(uint8_t*map,xpm_image_t*image,uint16_t x,uint16_t y){}void draw_grass(void){}void draw_apple(void){}void move_apple(void){}void loadLostMenu(void){abort();}
int main(void){apple food={.x=600,.y=500};apple1=&food;for(unsigned phase=0;phase<4;phase++){create_snake();last=RIGHT;for(COUNTER=1;COUNTER<=phase;COUNTER++)PlayInterruptHandler(TIMER);port_byte=DOWN;kbc_ih();PlayInterruptHandler(KEYBOARD);port_byte=DOWN|0x80;kbc_ih();PlayInterruptHandler(KEYBOARD);assert(last==(DOWN|0x80)&&"raw releases remain available to the original name entry");for(COUNTER=phase+1;COUNTER<=4;COUNTER++)PlayInterruptHandler(TIMER);assert(snake[size-1].x==400&&snake[size-1].y==320&&"a valid direction make must survive its release until the next authored movement tick");port_byte=UP;kbc_ih();PlayInterruptHandler(KEYBOARD);port_byte=UP|0x80;kbc_ih();PlayInterruptHandler(KEYBOARD);COUNTER=8;PlayInterruptHandler(TIMER);assert(snake[size-1].x==400&&snake[size-1].y==340&&"immediate reversal remains rejected");free(snake);}return 0;}
''',source_flags=['-Dmain=student_main','-O2'])

    def test_three_hunters_cursor_motion_restores_original_wall_pixels(self):
        import os,subprocess,tempfile
        cli=Path(os.environ.get('LCOM_CLI',str(ROOT/'build/machinelab')))
        with tempfile.TemporaryDirectory(prefix='three-hunters-render-')as temp:
            folder=Path(temp);project=COMMUNITY/'sirkotal--feup-lcom/proj/src'
            build=subprocess.run([str(cli),'compat','build',str(project),'--output',str(folder/'native')],capture_output=True,text=True,timeout=120);self.assertEqual(build.returncode,0,build.stdout+build.stderr)
            script=folder/'input.script';script.write_text('at 20 key ENTER down\nat 24 key ENTER up\nat 40 key RIGHT down\nat 44 key RIGHT up\nat 70 mouse 120 -20 0\nat 80 mouse 120 -20 0\nat 90 mouse 120 -20 0\nat 100 mouse 120 -20 0\nat 110 mouse 20 -10 0\nat 130 mouse 100 -100 0\n');frame=folder/'frame.ppm';result=subprocess.run([str(cli),'run','--headless','--script',str(script),'--max-ticks','180','--timeout','20','--dump-presented','--dump-frame',str(frame),'--',str(folder/'native/proj')],capture_output=True,text=True,timeout=30);self.assertNotIn('terminated by signal',result.stdout+result.stderr);self.assertTrue(frame.exists(),result.stdout+result.stderr);header,dimensions,maximum,pixels=frame.read_bytes().split(b'\n',3);self.assertEqual((header,dimensions,maximum),(b'P6',b'1024 768',b'255'))
            for y in range(90,102):
                for x in range(500,512):self.assertEqual(pixels[(y*1024+x)*3:(y*1024+x)*3+3],b'\xff\xff\xff','Original wall must survive ordinary crosshair movement without black erase holes')


    def test_minix_minesweeper_menu_retains_keyboard_prefix_across_notifications(self):
        self.compile_and_run(COMMUNITY/'tantch--Minix-MineSweeper/code',['game.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
int menu(void);extern int irq_set_kbc,irq_set_mouse,irq_set_timer;
typedef struct {unsigned bits;unsigned char code;} Event;
static const Event*events;static size_t length,position;static Event current;
static int selected;
void draw_menu(int option){assert(option>=1&&option<=4);selected=option;}
void update_graf(void){}void set_rtc(void){}int vg_fill(unsigned long color){assert(color==0);return 0;}void rtc_handle(void){}
int driver_receive(endpoint_t from,message*msg,int*status){assert(position<length);current=events[position++];msg->m_source=HARDWARE;msg->NOTIFY_ARG=current.bits;*status=IPC_STATUS_NOTIFY;return 0;}
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){assert(port==0x60&&current.bits==irq_set_kbc);*value=current.code;return 0;}
int mouse_handle(void){assert(current.bits==irq_set_mouse);return 0;}
static void choose(const Event*input,size_t count,int choice){events=input;length=count;position=0;assert(menu()==choice);assert(position==count&&selected==choice);}
int main(void){
 irq_set_mouse=BIT(12);irq_set_kbc=BIT(1);irq_set_timer=BIT(0);
 const Event enter[]={{BIT(1),0x1c},{BIT(1),0x9c}};choose(enter,sizeof enter/sizeof *enter,1);
 const Event down[]={{BIT(1),0xe0},{BIT(0),0},{BIT(1),0x50},{BIT(1),0xe0},{BIT(1),0xd0},{BIT(1),0x9c}};choose(down,sizeof down/sizeof *down,2);
 const Event down_up[]={{BIT(1),0xe0},{BIT(1),0x50},{BIT(1),0xe0},{BIT(1),0xd0},{BIT(1),0xe0},{BIT(12),0},{BIT(0),0},{BIT(1),0x48},{BIT(1),0xe0},{BIT(1),0xc8},{BIT(1),0x9c}};choose(down_up,sizeof down_up/sizeof *down_up,1);
 const Event escape[]={{BIT(1),0x81}};events=escape;length=1;position=0;assert(menu()==4&&position==1);
 return 0;
}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmain=minesweeper_entry','-Wno-implicit-function-declaration','-O2','-ftrivial-auto-var-init=pattern'])

    def test_henri_glyph_cleanup_releases_exactly_ten_owned_bitmap_pairs(self):
        self.compile_and_run(COMMUNITY/'HenriJSantos--LCOM/proj/src',['bmp.c'],r'''#include <lcom/lcf.h>
#include "bmp.h"
#include "text.h"
#include <assert.h>
static void *owned[20];static bool released[20];static unsigned loaded,releases;
Bitmap*mock_loadBitmap(const char*file){assert(loaded<10);Bitmap*b=calloc(1,sizeof*b);assert(b);b->bitmapData=calloc(4,sizeof(uint32_t));assert(b->bitmapData);owned[2*loaded]=b;owned[2*loaded+1]=b->bitmapData;loaded++;return b;}
void checked_free(void*p){unsigned i=0;for(;i<20;i++)if(owned[i]==p)break;assert(i<20&&"Glyph cleanup must not free an eleventh array slot");assert(!released[i]&&"Glyph allocation released twice");released[i]=true;releases++;free(p);}
#define loadBitmap mock_loadBitmap
#define free checked_free
#include "text.c"
#undef free
#undef loadBitmap
int main(void){init_numbers();assert(loaded==10);free_numbers();assert(releases==20&&"Each glyph owns both pixels and metadata");for(unsigned i=0;i<20;i++)assert(released[i]);return 0;}
''',source_flags=['-Dfree=checked_free','-O2'])

    def test_every_one_based_animation_frame_reads_its_own_sheet_cell(self):
        self.compile_and_run(COMMUNITY/'m21ark--lcom/proj/src',['game/views/animation/animation.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
#include "game/views/animation/animation.h"
static unsigned frame,writes;static uint32_t*sheet;
int fill_pixel(uint16_t x,uint16_t y,uint32_t color){assert(x>=100&&x<112&&y>=204&&y<209);unsigned row=(frame-1)/6,col=(frame-1)%6;uint32_t expected=sheet[(row*12+y-200)*72+col*12+x-100];assert(color==expected&&"every frame must select the matching zero-based sheet column");writes++;return 0;}
int main(void){long page=sysconf(_SC_PAGESIZE);size_t bytes=72*24*sizeof(*sheet),span=(bytes+page-1)/page*page;void*pages=mmap(NULL,span+2*page,PROT_NONE,MAP_PRIVATE|MAP_ANON,-1,0);assert(pages!=MAP_FAILED);assert(mprotect((char*)pages+page,span,PROT_READ|PROT_WRITE)==0);sheet=(uint32_t*)((char*)pages+page+span-bytes);for(unsigned y=0;y<24;y++)for(unsigned x=0;x<72;x++)sheet[y*72+x]=1+y*72+x;sprite_t sprite={.w=72,.h=24,.map=(uint8_t*)sheet};AnimSprite*animation=create_animSprite(&sprite,12,6,12,12);assert(animation);for(frame=1;frame<=12;frame++){writes=0;assert(draw_animSprite(animation,frame,100,200)==EXIT_SUCCESS);assert(writes==60);}free(animation);munmap(pages,span+2*page);return 0;}
''',source_flags=['-O2'])


    def test_small_font_caller_uses_one_based_frames_with_rectangular_glyphs(self):
        self.compile_and_run(COMMUNITY/'m21ark--lcom/proj/src',['game/views/animation/animation.c','game/views/font/font.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "game/views/font/font.h"
static uint32_t sheet[176*162];static sprite_t sprite={.w=176,.h=162,.map=(uint8_t*)sheet};static unsigned writes;static char text[]=" 0?Po";
sprite_t*make_sprite(const xpm_map_t map,enum xpm_image_type type){assert(type==XPM_8_8_8_8);return &sprite;}
void set_sprite_X(sprite_t*s,unsigned x){abort();}void set_sprite_Y(sprite_t*s,unsigned y){abort();}int draw_text_sprite(sprite_t*s,int color){abort();}void free_sprite(sprite_t*s){abort();}
int fill_pixel(uint16_t x,uint16_t y,uint32_t color){unsigned character=writes/220,index=text[character]-32,row=index/16,col=index%16;assert(character<sizeof(text)-1);assert(x>=100+character*10&&x<111+character*10&&y>=204&&y<224);unsigned sx=col*11+x-100-character*10,sy=row*27+y-200;assert(color==sheet[sy*176+sx]&&"font character must use its own zero-based ASCII sheet cell");writes++;return 0;}
int main(void){for(unsigned i=0;i<176*162;i++)sheet[i]=i+1;draw_text(text,100,200,0xffffff,true);assert(writes==(sizeof(text)-1)*220);return 0;}
''',source_flags=['-O2'])


    def test_minix_minesweeper_first_complete_mouse_packet_is_applied(self):
        self.compile_and_run(COMMUNITY/'tantch--Minix-MineSweeper/code',['game.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
int getLado(void);
#include "field.h"
#include "game.h"
#include <assert.h>
extern int irq_set_mouse,irq_set_kbc,irq_set_timer;
static int notifications,bytes,applied,resets;
static Campo_t board[3][3];
int getLado(void){return 3;}
int vg_fill(unsigned long color){assert(color==0);return 0;}
void draw_grid_easy(int first,Campo_t field[][getLado()]){assert(first==0&&field==board);}
void draw_mouse(int x,int y,unsigned long color){assert(x==100&&y==100&&color==63);}
void update_graf(void){}int getMousex(void){return 100;}int getMousey(void){return 100;}
void mouse_start(void){}
int driver_receive(endpoint_t from,message*msg,int*status){assert(notifications<4);msg->m_source=HARDWARE;msg->NOTIFY_ARG=notifications++<3?irq_set_mouse:irq_set_kbc;*status=IPC_STATUS_NOTIFY;return 0;}
int mouse_handle(void){return ++bytes;}
void mouse_int(Campo_t field[][getLado()],int me,int multi){assert(field==board&&bytes==3&&me==1&&multi==0);applied++;}
void IncSeg(void){abort();}int GetSeg(void){abort();}
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){assert(port==0x60);*value=0x81;return 0;}
void reset(Campo_t field[][getLado()]){assert(field==board);resets++;}
int main(void){irq_set_mouse=BIT(12);irq_set_kbc=BIT(1);irq_set_timer=BIT(0);assert(single_play(board)==0);assert(notifications==4&&bytes==3&&resets==1);assert(applied==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmain=minesweeper_entry','-Wno-implicit-function-declaration'])

    def test_minix_minesweeper_corner_neighbor_counts_stay_in_board(self):
        self.compile_and_run(COMMUNITY/'tantch--Minix-MineSweeper/code',['game.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <sys/mman.h>
#include <assert.h>
int getLado(void);
#include "field.h"
#include "game.h"
int getLado(void){return 3;}
int rand(void){return 0;}
int main(void){
  size_t page=(size_t)sysconf(_SC_PAGESIZE);unsigned char*memory=mmap(NULL,page*3,PROT_NONE,MAP_PRIVATE|MAP_ANON,-1,0);assert(memory!=MAP_FAILED&&mprotect(memory+page,page,PROT_READ|PROT_WRITE)==0);
  Campo_t(*board)[3]=(void*)(memory+page);memset(board,0,9*sizeof(Campo_t));set_fields(3,1,1,1,board);
  const int expected[3][3]={{-1,1,0},{1,1,0},{0,0,0}};
  for(int row=0;row<3;row++)for(int col=0;col<3;col++){assert(board[row][col].mine==(row==0&&col==0));assert(board[row][col].number==expected[row][col]);}
  assert(board[1][1].revealed==1);munmap(memory,page*3);return 0;
}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmain=minesweeper_entry','-Wno-implicit-function-declaration'])

    def test_minix_minesweeper_maps_framebuffer_in_bytes(self):
        self.compile_and_run(COMMUNITY/'tantch--Minix-MineSweeper/code',['vbe.c'],r'''#define LCOM_COMPAT_LEGACY 1
#define LCOM_COMPAT_LEGACY_LM_STATUS 1
#include <minix/drivers.h>
int getLado(void);
#include "vbe.h"
#include "lmlib.h"
#include <assert.h>
static vbe_mode_info_t info;
static unsigned char framebuffer[800*600];
int lm_init(void){return 0;}
void*lm_alloc(unsigned long length,mmap_t*m){assert(length==256);m->phys=0x10000;m->virtual=&info;m->size=256;return &info;}
void lcom_compat_legacy_lm_free(mmap_t*map){assert(((mmap_t*)map)->virtual==&info);}
int lcom_compat_legacy_int86(struct reg86u*regs){if(regs->u.w.ax==0x4f01){info.XResolution=800;info.YResolution=600;info.BitsPerPixel=8;info.PhysBasePtr=0xe0000000;}else assert(regs->u.w.ax==0x4f02&&regs->u.w.bx==(0x103|BIT(14)));return 0;}
int sys_privctl(endpoint_t ep,int request,void*arg){struct minix_mem_range*r=arg;assert(ep==SELF&&request==SYS_PRIV_ADD_MEM);assert(r->mr_base==0xe0000000&&r->mr_limit-r->mr_base==sizeof framebuffer);return 0;}
void*vm_map_phys(endpoint_t ep,void*physical,size_t size){assert(ep==SELF&&(uintptr_t)physical==0xe0000000&&size==sizeof framebuffer);return framebuffer;}
void panic(const char*format,...){abort();}
int main(void){vg_init(0x103);vg_set_pixel(0,12);vg_set_pixel(799+800*599,37);assert(framebuffer[0]==12&&framebuffer[sizeof framebuffer-1]==37);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-Wno-implicit-function-declaration'])

    def test_charmander_collision_then_escape_releases_round_once(self):
     self.compile_and_run(COMMUNITY/'HenriJSantos--LCOM/proj/src',['game.c','sprites.c'],r'''#include <lcom/lcf.h>
#include "game.h"
#include "bmp.h"
#include "video.h"
#include "sprites.h"
#include <assert.h>
static void *released[32];static unsigned releases;
void checked_free(void *p){if(!p)return;for(unsigned i=0;i<releases;i++)assert(released[i]!=p && "Round allocation released twice");released[releases++]=p;free(p);}
static Bitmap *pikachu[2];static unsigned loaded,deleted,scored,collisions;static bool hit;
Bitmap*loadBitmap(const char*file){Bitmap*b=calloc(1,sizeof*b);assert(b);b->bitmapInfoHeader.width=64;b->bitmapInfoHeader.height=64;if(strstr(file,"pikachu1"))pikachu[0]=b;if(strstr(file,"pikachu2"))pikachu[1]=b;loaded++;return b;}
void deleteBitmap(Bitmap*b){assert(b);deleted++;free(b);}
void setBackground(Bitmap*b){assert(b);}
int drawBitmap(Bitmap*b,int x,int y,bool check){assert(b);if(hit&&check&&(b==pikachu[0]||b==pikachu[1])){collisions++;return 0xff010203;}return 0;}
void hideBitmap(Bitmap*b,int x,int y){assert(b);}
void updateFrame(void){}
int parse_vbe_mode(uint16_t mode,vbe_mode_info_t*info){assert(mode==0x144);memset(info,0,sizeof*info);info->XResolution=1024;info->YResolution=768;return 0;}
void init_numbers(void){}void free_numbers(void){}void print_int(unsigned n,unsigned x,unsigned y){}void hide_int(unsigned n,unsigned x,unsigned y){}
void print_score(int score){assert(score==0);scored++;}
uint8_t get_hour(void){return 12;}void clear_sp_buffer(void){}void send_sp_message(char*m,uint8_t n){abort();}
int main(void){
    init_project();assert(loaded==12);
    /* Initial512,384crosshair is inside the original1P menu button. */
    event_byte=MOUSE_EVT_BIT;mouse_evt.type=CLICK;event_handler();
    /* An ordinary rendered enemy collision ends and releases the round. */
    hit=true;event_byte=TIMER_EVT_BIT;timer_evt.type=UPDATE_FRAME;event_handler();assert(collisions==1&&scored==1);
    /* Original GameOver Escape invokes cleanup again before project shutdown. */
    event_byte=KB_EVT_BIT;kb_evt.type=ESCAPE;event_handler();assert(scored==1&&deleted==loaded);return 0;
}
''',source_flags=['-Dfree=checked_free'])

    def test_duckhunt_failed_bios_setup_returns_null(self):
        self.compile_and_run(COMMUNITY/'andrebreis--feup-lcom/proj/src',['videoGraphics.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "videoGraphics.h"
#include "vbe.h"
#include <assert.h>
int lcom_compat_legacy_int86(struct reg86u*regs){assert(regs->u.w.ax==0x4f02 && regs->u.w.bx==(0x117|BIT(14)));return 1;}
int vbe_get_mode_info(unsigned short mode,vbe_mode_info_t*info){abort();}
int sys_privctl(endpoint_t ep,int request,void*arg){abort();}
void*vm_map_phys(endpoint_t ep,void*address,size_t length){abort();}
void panic(const char*format,...){abort();}
int main(void){assert(videoGraphicsInit(0x117)==NULL);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Werror=int-conversion'])

    def test_duckhunt_entry_uses_declared_graphics_lifecycle(self):
        self.compile_and_run(COMMUNITY/'andrebreis--feup-lcom/proj/src',['main.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "DuckHunt.h"
#include "Scores.h"
#include "videoGraphics.h"
#include <assert.h>
int duckhunt_entry(void);
static int initialized,closed,menus;
void sef_startup(void){} int sys_enable_iop(endpoint_t ep){return 0;}
int initializeGame(InterruptVariables*iv){assert(iv);return 0;}
void*videoGraphicsInit(unsigned short mode){assert(mode==0x117);initialized++;return &initialized;}
int videoGraphicsExit(void){assert(initialized==1);closed++;return 0;}
int menu(InterruptVariables*iv){assert(initialized==1);menus++;return 2;}
void exitGame(void){assert(menus==1);}
unsigned int playGame(InterruptVariables*iv){abort();}
char*showGameOver(InterruptVariables*iv,int score){abort();}
int scorePlace(int score){abort();} void insertScore(char*name,int score,int line){abort();}
void printHighScores(InterruptVariables*iv){abort();}
int main(void){assert(duckhunt_entry()==0);assert(initialized==1&&menus==1&&closed==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmain=duckhunt_entry','-Werror=implicit-function-declaration'])

    def test_duckhunt_formatted_date_survives_return(self):
        self.compile_and_run(COMMUNITY/'andrebreis--feup-lcom/proj/src',['RTC.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "RTC.h"
#include <assert.h>
int main(void){Date date={.year=2026,.month=10,.day=17,.hour=13,.minute=4};char*text=dateToStr(date);assert(strcmp(text,"13:04 17/10/2026")==0);date.minute=5;assert(strcmp(dateToStr(date),"13:05 17/10/2026")==0);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Werror=return-stack-address','-Werror=format'],compiler_override=['clang'])

    def test_duckhunt_fresh_highscores_creates_empty_readable_table(self):
        self.compile_and_run(COMMUNITY/'andrebreis--feup-lcom/proj/src',['Scores.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "Scores.h"
#include "Mouse.h"
#include <assert.h>
#undef fopen
#undef remove
static Bitmap background;
static int headings,received;
FILE*lcom_compat_fopen(const char*path,const char*mode){assert(strcmp(path,"/home//DuckHuntResources/res/scores.txt")==0);return fopen("scores.txt",mode);}
Bitmap*loadBitmap(const char*path){assert(strstr(path,"highscores.bmp"));return &background;}
void drawBitmap(Bitmap*bmp,int x,int y,Alignment alignment){assert(bmp==&background);}
void drawString(char*text,int x,int y,int spacing){assert(strcmp(text,"NR NAME SCORE HOUR DATE")==0);headings++;}
void drawNumber(unsigned int number,int x,int y,int spacing){abort();}
void flipBuffer(void){assert(headings==1);}
int driver_receive(endpoint_t src,message*msg,int*status){assert(received++==0);msg->m_source=HARDWARE;msg->NOTIFY_ARG=BIT(1);*status=IPC_STATUS_NOTIFY;return 0;}
int kbdReadKey(unsigned char*key){*key=0x81;return 0;}
int getPacket(char*packet){abort();}void updateMousePosition(char packet[3]){abort();}
int main(void){char dir[]="/tmp/lcom-duck-scores-XXXXXX";char*root=mkdtemp(dir);assert(root&&chdir(root)==0);InterruptVariables iv={.keyboardSet=BIT(1)};printHighScores(&iv);assert(headings==1&&received==1);FILE*f=fopen("scores.txt","r");assert(f&&fgetc(f)==EOF);fclose(f);assert(remove("scores.txt")==0&&chdir("/")==0&&rmdir(root)==0);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Wno-implicit-function-declaration'])

    def test_duckhunt_nonletter_scancodes_and_empty_keyboard_are_defined(self):
        self.compile_and_run(COMMUNITY/'andrebreis--feup-lcom/proj/src',['Keyboard.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "Keyboard.h"
#include <assert.h>
static int reads,waits;
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){assert(port==0x64);*value=0;reads++;return 0;}
uint32_t micros_to_ticks(uint32_t micros){assert(micros==20000);return 2;}
int tickdelay(clock_t ticks){assert(ticks==2);waits++;return 0;}
int main(void){
  const unsigned char codes[]={0x90,0x99,0x9e,0xa6,0xac,0xb2};const char letters[]="QPALZM";
  for(unsigned i=0;i<sizeof codes;i++)assert(codeToChar(codes[i])==letters[i]);
  const unsigned char nonletters[]={0,0x1e,0x9c,0x81,0xe0,0xff};
  for(unsigned i=0;i<sizeof nonletters;i++)assert(codeToChar(nonletters[i])==0);
  unsigned char value=0x55;assert(kbdReadKey(&value)==-1&&value==0x55&&reads==10&&waits==10);return 0;
}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Werror=return-type'])

    def test_mouse_startup_commands_do_not_test_indeterminate_ack_state(self):
        cases=[('jfguedes123--Y2S2-LCOM','devices/mouse/mouse.c','devices/mouse/mouse.h','mouse_write_cmd','kbc_write_cmd','kbc_read_out'),('mguerrinha--LCOM','controller/mouse/mouse.c','controller/mouse/mouse.h','write_to_mouse','write_KBC_command','util_sys_inb')]
        for slug,source,header,entry,write,read in cases:
            with self.subTest(game=slug):
                harness=r'''#include <lcom/lcf.h>
#include <assert.h>
#include "$HEADER$"
static unsigned writes,reads;static uint8_t expected;
int $WRITE$(uint8_t port,uint8_t value){assert(port==(writes%2?0x60:0x64));assert(value==(writes%2?expected:0xd4));writes++;return 0;}
int $READ$($PORTTYPE$ port,uint8_t*value){assert(port==0x60&&writes==2);reads++;*value=0xfa;return 0;}
int tickdelay(clock_t ticks){return 0;}uint32_t micros_to_ticks(uint32_t us){return 1;}
int main(void){expected=0xf4;assert($ENTRY$(expected)==0);assert(writes==2&&reads==1&&"first command must be sent before testing any ACK");writes=reads=0;expected=0xf5;assert($ENTRY$(expected)==0);assert(writes==2&&reads==1&&"each command consumes its own ACK");return 0;}
'''
                for name,value in [('HEADER',header),('WRITE',write),('READ',read),('ENTRY',entry),('PORTTYPE','int'if read=='util_sys_inb'else'uint8_t')]:harness=harness.replace('$'+name+'$',value)
                self.compile_and_run(COMMUNITY/slug/'src',[source],harness,source_flags=['-O2'])
    def test_green_field_startup_enables_mouse_reporting_before_game_loop(self):
        self.compile_and_run(COMMUNITY/'Manjeri998--LCOM25/proj/src',['proj.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
int subscribe_int(void);static int enabled,fail_enable;
int timer_subscribe_int(uint8_t*mask){*mask=1;return 0;}
int sub_mouse_int(uint8_t*mask){*mask=4;return 0;}
int kb_sub_int(uint8_t*mask){*mask=2;return 0;}
int write_mouse_command(uint8_t command){assert(command==0xf4);enabled++;return fail_enable;}
int main(void){assert(subscribe_int()==0);assert(enabled==1&&"the original game must enable reporting before expecting any mouse packet");fail_enable=1;assert(subscribe_int()!=0&&enabled==2&&"failed device initialization cannot enter a dead game loop");return 0;}
''',source_flags=['-Dmain=student_main','-O2'])

    def test_alien_missile_at_cursor_edge_clips_without_ending_round(self):
        self.compile_and_run(COMMUNITY/'jpvds1--LCOM/proj/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
#include "Drivers/GraphicCard.c"
#include "Missile.c"
#define FRAME(n) uint16_t*missile##n##_sprite;xpm_image_t missile##n##_img;
FRAME(1) FRAME(2) FRAME(3) FRAME(4) FRAME(5) FRAME(6) FRAME(7) FRAME(8) FRAME(9) FRAME(10) FRAME(11) FRAME(12) FRAME(13) FRAME(14) FRAME(15) FRAME(16) FRAME(17) FRAME(18) FRAME(19) FRAME(20) FRAME(21) FRAME(22) FRAME(23) FRAME(24)
int mouse_x=5,mouse_y=5;
int main(void){size_t page=(size_t)sysconf(_SC_PAGESIZE),size=20*12*2;uint8_t*memory=mmap(NULL,page*2,PROT_READ|PROT_WRITE,MAP_ANON|MAP_PRIVATE,-1,0);assert(memory!=MAP_FAILED&&mprotect(memory+page,page,PROT_NONE)==0);back_buffer=memory+page-size;mode_inf=(vbe_mode_info_t){.XResolution=20,.YResolution=12,.BitsPerPixel=16};memset(back_buffer,0,size);uint16_t art[80*80];for(unsigned i=0;i<6400;i++)art[i]=0x1234;art[36*80+36]=0;missile1_sprite=art;missile1_img=(xpm_image_t){.width=80,.height=80};startup_missile();shoot_missile();assert(missile.shot&&missile.x==5&&missile.y==5);assert(draw_missile(0)==0&&"a normal cursor-edge shot clips instead of aborting gameplay");for(unsigned y=0;y<12;y++)for(unsigned x=0;x<20;x++)assert(((uint16_t*)back_buffer)[y*20+x]==((x==1&&y==1)?0:0x1234));assert(vg_draw_pixel(20,0,0xffff)!=0);assert(vg_draw_pixel(0,12,0xffff)!=0);assert(vg_draw_pixel(19,11,0x4321)==0&&((uint16_t*)back_buffer)[239]==0x4321);munmap(memory,page*2);return 0;}
''')

    def test_tetris_respawn_preserves_the_callers_live_piece(self):
        self.compile_and_run(COMMUNITY/'lindirod--L.EIC-LCOM-2223/proj/src',['game.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "game.h"
GameState game_state=GAME;uint8_t scancode;
static Piece*caller;static Piece replacement;static unsigned deletes,moves;
Piece*new_piece(Board*board){replacement=(Piece){.board=board,.board_index={3,4,5,6},.color=0x123456,.rot=1,.piece_shape=I};return &replacement;}
void delete_piece(Piece*p){assert(p!=caller&&"the main loop retains this piece for subsequent timer and keyboard events");assert(p==&replacement);deletes++;}
bool update_movement(Piece*p, enum movement_type direction){assert(p==caller);if(moves++==0){assert(direction==DOWN&&p->board_index[0]==190);return false;}assert(direction==LEFT&&p->piece_shape==I&&p->board_index[0]==3&&p->color==0x123456);p->board_index[0]=2;return true;}
void draw_board(Board*b){assert(b==caller->board);}int draw_xpm(xpm_image_t*i,uint16_t x,uint16_t y){return 0;}
int main(void){Board board={0};Piece original={.board=&board,.board_index={190,191,192,193},.piece_shape=O,.color=7};caller=&original;play_game(caller);assert(game_state==GAME&&deletes==1&&caller->piece_shape==I&&caller->board==&board);uint8_t key=0x9e;movement_input(&key,caller);assert(moves==2&&caller->board_index[0]==2);return 0;}
''')

    def test_alien_laser_iteration_saves_successor_before_removal(self):
        self.compile_and_run(COMMUNITY/'jpvds1--LCOM/proj/src',['Handlers.c','Lists/LaserList.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "Handlers.h"
struct EnemyNode*EnemyFirst;vbe_mode_info_t mode_inf;struct LaserNode*ll_t;struct EnemyNode*el_t;uint16_t x=800,y=800,lives=3,score;
int current_ebullet_sprite;uint32_t color_laser;uint16_t*eb1_sprite,*eb2_sprite,*eb3_sprite;xpm_image_t eb1_img,eb2_img,eb3_img;
static struct LaserNode*removed;static void*removed_laser;static unsigned frees;
void regression_free(void*p){if(p==removed){memset(p,0xa5,sizeof*removed);frees++;}else if(p==removed_laser){memset(p,0xa5,sizeof(struct Laser));frees++;}else free(p);}
int draw_xpm(uint16_t*s,xpm_image_t i,uint16_t x,uint16_t y){return 0;}int vg_draw_rectangle(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint32_t c){return 0;}
struct AnimationNode*al_getNewNode(uint16_t x,uint16_t y){abort();}void al_addNode(const struct AnimationNode*n){abort();}void el_removeNode(struct EnemyNode*n){abort();}
int main(void){mode_inf.YResolution=100;removed=ll_getNewNode(0,100,0);struct LaserNode*next=ll_getNewNode(0,20,0);removed->next=next;removed_laser=removed->laser;LaserFirst=ll_t=removed;LaserAdded=next;assert(handle_laser()==0);assert(frees==2&&LaserFirst==next&&ll_t==next&&"iteration must never read next from freed laser node");assert(handle_laser()==0&&ll_t==NULL&&LaserFirst==next&&frees==2);free(removed_laser);free(removed);ll_removeFirst();return 0;}
''',source_flags=['-Dfree=regression_free'])

    def test_alien_missile_removes_adjacent_hit_enemies_without_freed_reads(self):
        self.compile_and_run(COMMUNITY/'jpvds1--LCOM/proj/src',['Lists/EnemyList.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "Missile.c"
#define FRAME(n) uint16_t*missile##n##_sprite;xpm_image_t missile##n##_img;
FRAME(1) FRAME(2) FRAME(3) FRAME(4) FRAME(5) FRAME(6) FRAME(7) FRAME(8) FRAME(9) FRAME(10) FRAME(11) FRAME(12) FRAME(13) FRAME(14) FRAME(15) FRAME(16) FRAME(17) FRAME(18) FRAME(19) FRAME(20) FRAME(21) FRAME(22) FRAME(23) FRAME(24)
int mouse_x,mouse_y;uint16_t score;static struct EnemyNode*removed[2];static unsigned freed,animations;
void regression_free(void*p){for(unsigned i=0;i<2;i++){if(p==removed[i]){memset(p,0xa5,sizeof*removed[i]);freed++;return;}if(p==removed[i]->enemy){memset(p,0xa5,sizeof(struct Enemy));freed++;return;}}free(p);}
int draw_xpm(uint16_t*s,xpm_image_t i,uint16_t x,uint16_t y){return 0;}
struct AnimationNode*al_getNewNode(uint16_t x,uint16_t y){static struct AnimationNode n;return &n;}void al_addNode(const struct AnimationNode*n){animations++;}
int main(void){removed[0]=el_getNewNode(180,200,0,0,0,0);removed[1]=el_getNewNode(190,210,0,0,0,0);struct EnemyNode*survivor=el_getNewNode(600,600,0,0,0,0);removed[0]->next=removed[1];removed[1]->next=survivor;EnemyFirst=removed[0];EnemyAdded=survivor;EnemySize=3;missile=(struct Missile){.shot=true,.x=200,.y=200,.frame=11,.frame_time=0};assert(handle_missile()==0);assert(score==40&&animations==2&&freed==4&&EnemyFirst==survivor&&EnemySize==1&&el_q==NULL);return 0;}
''',source_flags=['-Dfree=regression_free'])

    def test_neon_court_ball_rectangle_clips_at_goal_and_floor_edges(self):
        self.compile_and_run(COMMUNITY/'jotac431--FEUP-LCOM/proj/src',['devices/video.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
extern void*video_mem;extern vbe_mode_info_t vmi_p;extern int bytes_per_pixel;
int draw_rectangle(uint16_t,uint16_t,uint16_t,uint16_t,uint32_t);int draw_pixel(uint16_t,uint16_t,uint32_t);
int main(void){size_t page=(size_t)sysconf(_SC_PAGESIZE),size=20*12*4;uint8_t*m=mmap(NULL,page*2,PROT_READ|PROT_WRITE,MAP_ANON|MAP_PRIVATE,-1,0);assert(m!=MAP_FAILED&&mprotect(m+page,page,PROT_NONE)==0);video_mem=m+page-size;vmi_p=(vbe_mode_info_t){.XResolution=20,.YResolution=12,.BitsPerPixel=32};bytes_per_pixel=4;memset(video_mem,0,size);assert(draw_rectangle(18,10,5,5,0x123456)==0);for(unsigned y=0;y<12;y++)for(unsigned x=0;x<20;x++)assert(((uint32_t*)video_mem)[y*20+x]==((x>=18&&y>=10)?0x123456:0));assert(draw_pixel(20,0,1)!=0&&draw_pixel(0,12,1)!=0);assert(draw_rectangle((uint16_t)-2,(uint16_t)-1,4,3,0xabcdef)==0);assert(((uint32_t*)video_mem)[0]==0xabcdef&&((uint32_t*)video_mem)[21]==0xabcdef&&((uint32_t*)video_mem)[22]==0);munmap(m,page*2);return 0;}
''')

    def test_alien_finished_explosion_advances_without_freed_node_read(self):
        self.compile_and_run(COMMUNITY/'jpvds1--LCOM/proj/src',['Handlers.c','Lists/AnimationList.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "Handlers.h"
struct AnimationNode*al_t;static struct AnimationNode*expired;static void*animation;static unsigned freed;
#define FRAME(n) uint16_t*explosion##n##_sprite;xpm_image_t explosion##n##_img;
FRAME(1) FRAME(2) FRAME(3) FRAME(4) FRAME(5) FRAME(6) FRAME(7)
int draw_xpm(uint16_t*s,xpm_image_t i,uint16_t x,uint16_t y){return 0;}
void regression_free(void*p){if(p==expired){memset(p,0xa5,sizeof*expired);freed++;}else if(p==animation){memset(p,0xa5,sizeof(struct Animation));freed++;}else free(p);}
int main(void){expired=al_getNewNode(0,0);struct AnimationNode*next=al_getNewNode(20,20);expired->next=next;animation=expired->animation;expired->animation->curr_xpm=6;expired->animation->time_to_change=1;AnimationFirst=al_t=expired;AnimationAdded=next;assert(handle_animation()==0);assert(freed==2&&AnimationFirst==next&&al_t==next&&"completed explosion must advance before its storage is released");assert(handle_animation()==0&&al_t==NULL);return 0;}
''',source_flags=['-Dfree=regression_free'])

    def test_green_field_sidebar_click_cannot_deploy_or_index_the_board(self):
        self.compile_and_run(COMMUNITY/'Manjeri998--LCOM25/proj/src',['Controller/MouseController/mouseController.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "Controller/MouseController/mouseController.h"
bool firstMove=true,exploded;uint8_t flags=35;static unsigned deployments,digs;
void deployBombs(uint8_t x,uint8_t y){assert(x<15&&y<15&&"sidebar is outside the playable board");deployments++;}void dig(uint8_t x,uint8_t y){assert(x<15&&y<15);digs++;}
void drawFlag(uint8_t x,uint8_t y){}int drawRectangleToBackground(uint32_t c,uint16_t x,uint16_t y,uint16_t w,uint16_t h){return 0;}
int main(void){Mouse mouse={0};mouse.pos.x=600;mouse.pos.y=100;handleLeftClick(&mouse);assert(firstMove&&deployments==0&&digs==0);mouse.pos.x=40;mouse.pos.y=40;handleLeftClick(&mouse);assert(!firstMove&&deployments==1&&digs==1);mouse.pos.x=799;handleLeftClick(&mouse);assert(!exploded&&deployments==1&&digs==1);mouse.pos.x=599;mouse.pos.y=599;handleRightClick(&mouse);assert(flags==34&&gameBoard[14][14].isFlagged);mouse.pos.y=600;handleRightClick(&mouse);assert(flags==34&&gameBoard[14][14].isFlagged);return 0;}
''')


    def test_space_dog_mouse_command_waits_for_clear_input_and_real_ack(self):
     self.compile_and_run(COMMUNITY/'marinuas--FEUP-LCOM/proj/src',['mouse.c'],r'''#include <lcom/lcf.h>
#include "mouse.h"
#include <assert.h>
static unsigned reads,writes,stage,busy,ack_index,delays;static unsigned char command,responses[4];static int pending,stuck,no_ack,write_error;
int util_sys_inb(int port,uint8_t*value){assert(++reads<100);if(port==0x64){if(stuck||busy){if(busy)busy--;*value=0x06;}else *value=pending&&!no_ack?0x25:0x04;return 0;}assert(port==0x60&&pending&&!no_ack);*value=responses[ack_index++];pending=0;return 0;}
int sys_outb(int port,uint32_t value){if(write_error)return EIO;if(port==0x64){assert(stage==0&&value==0xd4&&!busy);stage=1;busy=2;}else{assert(port==0x60&&stage==1&&value==command&&!busy);stage=0;pending=1;writes++;}return 0;}
int tickdelay(clock_t ticks){assert(ticks>0);delays++;return 0;}
uint32_t micros_to_ticks(uint32_t us){assert(us>0);return 1;}
static void reset(unsigned char c){reads=writes=stage=ack_index=delays=0;busy=2;pending=stuck=no_ack=write_error=0;command=c;responses[0]=0xfa;responses[1]=0xfa;}
int main(void){for(unsigned c=0;c<2;c++){reset(c?0xf4:0xea);if(!c)busy=0;assert(mouse_write_command(command)==0&&writes==1&&ack_index==1&&delays>=(c?4:2));}reset(0xf4);responses[0]=0xfe;assert(mouse_write_command(command)==0&&writes==2&&ack_index==2);reset(0xf4);responses[0]=0xfc;assert(mouse_write_command(command)!=0&&writes==1&&ack_index==1);reset(0xf4);stuck=1;assert(mouse_write_command(command)!=0&&writes==0);reset(0xf4);no_ack=1;assert(mouse_write_command(command)!=0&&writes==1&&ack_index==0);reset(0xf4);write_error=1;assert(mouse_write_command(command)!=0&&writes==0);return 0;}
''')

    def test_space_dog_sustained_fire_stays_in_owned_object_tables(self):
     for first_phase in (0,1):
      self.compile_and_run(COMMUNITY/'marinuas--FEUP-LCOM/proj/src',['proj.c'],r'''#include <lcom/lcf.h>
#include "video_gr.h"
#include <assert.h>
int proj_main_loop(int,char**);uint32_t counter;
static unsigned phase,mouse_byte,frames,writes;static bool ack_pending,kbd_pending;static uint8_t keycode;static uint8_t pixel[3];
uint8_t*xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*image){memset(image,0,sizeof*image);image->width=image->height=1;image->size=3;image->bytes=pixel;return pixel;}
void*vg_init(uint16_t mode){assert(mode==0x115);return pixel;}
int vg_exit(void){return 0;}
int vg_draw_xpm_proj(xpm_image_t image,uint16_t x,uint16_t y,uint8_t*map){return 0;}
void change_buffer(void){}
int mouse_print_packet(struct packet*p){return 0;}
int sys_irqsetpolicy(int irq,int policy,int*hook){return 0;}int sys_irqenable(int*hook){return 0;}int sys_irqdisable(int*hook){return 0;}int sys_irqrmpolicy(int*hook){return 0;}
int timer_subscribe_int(uint8_t*bit){*bit=0;return 0;}int timer_unsubscribe_int(void){return 0;}void timer_int_handler(void){counter++;}
int util_sys_inb(int port,uint8_t*value){if(port==0x64){*value=ack_pending?0x25:0x05;return 0;}assert(port==0x60);if(ack_pending){*value=0xfa;ack_pending=0;}else if(kbd_pending){*value=keycode;kbd_pending=0;}else{static const uint8_t packet[]={9,0,0,0x29,0,6};assert(mouse_byte<sizeof packet);*value=packet[mouse_byte++];}return 0;}
int sys_outb(int port,uint32_t value){if(port==0x60){assert(value==0xea||value==0xf4||value==0xf5);ack_pending=1;writes++;}else assert(port==0x64&&value==0xd4);return 0;}
int tickdelay(clock_t ticks){return 0;}uint32_t micros_to_ticks(uint32_t micros){return 1;}
int driver_receive(endpoint_t endpoint,message*m,int*status){assert(phase<72);memset(m,0,sizeof*m);m->m_source=HARDWARE;*status=NOTIFY;if(phase==0)m->NOTIFY_ARG=BIT(0);else if(phase<4)m->NOTIFY_ARG=BIT(12);else if(phase<64){m->NOTIFY_ARG=BIT(0);frames++;if(phase==4){keycode=0x39;kbd_pending=1;m->NOTIFY_ARG|=BIT(1);}}else if(phase==64||phase==65){m->NOTIFY_ARG=BIT(0)|BIT(1);keycode=phase==64?0x81:0x39;kbd_pending=1;}else m->NOTIFY_ARG=BIT(12);phase++;return 0;}
int main(void){phase=$FIRST_PHASE$;assert(proj_main_loop(0,NULL)==0);assert(phase==69&&frames==60&&writes==3);return 0;}
'''.replace('$FIRST_PHASE$',str(first_phase)),source_flags=['-Dmain=space_dog_original_main','-fsanitize=bounds','-fsanitize-undefined-trap-on-error'])

    def test_space_dog_vertical_motion_keeps_the_complete_original_sprite_visible(self):
     self.compile_and_run(COMMUNITY/'marinuas--FEUP-LCOM/proj/src',['moves.c'],r'''#include <lcom/lcf.h>
#include "moves.h"
#include <assert.h>
int dog_init_pos=300;static unsigned draws;
int vg_draw_xpm_proj(xpm_image_t image,uint16_t x,uint16_t y,uint8_t*map){assert(x==40&&image.width==120&&image.height==102&&y>=10&&y+image.height<=600);draws++;return 0;}
int main(void){xpm_image_t image={0};image.width=120;image.height=102;uint8_t pixel[3]={0};for(unsigned i=0;i<10;i++)assert(up(image,pixel)==0);assert(dog_init_pos==10);for(unsigned i=0;i<30;i++)assert(down(image,pixel)==0);assert(dog_init_pos==498&&draws==40);return 0;}
''')
    def test_space_dog_mouse_movement_clamps_after_each_complete_packet(self):
     self.compile_and_run(COMMUNITY/'marinuas--FEUP-LCOM/proj/src',['mouse.c'],r'''#include <lcom/lcf.h>
#include "mouse.h"
#include <assert.h>
extern int mouse_x,mouse_y;
int main(void){enum game_state state=MENU;struct packet p={0};p.delta_x=127;p.delta_y=-127;for(unsigned i=0;i<8;i++)assert(mouse_pos(&p,&state)==0);assert(mouse_x==780&&mouse_y==569&&state==MENU);p.delta_x=-127;p.delta_y=127;for(unsigned i=0;i<8;i++)assert(mouse_pos(&p,&state)==0);assert(mouse_x==0&&mouse_y==0);p.delta_x=100;p.delta_y=-55;for(unsigned i=0;i<4;i++)assert(mouse_pos(&p,&state)==0);p.delta_x=p.delta_y=0;p.lb=1;assert(mouse_pos(&p,&state)==0&&state==PLAY);return 0;}
''')

    def test_avoid_it_shared_keyboard_and_timer_notification_moves_player(self):
     self.compile_and_run(COMMUNITY/'EstevesAndre--LCOM1718/proj/src',['game.c','sprite.c','read_xpm.c','linked_list.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "game.h"
#include "kbd.h"
#include <assert.h>
static Sprite*player;static unsigned notifications;static int observed_x=-1,observed_y=-1;
extern unsigned long global_timer_counter;extern unsigned char scancode;
void*regression_alloc(size_t size){void*p=malloc(size);assert(p);if(!player&&size==sizeof(Sprite))player=p;return p;}
int driver_receive(endpoint_t source,message*m,int*status){assert(notifications<5);if(notifications==1||notifications==2)assert(observed_x==165&&observed_y==488);if(notifications==3||notifications==4)assert(observed_x==175&&observed_y==498);memset(m,0,sizeof*m);m->m_source=HARDWARE;m->NOTIFY_ARG=notifications==0?(BIT(0)|BIT(1)):notifications==3?BIT(0):BIT(1);*status=NOTIFY;notifications++;return 0;}
void kbd_int_handler(void){scancode=notifications==1?0x1e:notifications==2?0x9e:notifications==3?0x20:0x81;}
void timer_int_handler(void){global_timer_counter++;}
void mouse_int_handler(void){abort();}
void display_time(int t){}void display_points(int t,int hit){}void draw_inicial_stuff(int c){}
void copy_to_videomem(void){if(notifications>=1){observed_x=player->x;observed_y=player->y;}}
int vg_pixel_color(unsigned short x,unsigned short y,unsigned short color){return 0;}
void vg_get_color(unsigned short x,unsigned short y,char*pixel){*pixel=0;}
int draw_if_not_player(unsigned short x,unsigned short y,unsigned short color){return 0;}
int main(void){assert(game(0)==0&&notifications==5);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-include','lcom/compat.h','-Dmalloc=regression_alloc'])

    def test_pixel_wings_path_copy_preserves_percent_characters_and_capacity(self):
     self.compile_and_run(COMMUNITY/'DuarteOliveira8--FEUP-LCOM/proj/src',['bitmap.c'],r'''#include "bitmap.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
int main(void){char percent[]="sprites/100%%dragon.bmp";const char*copy=getPath(percent);assert(copy&&copy!=percent&&!strcmp(copy,percent));free((void*)copy);char long_path[600];memset(long_path,'x',sizeof long_path-1);long_path[sizeof long_path-1]=0;copy=getPath(long_path);assert(copy&&copy!=long_path&&!strcmp(copy,long_path));free((void*)copy);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-include','lcom/compat.h'])

    def test_sudoku_clock_redraw_does_not_open_a_cell_editor(self):
        self.compile_and_run(COMMUNITY/'antoniopedrofraga--LCOM-FEUP/proj/code',['jogo.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "jogo.h"
static unsigned char pixels[1024*768*2];static unsigned notifications;
int drawBitmap_byLine(Bitmap*b,int x,int y,Alignment a){return 0;}
int drawBitmap_byPixel(Bitmap*b,int x,int y,Alignment a){return 0;}
int drawBitmap_onBoard(Bitmap*b,int x,int y,Alignment a,unsigned char**buffer){return 0;}
int erase_square(Bitmap*b,int x,int y,Alignment a,unsigned char**buffer,unsigned char*bg){return 0;}
unsigned char*double_buffering(void){return pixels;}
void*vg_adress(void){return pixels;}
short get_h_res(void){return 1024;}short get_v_res(void){return 768;}
void clean_buffer(void){}
int get_day(void){return 1;}int get_month(void){return 1;}int get_year(void){return 17;}int get_hour(void){return 1;}int get_minutes(void){return 1;}
unsigned long get_kb(void){return ESC;}unsigned short get_packets(void){return 0;}
int driver_receive(endpoint_t source,message*m,int*status){assert(!"clock repaint must not request keyboard input");memset(m,0,sizeof*m);m->m_source=HARDWARE;*status=NOTIFY;m->NOTIFY_ARG=notifications++==0?2:1;return 0;}
FILE*lcom_compat_fopen(const char*path,const char*mode){return *mode=='r'?NULL:tmpfile();}
int main(void){Game game={0};Bitmap square={0};square.bitmapInfoHeader.width=square.bitmapInfoHeader.height=38;game.bmps.square=&square;game.mouse.x=100;game.mouse.y=100;game.mouse.lb=1;game.sudoku.board[0][0][0].x=100;game.sudoku.board[0][0][0].y=100;game.sudoku.board[0][0][0].hovered=1;assert(draw_game(&game,NULL,pixels)==0);assert(notifications==0);return 0;}
''',source_flags=['-O2','-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-Wno-implicit-function-declaration','-Werror=uninitialized'])

    def test_pixel_wings_scores_preserve_player_state_and_owned_table_bounds(self):
     project=COMMUNITY/'DuarteOliveira8--FEUP-LCOM/proj/src'
     self.compile_and_run(project,['pixelWings.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "pixelWings.h"
#include <assert.h>
#undef fopen
static const char *contents;static int missing;
FILE*lcom_compat_fopen(const char*path,const char*mode){assert(!strcmp(path,"/home/lcom/proj/src/scores.txt")&&!strcmp(mode,"r"));if(missing)return NULL;if(!contents)return fopen("$SCORES$",mode);FILE*f=tmpfile();assert(f);fputs(contents,f);rewind(f);return f;}
static void clean(gameData*game){memset(game,0,sizeof*game);game->dragonX=37;game->dragonY=326;game->currentDragon=1;game->batPeriod=180;}
static void unchanged(const gameData*game){assert(game->dragonX==37&&game->dragonY==326&&game->currentDragon==1&&game->batPeriod==180);}
int main(void){gameData game;clean(&game);loadScores(&game);unchanged(&game);assert(game.scoreCounter==10);assert(!strcmp(game.scores[0].username,"DUARTE")&&game.scores[0].usernameCounter==6&&game.scores[0].valor==99999);assert(!memcmp(game.scores[0].currentDate,(char[]){0,2,'/',0,1,'/',2,0,1,8},10));assert(!strcmp(game.scores[9].username,"OLA")&&game.scores[9].valor==125);
    contents="12-ABCDEFGHIJKL-50-31/12/2017\n13-";clean(&game);loadScores(&game);unchanged(&game);assert(game.scoreCounter==1&&!strcmp(game.scores[0].username,"ABCDEFGHIJKL"));
    const char*invalid[]={"13-ABCDEFGHIJKLM-50-01/01/2018\n","4-ABC-5-01/01/2018\n","3-ABC-5-01/01\n","3-ABC-5-01/99/2018\n","3-ABC-5-01/01/2018TRAILING\n"};for(unsigned n=0;n<sizeof invalid/sizeof*invalid;n++){contents=invalid[n];clean(&game);loadScores(&game);unchanged(&game);assert(game.scoreCounter==0);}
    contents="1-A-1-01/01/2018\n1-B-2-01/01/2018\n1-C-3-01/01/2018\n1-D-4-01/01/2018\n1-E-5-01/01/2018\n1-F-6-01/01/2018\n1-G-7-01/01/2018\n1-H-8-01/01/2018\n1-I-9-01/01/2018\n1-J-10-01/01/2018\n1-K-11-01/01/2018\n1-L-12-01/01/2018\n";clean(&game);loadScores(&game);unchanged(&game);assert(game.scoreCounter==10&&!strcmp(game.scores[9].username,"J"));
    missing=1;clean(&game);loadScores(&game);unchanged(&game);assert(game.scoreCounter==0);return 0;}
'''.replace('$SCORES$',str(project/'scores.txt')),source_flags=['-DLCOM_COMPAT_LEGACY=1','-include','lcom/compat.h'])

    def test_sudoku_menu_waits_for_actual_keyboard_escape(self):
        self.compile_and_run(COMMUNITY/'antoniopedrofraga--LCOM-FEUP/proj/code',['jogo.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "jogo.h"
static unsigned char pixels[1024*768*2];static unsigned notifications;
int drawBitmap_byLine(Bitmap*b,int x,int y,Alignment a){return 0;}
int drawBitmap_byPixel(Bitmap*b,int x,int y,Alignment a){return 0;}
int drawBitmap_onBoard(Bitmap*b,int x,int y,Alignment a,unsigned char**buffer){return 0;}
int erase_square(Bitmap*b,int x,int y,Alignment a,unsigned char**buffer,unsigned char*bg){return 0;}
unsigned char*double_buffering(void){return pixels;}
void*vg_adress(void){return pixels;}
short get_h_res(void){return 1024;}short get_v_res(void){return 768;}
void clean_buffer(void){}
int get_day(void){return 1;}int get_month(void){return 1;}int get_year(void){return 17;}int get_hour(void){return 1;}int get_minutes(void){return 1;}
unsigned long get_kb(void){return ESC;}unsigned short get_packets(void){return 0;}
int driver_receive(endpoint_t source,message*m,int*status){assert(notifications<2);memset(m,0,sizeof*m);m->m_source=HARDWARE;*status=NOTIFY;m->NOTIFY_ARG=notifications++==0?2:1;return 0;}
FILE*lcom_compat_fopen(const char*path,const char*mode){return *mode=='r'?NULL:tmpfile();}
int main(void){Game game={0};game.irq_set_timer=2;game.irq_set_kb=1;assert(menu1(&game)==0);assert(notifications==2);return 0;}
''',source_flags=['-O2','-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-Wno-implicit-function-declaration','-Werror=uninitialized'])

    def test_sudoku_difficulty_path_and_board_records_are_bounded(self):
        self.compile_and_run(COMMUNITY/'antoniopedrofraga--LCOM-FEUP/proj/code',['jogo.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "jogo.h"
static unsigned difficulty;static const char *records;
FILE*lcom_compat_fopen(const char*path,const char*mode){const char*dif[]={"easy","medi","hard"};char prefix[100];snprintf(prefix,sizeof prefix,"//home//lcom1415-t6g09-proj//sudoku//%s//",dif[difficulty-1]);assert(!strncmp(path,prefix,strlen(prefix)));assert(!strcmp(mode,"r"));const char*end=path+strlen(prefix);assert(end[0]>='1'&&end[0]<='3'&&!strcmp(end+1,".txt"));FILE*f=tmpfile();assert(f);fputs(records,f);rewind(f);return f;}
int main(void){Game game;for(difficulty=1;difficulty<=3;difficulty++){memset(&game,0,sizeof game);records="7 1 2 3\n";assert(load_sudoku(&game,difficulty)==0);assert(game.sudoku.board[1][2][3].value==7&&game.sudoku.board[1][2][3].hovered==0);}difficulty=1;records="7 4 2 3\n";assert(load_sudoku(&game,1)==1);records="7 1";assert(load_sudoku(&game,1)==1);return 0;}
''',source_flags=['-O2','-fstack-protector-all','-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-Wno-implicit-function-declaration'])

    def test_sudoku_mouse_uses_header_sign_bits_for_full_ps2_range(self):
        self.compile_and_run(COMMUNITY/'antoniopedrofraga--LCOM-FEUP/proj/code',['jogo.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "jogo.h"
int drawBitmap_byPixel(Bitmap*b,int x,int y,Alignment a){return 0;}
int main(void){int deltas[]={-255,-200,-128,-127,-1,0,1,127,128,200,255};Game game={0};game.mouse.w=64;game.mouse.h=51;for(unsigned i=0;i<sizeof deltas/sizeof deltas[0];i++)for(unsigned j=0;j<sizeof deltas/sizeof deltas[0];j++){int x=deltas[i],y=deltas[j];game.mouse.x=500;game.mouse.y=400;int packet[3]={8|(x<0?16:0)|(y<0?32:0),x&255,y&255};draw_mouse(&game,packet);int ex=500+2*x,ey=400-2*y;if(ex<0)ex=0;if(ex>959)ex=959;if(ey<0)ey=0;if(ey>716)ey=716;assert(game.mouse.x==ex&&game.mouse.y==ey);}return 0;}
''',source_flags=['-O2','-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-Wno-implicit-function-declaration'])

    def test_sudoku_mouse_commands_consume_their_responses(self):
        self.compile_and_run(COMMUNITY/'antoniopedrofraga--LCOM-FEUP/proj/code',['mouse.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "mouse.h"
static unsigned pending,commands;static uint32_t response=0xfa;
int (sys_outb)(int port,uint32_t data){if(port==0x60){assert(data==0xf4||data==0xf5);pending++;commands++;}return 0;}
int (sys_inb)(int port,uint32_t*value){if(port==0x64)*value=pending?0x21:0;else{assert(port==0x60&&pending);pending--;*value=response;}return 0;}
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){uint32_t v;int r=(sys_inb)(port,&v);if(!r)*value=v;return r;}
uint32_t micros_to_ticks(uint32_t us){return 1;}int tickdelay(clock_t ticks){return 0;}
int main(void){assert(activate_mouse()==0);assert(pending==0&&commands==1);assert(deactivate_mouse()==0);assert(pending==0&&commands==2);response=0xfe;assert(activate_mouse()==1&&pending==0);return 0;}
''',source_flags=['-O2','-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1'])

    def test_sudoku_clock_redraw_does_not_apply_partial_mouse_packet(self):
        self.compile_and_run(COMMUNITY/'antoniopedrofraga--LCOM-FEUP/proj/code',['jogo.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "jogo.h"
static unsigned char pixels[1024*768*2];static unsigned notifications;static Game*observed;
int drawBitmap_byLine(Bitmap*b,int x,int y,Alignment a){return 0;}
int drawBitmap_byPixel(Bitmap*b,int x,int y,Alignment a){return 0;}
int drawBitmap_onBoard(Bitmap*b,int x,int y,Alignment a,unsigned char**buffer){return 0;}
int erase_square(Bitmap*b,int x,int y,Alignment a,unsigned char**buffer,unsigned char*bg){return 0;}
unsigned char*double_buffering(void){return pixels;}
void*vg_adress(void){return pixels;}
short get_h_res(void){return 1024;}short get_v_res(void){return 768;}
void clean_buffer(void){}
int get_day(void){return 1;}int get_month(void){return 1;}int get_year(void){return 17;}int get_hour(void){return 1;}int get_minutes(void){return 1;}
unsigned long get_kb(void){return ESC;}unsigned short get_packets(void){static unsigned read;return read++==0?8:12;}
int driver_receive(endpoint_t source,message*m,int*status){assert(notifications<4);if(notifications==3)assert(observed->mouse.x==100&&observed->mouse.y==100);memset(m,0,sizeof*m);m->m_source=HARDWARE;*status=NOTIFY;m->NOTIFY_ARG=notifications<2?4:notifications==2?8:1;notifications++;return 0;}
FILE*lcom_compat_fopen(const char*path,const char*mode){return *mode=='r'?NULL:tmpfile();}
int main(void){Game game={0};observed=&game;game.mouse.x=100;game.mouse.y=100;game.mouse.w=64;game.mouse.h=51;game.irq_set_mouse=4;game.irq_set_rtc=8;game.irq_set_kb=1;assert(menu1(&game)==0);assert(notifications==4);return 0;}
''',source_flags=['-O2','-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-Wno-implicit-function-declaration','-Werror=uninitialized'])

    def test_bricket_collision_iteration_stays_inside_128_bricks(self):
        self.compile_and_run(COMMUNITY/'afonsojramos--feup-lcom/brickbreaker/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "video_gr.c"
char *read_xpm(char*map[],int*w,int*h){abort();}
int main(void){assert(getBrickSize()==128);for(unsigned i=0;i<128;i++)allBricks[i].visible=0;ball.x=450;ball.y=600;testCollision();render.ball2=1;ball2.x=450;ball2.y=600;testCollision2();return 0;}
''')

    def test_bricket_master_clock_returns_success_for_early_escape(self):
        self.compile_and_run(COMMUNITY/'afonsojramos--feup-lcom/brickbreaker/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "i8042.h"
#include "i8254.h"
#include "video_gr.h"
#include "kbd.h"
#include "mouse.h"
#include "rtc.h"
#include "timer.h"
static unsigned frames,notifications;static FILE*logfile;
#define firstRender() (++frames)
#define drawRectangle(...) 0
#define highlightOption(...) ((void)0)
#define cycleAlphabet(...) ((void)0)
#define loadSprites() ((void)0)
#define moveCursor(...) ((void)0)
#define saveHighscores() ((void)0)
#define sortHighscore() ((void)0)
#define updateCheck() 0
#define extractDate(...) ((void)0)
#define updateTime(...) ((void)0)
#define kbd_subscribe_int() (1u<<13)
#define mouse_subscribe_int() (1u<<1)
#define kbd_unsubscribe_int() 0
#define mouse_unsubscribe_int() 0
#define keyboard_int_handler_C() ESC_KEY
#define setupMouse() ((void)0)
#define testCursorCol() 0
#define moveBall() ((void)0)
#define testCollision() 0
#define testCollision2() 0
#define testBallCol() 0
#define launchHighscore() ((void)0)
#define testPowerUpCollision() ((void)0)
#define movePaddle(...) ((void)0)
#define renderFrame() ((void)0)
#define updateTimer() ((void)0)
#include "timer.c"
int sys_irqsetpolicy(int irq,int flags,int*hook){assert(irq==0&&*hook==4);return 0;}
int sys_irqenable(int*hook){return 0;}
int sys_irqdisable(int*hook){return 0;}
int sys_irqrmpolicy(int*hook){return 0;}
int (sys_inb)(int port,uint32_t*value){return EIO;}
int driver_receive(endpoint_t endpoint,message*m,int*status){assert(notifications<2);memset(m,0,sizeof*m);m->m_source=HARDWARE;*status=NOTIFY;m->NOTIFY_ARG=notifications++==0?(1u<<13):(1u<<4);return 0;}
FILE *lcom_compat_fopen(const char*path,const char*mode){assert(!strcmp(path,"/tmp/log")&&!strcmp(mode,"a+"));logfile=tmpfile();return logfile;}
int main(void){sm.state=GAME_MENU;assert(masterClock()==0);assert(frames==1&&notifications==2);fclose(logfile);return 0;}
''')

    def test_bricket_cursor_stays_visible_at_frame_edges(self):
        self.compile_and_run(COMMUNITY/'afonsojramos--feup-lcom/brickbreaker/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "video_gr.c"
static unsigned char pixels[1024*768];
char *read_xpm(char*map[],int*w,int*h){*w=*h=1;return calloc(1,1);}
int main(void){video_mem=(char*)pixels;h_res=1024;v_res=768;bits_per_pixel=8;cursor.sensitivity=.15f;cursor.x=cursor.y=0;moveCursor(-127,127);assert(cursor.x==0 && cursor.y==0);cursor.x=1003;cursor.y=747;moveCursor(127,-127);assert(cursor.x==1003 && cursor.y==747);cursor.x=500;cursor.y=400;moveCursor(-200,200);assert(cursor.x==470 && cursor.y==370);return 0;}
''')

    def test_bricket_mouse_decodes_complete_signed_9_bit_deltas(self):
        self.compile_and_run(COMMUNITY/'afonsojramos--feup-lcom/brickbreaker/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "i8042.h"
#include "i8254.h"
#include "video_gr.h"
#include "kbd.h"
#include "mouse.h"
#include "rtc.h"
#include "timer.h"
static unsigned frames,notifications;static FILE*logfile; static int observed_x, observed_y;
#define firstRender() (++frames)
#define drawRectangle(...) 0
#define highlightOption(...) ((void)0)
#define cycleAlphabet(...) ((void)0)
#define loadSprites() ((void)0)
#define moveCursor(x,y) (observed_x=(x),observed_y=(y))
#define saveHighscores() ((void)0)
#define sortHighscore() ((void)0)
#define updateCheck() 0
#define extractDate(...) ((void)0)
#define updateTime(...) ((void)0)
#define kbd_subscribe_int() (1u<<13)
#define mouse_subscribe_int() (1u<<1)
#define kbd_unsubscribe_int() 0
#define mouse_unsubscribe_int() 0
#define keyboard_int_handler_C() ESC_KEY
#define setupMouse() ((void)0)
#define testCursorCol() 0
#define moveBall() ((void)0)
#define testCollision() 0
#define testCollision2() 0
#define testBallCol() 0
#define launchHighscore() ((void)0)
#define testPowerUpCollision() ((void)0)
#define movePaddle(...) ((void)0)
#define renderFrame() ((void)0)
#define updateTimer() ((void)0)
#include "timer.c"
int sys_irqsetpolicy(int irq,int flags,int*hook){assert(irq==0&&*hook==4);return 0;}
int sys_irqenable(int*hook){return 0;}
int sys_irqdisable(int*hook){return 0;}
int sys_irqrmpolicy(int*hook){return 0;}
int (sys_inb)(int port,uint32_t*value){return EIO;}
int driver_receive(endpoint_t endpoint,message*m,int*status){assert(notifications<2);memset(m,0,sizeof*m);m->m_source=HARDWARE;*status=NOTIFY;m->NOTIFY_ARG=notifications++==0?(1u<<13):(1u<<4);return 0;}
FILE *lcom_compat_fopen(const char*path,const char*mode){assert(!strcmp(path,"/tmp/log")&&!strcmp(mode,"a+"));logfile=tmpfile();return logfile;}
int main(void){int deltas[]={-255,-200,-128,-127,-1,0,1,127,128,200,255};sm.state=GAME_MENU;for(unsigned i=0;i<sizeof deltas/sizeof deltas[0];i++)for(unsigned j=0;j<sizeof deltas/sizeof deltas[0];j++){int x=deltas[i],y=deltas[j];char packet[3]={(char)(8|(x<0?16:0)|(y<0?32:0)),(char)x,(char)y};mouseProcessing(packet,0);assert(observed_x==x && observed_y==y);}return 0;}
''')

    def test_whack_entry_uses_owning_video_declaration(self):
        self.compile_and_run(COMMUNITY/'joaoacsoares--LCOM/lcom1415-t3g15/projeto/src',['main.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "Whack.h"
#include "Mouse.h"
#include "Graphics.h"
int original_game_main(int,char**);
static Whack game={.done=1};static int initialized,exited,stopped;
void sef_startup(void){}
time_t lcom_compat_time(time_t*t){if(t)*t=123;return 123;}
Whack *startWhack(void){return &game;}
void stopWhack(Whack*w){assert(w==&game);stopped++;}
void *vg_init(unsigned short mode){assert(mode==0x114);initialized++;return &game;}
int vg_exit(void){exited++;return 0;}
void updateWhack(Whack*w){abort();}
void drawWhack(Whack*w){abort();}
int get_counter(Timer_t*t){abort();}
Mouse *getMouse(void){abort();}
void flipMouse(void){abort();}
void drawMouse(void){abort();}
void flipDisplay(void){abort();}
int main(void){char*args[]={"./projeto",NULL};assert(original_game_main(1,args)==0);assert(initialized==1&&exited==1&&stopped==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmain=original_game_main'])

    def test_whack_framebuffer_mapping_and_copies_use_pixel_bytes(self):
        self.compile_and_run(COMMUNITY/'joaoacsoares--LCOM/lcom1415-t3g15/projeto/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "Graphics.c"
#define FRAME_BYTES (800u*600u*2u)
static unsigned char*frame;
int lcom_compat_legacy_int86(struct reg86u*r){return 0;}
int vbe_get_mode_info(unsigned short mode,vbe_mode_info_t*info){memset(info,0,sizeof*info);info->XResolution=800;info->YResolution=600;info->BitsPerPixel=16;info->PhysBasePtr=0xe0000000;return 0;}
int sys_privctl(endpoint_t ep,int request,void*arg){struct mem_range*range=arg;assert(range->mr_base==0xe0000000u&&range->mr_limit-range->mr_base==FRAME_BYTES);return 0;}
void*vm_map_phys(endpoint_t ep,void*address,size_t length){assert(length==FRAME_BYTES);return frame;}
int main(void){size_t page=(size_t)sysconf(_SC_PAGESIZE),pages=(FRAME_BYTES+page-1)/page*page;unsigned char*allocation=mmap(NULL,pages+page,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);assert(allocation!=MAP_FAILED);assert(mprotect(allocation+pages,page,PROT_NONE)==0);frame=allocation+pages-FRAME_BYTES;assert(vg_init(0x114)==frame);assert(getMemSize()==FRAME_BYTES);memset(frame,0x5a,FRAME_BYTES);saveBuffer();flipMouse();memset(frame,0,FRAME_BYTES);flipDisplay();for(size_t i=0;i<FRAME_BYTES;++i)assert(frame[i]==0x5a);free(buffer);free(mouseBuffer);munmap(allocation,pages+page);return 0;}
''')

    def test_whack_mallet_renderer_clips_to_actual_framebuffer(self):
        self.compile_and_run(COMMUNITY/'joaoacsoares--LCOM/lcom1415-t3g15/projeto/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "Bitmap.c"
#define FRAME_BYTES (800u*600u*2u)
static unsigned char*frame;
unsigned int getHorRes(void){return 800;}
unsigned int getVerRes(void){return 600;}
char *getVideoMemMouse(void){return (char*)frame;}
int main(void){size_t page=(size_t)sysconf(_SC_PAGESIZE),pages=(FRAME_BYTES+page-1)/page*page;unsigned char*allocation=mmap(NULL,pages+page,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);assert(allocation!=MAP_FAILED);assert(mprotect(allocation+pages,page,PROT_NONE)==0);frame=allocation+pages-FRAME_BYTES;unsigned char pixels[4*3*2];for(unsigned i=0;i<12;i++){pixels[i*2]=(unsigned char)(i+1);pixels[i*2+1]=(unsigned char)(i+21);}pixels[8]=255;pixels[9]=255;Bitmap bmp={.bitmapInfoHeader={.width=4,.height=3},.bitmapData=pixels};unsigned char*expected=malloc(FRAME_BYTES);const int points[][2]={{799,599},{-2,-1},{798,598},{-8,20},{810,20},{20,-5},{20,610},{785,585},{10,10}};for(unsigned n=0;n<sizeof(points)/sizeof(*points);n++){int ox=points[n][0],oy=points[n][1];memset(frame,0x71,FRAME_BYTES);memset(expected,0x71,FRAME_BYTES);for(int sy=0;sy<3;sy++)for(int sx=0;sx<4;sx++){int x=ox+sx,y=oy+2-sy;if(x<0||x>=800||y<0||y>=600)continue;unsigned char*pixel=pixels+(sy*4+sx)*2;if(pixel[0]==255&&pixel[1]==255)continue;memcpy(expected+(y*800+x)*2,pixel,2);}drawBitmapMouse(&bmp,ox,oy,ALIGN_LEFT);assert(!memcmp(frame,expected,FRAME_BYTES));}free(expected);munmap(allocation,pages+page);return 0;}
''')

    def test_dodgem_timer_status_does_not_overwrite_byte_destination(self):
        self.compile_and_run(COMMUNITY/'dansreis--LCOM-DODGEM/DODGEM/src',['timer.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "timer.h"
static unsigned writes,failed;static uint16_t ports[12];static uint8_t values[12];
int sys_outb(int port,uint32_t value){assert(writes<12);ports[writes]=port;values[writes++]=(uint8_t)value;return 0;}
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){assert(port==0x40);if(failed)return EIO;*value=0xb6;return 0;}
int main(void){assert(timer_set_square(0,60)==0);assert(writes==4);assert(ports[0]==0x43&&values[0]==0xe2);assert(ports[1]==0x43&&values[1]==0x36);assert(ports[2]==0x40&&values[2]==0xae);assert(ports[3]==0x40&&values[3]==0x4d);struct{unsigned char before[16],status,after[16];}guard;memset(&guard,0xa5,sizeof guard);assert(timer_get_conf(0,&guard.status)==0&&guard.status==0xb6);for(unsigned i=0;i<16;i++)assert(guard.before[i]==0xa5&&guard.after[i]==0xa5);failed=1;guard.status=0x71;assert(timer_get_conf(0,&guard.status)!=0&&guard.status==0x71);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-fstack-protector-all'])

    def test_gold_miner_reads_mouse_ack_only_when_output_buffer_is_full(self):
     self.compile_and_run(COMMUNITY/'Andrepereira2001--feup-lcom-2020/src',['mouse.c'],r'''#include <lcom/lcf.h>
#include "mouse.h"
#include <assert.h>
static unsigned reads,delays,writes;static bool pending=true;
int util_sys_inb(int port,uint8_t *value){if(port==0x64)*value=pending?0x21:0;else {assert(port==0x60&&pending);*value=0xfa;pending=false;reads++;}return 0;}
int(sys_outb)(int port,uint32_t value){if((writes++%2)==0){assert(port==0x64&&value==0xd4);}else {assert(port==0x60&&(value==0xf4||value==0xf5));pending=true;}return 0;}
uint32_t micros_to_ticks(uint32_t micros){assert(micros);return 1;}
int tickdelay(clock_t ticks){assert(ticks);delays++;return 0;}
int main(void){uint8_t ack=0xcc;assert(mouse_read_ack(&ack)==0&&ack==0xfa&&reads==1&&delays==0);assert(mouse_enable_data_reporting_me()==0&&reads==2&&writes==2&&delays==0);assert(mouse_disable_data_reporting()==0&&reads==3&&writes==4&&delays==0);return 0;}
''')

    def test_retchut_cursor_constructor_fits_its_owned_allocation(self):
     project=COMMUNITY/'Retchut--LCOM-2020-2021/proj/src'
     self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
static unsigned char *allocation;static size_t page;
static void *cursor_malloc(size_t length){page=(size_t)sysconf(_SC_PAGESIZE);assert(length&&length<=page);allocation=mmap(NULL,page*2,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);assert(allocation!=MAP_FAILED);assert(mprotect(allocation+page,page,PROT_NONE)==0);return allocation+page-length;}
#define malloc cursor_malloc
#include "$OWNER$"
#undef malloc
struct Image game_img[6];
int main(void){assert(generate_cursor()==0);assert(cursor_obj->x==400&&cursor_obj->y==300&&cursor_obj->img==&game_img[crosshair_img]);assert(munmap(allocation,page*2)==0);return 0;}
'''.replace('$OWNER$',str(project/'logic.c')))
    def test_retchut_highscores_stop_at_owned_table_and_name_bounds(self):
     project=COMMUNITY/'Retchut--LCOM-2020-2021/proj/src'
     self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
static const char *contents;
static FILE *scores_open(const char *name,const char *mode){assert(strcmp(name,"home/lcom/labs/proj/highscores.txt")==0&&strcmp(mode,"r")==0);FILE*f=tmpfile();assert(f);fputs(contents,f);rewind(f);return f;}
#define fopen scores_open
#include "$OWNER$"
#undef fopen
static unsigned char *pages[8];static unsigned used;static size_t page;
static void *guarded(size_t length){unsigned char *p=mmap(NULL,page*2,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);assert(p!=MAP_FAILED&&used<8);pages[used++]=p;assert(mprotect(p+page,page,PROT_NONE)==0);return p+page-length;}
int main(void){page=(size_t)sysconf(_SC_PAGESIZE);char**n=guarded(5*sizeof*n);int*t=guarded(5*sizeof*t),*s=guarded(5*sizeof*s);for(unsigned i=0;i<5;i++){n[i]=guarded(6);strcpy(n[i],"NONE");t[i]=s[i]=0;}
    contents="ALICE 1 10\nBOB 2 20\nCAROL 3 30\nDAVE 4 40\nEVE 5 50\nEXTRA 6 60\n";assert(load_highscores(n,t,s)==0);assert(strcmp(n[0],"EVE")==0&&s[0]==50&&t[0]==5);assert(strcmp(n[4],"ALICE")==0&&s[4]==10);
    for(unsigned i=0;i<5;i++){strcpy(n[i],"NONE");t[i]=s[i]=0;}contents="ABCDEFGHIJKLMNOPQRSTUVWXYZ 1 99\n";assert(load_highscores(n,t,s)==0);for(unsigned i=0;i<5;i++)assert(strcmp(n[i],"NONE")==0&&s[i]==0&&t[i]==0);for(unsigned i=0;i<used;i++)assert(munmap(pages[i],page*2)==0);return 0;}
'''.replace('$OWNER$',str(project/'highscore.c')))

    def test_dodgem_score_loader_bounds_real_records_and_eof(self):
     project=COMMUNITY/'dansreis--LCOM-DODGEM/DODGEM/src'
     self.compile_and_run(project,[],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
static const char *contents;
static FILE *score_input(const char *filename,const char *mode);
#define fopen score_input
#include "$OWNER$"
#undef fopen
static FILE *score_input(const char *filename,const char *mode){assert(strcmp(filename,FILENAME)==0&&strcmp(mode,"r")==0);if(!contents)return fopen("$SCORES$","r");FILE *file=tmpfile();assert(file);fputs(contents,file);rewind(file);return file;}
int main(void){
    contents="0\nALICE\n48\n10\n27 12 14 10 50 15 \n";
    assert(loadScores()==0&&noborderSize==1&&borderSize==0);assert(strcmp(players_noborder[0].nickname,"ALICE")==0&&players_noborder[0].segundos==48&&players_noborder[0].centesimas==10);assert(players_noborder[0].data.day==27&&players_noborder[0].data.month==12&&players_noborder[0].data.year==14&&players_noborder[0].data.hours==10&&players_noborder[0].data.min==50&&players_noborder[0].data.sec==15);
    contents=NULL;assert(loadScores()==0&&borderSize==3&&noborderSize==4);assert(strcmp(players_border[0].nickname,"contador5")==0&&players_border[0].data.hours==18);
    contents="1\nABCDEFGHIJK\n48\n10\n27 12 14 10 50 15 \n";assert(loadScores()==0&&borderSize==1&&strcmp(players_border[0].nickname,"ABCDEFGHIJK")==0);
    contents="1\nABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ\n48\n10\n27 12 14 10 50 15 \n";assert(loadScores()!=0&&borderSize==0&&noborderSize==0);
    contents="1\nA\n00\n";assert(loadScores()!=0&&borderSize==0);
    contents="0\nA\n00\n00\nnot a date\n";assert(loadScores()!=0&&noborderSize==0);
    contents="0\n0s2\n";assert(loadScores()==-1&&noborderSize==0);
    char many[512]={0};for(unsigned i=0;i<6;i++)strcat(many,"0\nBOB\n01\n02\n27 12 14 10 50 15 \n");contents=many;assert(loadScores()==0&&noborderSize==4&&borderSize==0);return 0;
}
'''.replace('$OWNER$',str(project/'DODGEM.c')).replace('$SCORES$',str(project.parent/'scores.txt')))

    def test_set_game_calls_use_the_owning_dispatcher_prototypes(self):
     self.compile_and_run(COMMUNITY/'francisquinha--LCOM/code',['game.c'],'int main(void){return 0;}\n',source_flags=['-DLCOM_COMPAT_LEGACY=1','-include','lcom/compat.h','-Werror=implicit-function-declaration'])
    def test_set_first_mouse_packet_preserves_bit_three_in_payload_bytes(self):
     self.compile_and_run(COMMUNITY/'francisquinha--LCOM/code',['mouse.c'],r'''#include <lcom/lcf.h>
#include "mouse.h"
#include <assert.h>
static const unsigned char *stream;static size_t offset;
int lcom_compat_sys_inb_ulong(int port,unsigned long *value){if(port==0x64)*value=0x21;else {assert(port==0x60);*value=stream[offset++];}return 0;}
uint32_t micros_to_ticks(uint32_t micros){(void)micros;abort();return 0;}
int tickdelay(clock_t ticks){(void)ticks;abort();return 1;}
static void packet(const unsigned char bytes[3],int dx,int dy){Mouse *mouse=create_mouse();assert(mouse);stream=bytes;offset=0;for(unsigned i=0;i<3;i++)mouse_int_handler(mouse);assert(offset==3&&mouse->info&&mouse->byte_counter==0&&!mouse->first_packet);assert(mouse->x==dx&&mouse->y==dy);free(mouse);}
int main(void){const unsigned char positive[]={0x08,9,9},negative[]={0x28,26,232};packet(positive,9,-9);packet(negative,26,24);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-include','lcom/compat.h'])

    def test_one_shot_lever_loader_retains_images_without_freeing_local_metadata(self):
     project=COMMUNITY/'pedrojfs17--FEUP-LCOM/proj/src'
     self.compile_and_run(project,[],r'''#include "$GAME_SOURCE$"
#include <assert.h>
static unsigned loaded;
static uint32_t pixels[10][4];
uint8_t *xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t *image){assert(map&&type==XPM_8_8_8_8&&loaded<10);pixels[loaded][3]=0xff001100+loaded;*image=(xpm_image_t){.type=type,.width=2,.height=2,.size=sizeof pixels[0],.bytes=(uint8_t*)pixels[loaded++]};return image->bytes;}
int main(void){LoadLeverXPM();assert(loaded==10);for(unsigned i=0;i<7;i++)assert(LeverPositions[i].bytes==(uint8_t*)pixels[i]&&((uint32_t*)LeverPositions[i].bytes)[3]==0xff001100+i);for(unsigned i=0;i<2;i++)assert(LeverColors[i].bytes==(uint8_t*)pixels[i+7]);assert(CleanLever.bytes==(uint8_t*)pixels[9]);return 0;}
'''.replace('$GAME_SOURCE$',str(project/'Game.c')))
    def test_one_shot_multiplayer_heart_renderer_keeps_stack_metadata_owned(self):
     self.compile_and_run(COMMUNITY/'pedrojfs17--FEUP-LCOM/proj/src',['MultiPlayer.c'],r'''#include <lcom/lcf.h>
#include "MultiPlayer.h"
#include <assert.h>
static unsigned loaded,blue,red;
static uint32_t *pixels[2];
uint8_t *xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t *image){assert(map&&type==XPM_8_8_8_8&&loaded<2);unsigned n=loaded++;pixels[n]=malloc(32*32*sizeof(uint32_t));assert(pixels[n]);for(unsigned i=0;i<32*32;i++)pixels[n][i]=n?0xff223344:0xff556677;*image=(xpm_image_t){.type=type,.width=32,.height=32,.size=32*32*sizeof(uint32_t),.bytes=(uint8_t*)pixels[n]};return image->bytes;}
uint32_t xpm_transparency_color(enum xpm_image_type type){assert(type==XPM_8_8_8_8);return 0xff000000;}
int changePixelColor(uint16_t x,uint16_t y,uint32_t color){assert(y>=734&&y<766);if(color==0xff556677){assert(x>=100&&x<212);blue++;}else {assert(color==0xff223344&&x>=830&&x<942);red++;}return 0;}
int main(void){draw_hearts();assert(loaded==2&&blue==3*32*32&&red==3*32*32);return 0;}
''')

    def test_one_shot_player_constructor_keeps_loaded_image_data_alive(self):
        self.compile_and_run(COMMUNITY/'pedrojfs17--FEUP-LCOM/proj/src',['Player.c'],r'''#include <lcom/lcf.h>
#include "Player.h"
#include <assert.h>
static uint32_t pixels[4]={0xff112233,0xff445566,0xff778899,0xffaabbcc};
uint8_t *xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t *image){assert(type==XPM_8_8_8_8);*image=(xpm_image_t){.type=type,.width=2,.height=2,.size=sizeof pixels,.bytes=(uint8_t*)pixels};return image->bytes;}
int main(void){for(unsigned i=0;i<3;++i){Player *p=createPlayer(i);assert(p);assert(p->img.width==2&&p->img.height==2&&p->img.bytes==(uint8_t*)pixels);assert(((uint32_t*)p->img.bytes)[3]==0xffaabbcc);free(p);}return 0;}
''')

    def test_get_lost_first_clock_draw_never_cleans_uninitialized_width(self):
        self.compile_and_run(COMMUNITY/'RitaPeixoto--FEUP-LCOM/proj/src',['interrupts.c','rtc.c'],r'''#include <lcom/lcf.h>
#include "interrupts.h"
#include "rtc.h"
#include <assert.h>
#include <sys/mman.h>
static unsigned selected,cleared,menus,draws;
uint32_t timer_counter;
static xpm_image_t glyph;
int(sys_outb)(int port,uint32_t value){assert(port==0x70);selected=value;return 0;}
int(sys_inb)(int port,uint32_t *value){assert(port==0x71);switch(selected){case 0:*value=5;break;case 2:*value=0x22;break;case 4:*value=0x21;break;case 7:*value=0x16;break;case 8:*value=0x10;break;case 9:*value=0x26;break;case 10:*value=0;break;default:assert(0);}return 0;}
uint16_t gethresolution(void){return 1152;}
uint16_t getvresolution(void){return 864;}
void timer_int_handler(void){timer_counter++;}
void draw_menu(Menu *menu){menus++;}
int draw_mouse(Mouse *mouse){return 0;}
void updateBuffer(void){}
void clean_mouse(int x,int y,xpm_image_t image){assert(0);}
uint8_t getContent(void){assert(0);return 0;}
void mouse_ih(void){assert(0);}
void kbc_ih(void){assert(0);}
void mouse_packet_parse(struct packet *packet,uint8_t bytes[3]){assert(0);}
bool check_possible_mov(struct packet packet,Mouse *mouse,bool menu){assert(0);return false;}
int mouse_gestures(Menu *menu,struct packet *packet,int x,int y,RealTime *rt,Numbers *numbers){assert(0);return 0;}
int draw_xpm(uint16_t x,uint16_t y,xpm_image_t image){assert(x+image.width<=1152 && y+image.height<=864);draws++;return 0;}
xpm_image_t numbers_to_xpm(Numbers *numbers,int value){assert(value>=0&&value<=9);return glyph;}
int change_pixel_color_32(uint16_t x,uint16_t y,uint32_t color){assert(x<1152&&y<864);cleared++;return 0;}
int main(void){
 size_t length=1152*864*4,page=(size_t)sysconf(_SC_PAGESIZE);assert(length%page==0);
 unsigned char *image=mmap(NULL,length+page,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);assert(image!=MAP_FAILED);assert(mprotect(image+length,page,PROT_NONE)==0);
 glyph=(xpm_image_t){.width=21,.height=30};Numbers numbers={.zero=glyph,.two=glyph,.slash=glyph,.colon=glyph};Menu menu={.img_initial={.bytes=image,.width=1152,.height=864}};Mouse mouse={0};RealTime rt={.wt=0xa0909};message event={0};event.m_notify.interrupts=1;
 assert(interrupts_state_menu(event,2,4,1,&menu,&mouse,&numbers,&rt)==0);assert(rt.wt>60&&rt.wt<1152);assert(cleared==0&&draws>0&&menus==1);
 timer_counter=59;assert(interrupts_state_menu(event,2,4,1,&menu,&mouse,&numbers,&rt)==0);assert(cleared>0&&menus==1);munmap(image,length+page);return 0;
}''')

    def test_minixforce_vbe_info_allocates_complete_bios_result(self):
     self.compile_and_run(COMMUNITY/'Marantesss--feup-lcom/proj/src',['vbe.c'],r'''#include <lcom/lcf.h>
   #include "vbe.h"
   #include <assert.h>
   static size_t allocated;
   static vbe_mode_info_t result;
   void *lm_init(bool enable){return &result;}
   void *lm_alloc(size_t length,mmap_t *map){allocated=length;map->phys=0x10000;map->virt=&result;map->size=length;return map->virt;}
   bool lm_free(const mmap_t *map){assert(map->virt==&result);return true;}
   int lcom_compat_legacy_int86(struct reg86u *regs){assert(regs->u.w.ax==0x4f01 && regs->u.w.cx==0x143);if(allocated<sizeof result)return 22;result.XResolution=800;result.YResolution=600;result.BitsPerPixel=32;regs->u.w.ax=0x004f;return 0;}
   int main(void){vbe_mode_info_t info={0};assert(vbe_get_info_mode(0x143,&info)==0);assert(info.XResolution==800 && info.YResolution==600 && info.BitsPerPixel==32);return 0;}
   ''')

    def test_dragon_maps_packed24_framebuffer_bytes(self):
     self.compile_and_run(COMMUNITY/'nmtc01--LCOM-Projects/proj/src',['graphi.c'],r'''#include <lcom/lcf.h>
   #include "graphi.h"
   #include <assert.h>
   static vbe_mode_info_t info;
   static unsigned char pixels[1152*864*3];
   void *lm_init(bool enable){return &info;}
   void *lm_alloc(size_t length,mmap_t *map){assert(length==sizeof info);map->phys=0x10000;map->virt=&info;map->size=length;return map->virt;}
   bool lm_free(const mmap_t *map){return true;}
   int lcom_compat_legacy_int86(struct reg86u *regs){if(regs->u.w.ax==0x4f01){info.XResolution=1152;info.YResolution=864;info.BitsPerPixel=24;info.PhysBasePtr=0xe0000000;}else assert(regs->u.w.ax==0x4f02 && regs->u.w.bx==(0x14b|BIT(14)));regs->u.w.ax=0x004f;return 0;}
   int sys_privctl(endpoint_t ep,int req,void *arg){struct minix_mem_range *range=arg;assert(range->mr_limit-range->mr_base==sizeof pixels);return 0;}
   void *vm_map_phys(endpoint_t ep,void *base,size_t length){assert(length==sizeof pixels);return pixels;}
   int main(void){assert(vg_init(0x14b)==pixels);return 0;}
   ''')

    def test_dragon_date_renders_four_year_digits_within_storage(self):
     previous=os.environ.get('CC');os.environ['CC']=(previous or 'cc')+' -fstack-protector-all'
     try:
      self.compile_and_run(COMMUNITY/'nmtc01--LCOM-Projects/proj/src',[],r'''#include <lcom/lcf.h>
   #include <assert.h>
   #include "graphi.c"
   int main(void){
    h_res=200;v_res=100;byte_per_pixel=3;second_buffer=calloc(200*100,3);assert(second_buffer);
    unsigned char digits[10][10*18*3];xpm_image_t *images[]={&bola,&one,&two,&three,&four,&five,&six,&seven,&eight,&nine};
    for(int i=0;i<10;i++){memset(digits[i],10+i,sizeof digits[i]);*images[i]=(xpm_image_t){.bytes=digits[i],.width=10,.height=18,.type=XPM_8_8_8};}
    print_date(38,51,20,16,10,26,0,0);
    assert(second_buffer[60*3]==12 && second_buffer[72*3]==10 && second_buffer[84*3]==12 && second_buffer[96*3]==16);
    free(second_buffer);return 0;
   }''')
     finally:
      if previous is None:os.environ.pop('CC',None)
      else:os.environ['CC']=previous

    def test_dragon_startup_requests_packed24_sprites(self):
     self.compile_and_run(COMMUNITY/'nmtc01--LCOM-Projects/proj/src',['proj.c'],r'''#include <lcom/lcf.h>
   #include "graphi.h"
   #include <assert.h>
   static unsigned loads;
   static uint8_t pixels[3];
   void *vg_init(uint16_t mode){assert(mode==0x14b);return pixels;}
   int vg_exit(void){return 0;}
   FILE *lcom_compat_fopen(const char *path,const char *mode){assert(!strcmp(path,"save.txt") && !strcmp(mode,"w"));return tmpfile();}
   uint8_t *xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t *image){assert(map);assert(type==XPM_8_8_8);loads++;*image=(xpm_image_t){.width=1,.height=1,.bytes=pixels,.size=3,.type=type};return pixels;}
   int escape_or_move(xpm_image_t sprite[],xpm_image_t mouth[],xpm_image_t shot[],xpm_image_t big[],xpm_image_t ghost[],xpm_image_t exploded[],uint16_t x,uint16_t y,uint16_t movement,uint8_t rate){assert(loads==6);assert(sprite->type==XPM_8_8_8 && mouth->type==XPM_8_8_8 && shot->type==XPM_8_8_8 && big->type==XPM_8_8_8 && ghost->type==XPM_8_8_8 && exploded->type==XPM_8_8_8);return 0;}
   int main(void){char *argv[]={"proj",NULL};assert(proj_main_loop(1,argv)==0);assert(loads==6);return 0;}
   ''',source_flags=('-Dmain=dragon_program_entry',))

    def test_minixop_map_load_stays_within_its_row_allocation(self):
     self.compile_and_run(COMMUNITY/'DoStini--feup-lcom/proj/system',[],r'''#include <lcom/lcf.h>
   #include <assert.h>
   static struct {unsigned char *p;size_t n;} allocations[16];
   static unsigned count,live;
   static void *guarded_malloc(size_t n){unsigned char *p=malloc(n+16);assert(p && count<16);memset(p+n,0xa5,16);allocations[count++]=(typeof(allocations[0])){p,n};live++;return p;}
   static void check_guards(void){for(unsigned i=0;i<count;i++)if(allocations[i].p)for(size_t j=0;j<16;j++)assert(allocations[i].p[allocations[i].n+j]==0xa5);}
   static void guarded_free(void *p){check_guards();for(unsigned i=0;i<count;i++)if(allocations[i].p==p){free(p);allocations[i].p=NULL;live--;return;}assert(0);}
   static FILE *map_file(const char *name,const char *mode){assert(!strcmp(name,"/home/lcom/labs/minixop/data/maps/map007.mxmap"));assert(!strcmp(mode,"r"));FILE *f=tmpfile();assert(f);fputs("2 3\n-3 0 1\n2 3 0\n",f);rewind(f);return f;}
   #define malloc guarded_malloc
   #define free guarded_free
   #define fopen map_file
   #include "file/map.c"
   #undef malloc
   #undef free
   #undef fopen
   int main(void){map_t map={0};assert(load_map(7,&map)==OK);check_guards();assert(map.lines==2 && map.columns==3);assert(map.matrix[0][0]==-3 && map.matrix[0][1]==0 && map.matrix[0][2]==1);assert(map.matrix[1][0]==2 && map.matrix[1][1]==3 && map.matrix[1][2]==0);close_map(&map);assert(live==0);return 0;}
   ''')

    @unittest.skipUnless(sys.platform == 'darwin' and Path('/usr/lib/libgmalloc.dylib').is_file(),
                         'The full startup allocation check requires macOS GuardMalloc')
    def test_pharmacy_pointer_array_survives_actual_guarded_startup(self):
        project = COMMUNITY/'JoaoAlvaroFerreira--FEUP-LCOM/proj/src'
        runtime_build = Path.cwd() if (Path.cwd()/'machinelab').is_file() else ROOT/'build'
        runtime = runtime_build/'machinelab'
        self.assertTrue(runtime.is_file(), 'Build lcom-ng before running the startup regression')
        with tempfile.TemporaryDirectory(prefix='lcom-pharmacy-startup-') as temporary:
            folder = Path(temporary)
            assets = folder/'assets'
            # Runtime logs and saved scores stay away from the pinned source tree.
            target = assets/'proj/src'
            target.mkdir(parents=True)
            for name in ('images', 'highscores'):
                shutil.copytree(project/name, target/name)
            output = folder/'build'
            built = subprocess.run([sys.executable, str(ROOT/'sdk/compat/build_project.py'),
                                    str(project), '--sdk', str(ROOT/'sdk'), '--runtime-build',
                                    str(runtime_build), '--output', str(output), '--asset-root',
                                    str(assets), '--debug'], capture_output=True, text=True, timeout=30)
            self.assertEqual(built.returncode, 0, built.stdout+built.stderr)
            program = json.loads((output/'build.json').read_text())['binary']
            environment = {**os.environ, 'DYLD_INSERT_LIBRARIES':'/usr/lib/libgmalloc.dylib',
                           'MallocScribble':'1', 'LCOM_COMPAT_PATH_ALIAS_COUNT':'1',
                           'LCOM_COMPAT_PATH_ALIAS_0_FROM':'/home/lcom/labs/proj',
                           'LCOM_COMPAT_PATH_ALIAS_0_TO':'proj'}
            result = subprocess.run([str(runtime), 'run', '--headless', '--max-ticks', '400',
                                     '--timeout', '12', '--', program], env=environment,
                                    capture_output=True, text=True, timeout=15)
            self.assertEqual(result.returncode, 1, result.stdout+result.stderr)
            self.assertIn('max virtual ticks reached (400)', result.stdout+result.stderr)
            self.assertNotIn('SIGABRT', result.stdout+result.stderr)
            self.assertNotIn('DEADLYSIGNAL', result.stdout+result.stderr)

    def test_pharmacy_postgame_name_edits_writable_storage(self):
        self.compile_and_run(COMMUNITY/'JoaoAlvaroFerreira--FEUP-LCOM/proj/src',['proj.c','util.c'],r'''#include <lcom/lcf.h>
void alterName(void);
#include <assert.h>
extern char *name;
extern uint32_t scanCode;
int main(void){assert(strcmp(name,"AAA")==0);scanCode=0x11;alterName();assert(strcmp(name,"BAA")==0);scanCode=0x20;alterName();scanCode=0x11;alterName();assert(strcmp(name,"BBA")==0);scanCode=0x1e;alterName();scanCode=0x1f;alterName();assert(strcmp(name,"ABA")==0);return 0;}
''',source_flags=['-Dmain=pharmacy_guest_entry'])

    def test_sandz_wave_banner_fits_and_preserves_complete_wave_number(self):
        self.compile_and_run(COMMUNITY/'Tiagocv64--SandZ-LCOM-Proj/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdarg.h>
#include <limits.h>
#include "UI_game.h"
#include "text.h"
static char rendered[128];
static int checked_sprintf(char *dest,size_t capacity,const char *format,...){char result[512];va_list args;va_start(args,format);int n=vsnprintf(result,sizeof result,format,args);va_end(args);assert(n>=0 && (size_t)n<capacity);memcpy(dest,result,(size_t)n+1);return n;}
#define sprintf(dest,...) checked_sprintf(dest,sizeof(dest),__VA_ARGS__)
#include "UI_game.c"
#undef sprintf
int h_res=1280,v_res=1024;
void print_string(char *text,uint32_t size,uint32_t y,uint32_t x,uint32_t color,int font,int align){assert(size==strlen(text));assert(size<sizeof rendered);memcpy(rendered,text,size+1);}
int main(void){currentWave=1;drawWave();assert(strcmp(rendered,"All clear, wave 1 incoming!")==0);currentWave=INT_MAX;drawWave();assert(strcmp(rendered,"All clear, wave 2147483647 incoming!")==0);return 0;}
''')

    def test_pharmacy_text_fits_complete_counters_and_name_prompt(self):
        harness = r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <limits.h>
static struct {void *address;size_t bytes;} allocations[256];
static size_t allocated;
static char numbers[4][32];
static unsigned number_count;
static void *checked_malloc(size_t bytes){void *p=malloc(bytes);assert(p && allocated<256);allocations[allocated++]=(typeof(allocations[0])){p,bytes};return p;}
static char *checked_strcpy(char *dest,const char *src){size_t capacity=0;for(size_t i=0;i<allocated;i++)if(allocations[i].address==dest)capacity=allocations[i].bytes;assert(capacity>=strlen(src)+1);memcpy(dest,src,strlen(src)+1);return dest;}
static int checked_sprintf(char *dest,size_t capacity,const char *format,...){char result[512];va_list args;va_start(args,format);int n=vsnprintf(result,sizeof result,format,args);va_end(args);assert(n>=0 && (size_t)n<capacity);memcpy(dest,result,(size_t)n+1);if(!strcmp(format,"%u")||!strcmp(format,"%d")){assert(number_count<4);strcpy(numbers[number_count++],result);}return n;}
#undef sprintf
#undef strcpy
#define sprintf(dest,...) checked_sprintf(dest,sizeof(dest),__VA_ARGS__)
#define strcpy checked_strcpy
#define malloc checked_malloc
#include "util.c"
#undef malloc
#undef strcpy
#undef sprintf
Objeto rato;
unsigned int points,tempo;
Bitmap *loadBitmap(const char *path){return NULL;}
void drawBitmap(Bitmap *bmp,int x,int y,Alignment alignment){}
static void release_objects(void){for(size_t i=0;i<allocated;i++)free(allocations[i].address);allocated=0;}
'''
        cases = {
            'HUD': r'''int main(void){points=3;tempo=10;drawAllInGame(NULL,0,NULL,0,NULL,0,NULL,0);assert(!strcmp(numbers[0],"3")&&!strcmp(numbers[1],"10"));release_objects();points=UINT_MAX;tempo=UINT_MAX;drawAllInGame(NULL,0,NULL,0,NULL,0,NULL,0);assert(!strcmp(numbers[2],"4294967295")&&!strcmp(numbers[3],"4294967295"));release_objects();return 0;}''',
            'postgame prompt': r'''int main(void){char prompt[]="INSERT NAME AAA THEN PRESS SPACE BAR";Objeto_texto *text=create_objeto_texto(prompt,150,600,ALIGN_LEFT,false);assert(!strcmp(text->string,prompt));assert(text->number_bitmaps==strlen(prompt));release_objects();return 0;}'''}
        for label, body in cases.items():
            with self.subTest(label=label):
                self.compile_and_run(COMMUNITY/'JoaoAlvaroFerreira--FEUP-LCOM/proj/src', [], harness+body)

    def test_mosu_score_path_accepts_basename_and_path_launches(self):
        self.compile_and_run(COMMUNITY/'Rockdell--2LCOM-MOsu/src', [], r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdint.h>
static char *buffer;
static const char *expected;
char *mosu_checked_strcpy(char *dest,const char *src){if(!buffer)buffer=dest;assert((uintptr_t)dest>=(uintptr_t)buffer);assert((uintptr_t)dest+strlen(src)+1<=(uintptr_t)buffer+200);memcpy(dest,src,strlen(src)+1);return dest;}
#undef strcpy
#define strcpy mosu_checked_strcpy
#define main mosu_program_entry
#include "mosu.c"
#undef main
#undef strcpy
void sef_startup(void){}
int sys_enable_iop(endpoint_t endpoint){assert(endpoint==SELF);return 0;}
int init_game(char *path){assert(strcmp(path,expected)==0);return 0;}
static void launch(char *program,const char *path){buffer=NULL;expected=path;char *argv[]={program,NULL};assert(mosu_program_entry(1,argv)==0);}
int main(void){launch("mosu","highscores.txt");launch("./mosu","./highscores.txt");launch("/home/lcom/mosu","/home/lcom/highscores.txt");return 0;}
''')

    def compile_and_run(self, project, sources, harness, source_flags=(), include_dirs=(), compiler_override=None):
        compiler = compiler_override or shlex.split(os.environ.get('CC', 'cc'))
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
            result = subprocess.run([*compiler, *flags, str(main), *objects, linker, '-lm', '-o', str(binary)],
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            result = subprocess.run([str(binary)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_luz_vbe_info_allocates_complete_bios_result(self):
        self.compile_and_run(COMMUNITY/'joaohluz--FEUP-LCOM/proj/src',['graphics.c'],r'''#include <lcom/lcf.h>
#include "graphics.h"
#include <assert.h>
static size_t allocated;
static vbe_mode_info_t result;
void *lm_alloc(size_t length,mmap_t *map){allocated=length;map->phys=0x10000;map->virt=&result;map->size=length;return map->virt;}
bool lm_free(const mmap_t *map){assert(map->virt==&result);return 0;}
int lcom_compat_legacy_int86(struct reg86u *regs){assert(regs->u.w.ax==0x4f01 && regs->u.w.cx==0x117);if(allocated<sizeof result)return 22;result.XResolution=1024;result.YResolution=768;result.BitsPerPixel=16;regs->u.w.ax=0x004f;return 0;}
int main(void){vbe_mode_info_t info={0};assert(vbe_info(0x117,&info)==0);assert(info.XResolution==1024 && info.YResolution==768 && info.BitsPerPixel==16);return 0;}
''')
    def test_paintmix_maps_framebuffer_bytes_not_pixel_bits(self):
        self.compile_and_run(COMMUNITY/'TitoGrine--LCOM_Project/projeto/src',['graphics.c'],r'''#include <lcom/lcf.h>
#include "graphics.h"
#include <assert.h>
static vbe_mode_info_t info;
static unsigned char pixels[1024*768*4];
void *lm_init(bool enable){return &info;}
void *lm_alloc(size_t length,mmap_t *map){assert(length==sizeof info);map->phys=0x10000;map->virt=&info;map->size=length;return map->virt;}
bool lm_free(const mmap_t *map){return true;}
int lcom_compat_legacy_int86(struct reg86u *regs){if(regs->u.w.ax==0x4f01){info.XResolution=1024;info.YResolution=768;info.BitsPerPixel=32;info.PhysBasePtr=0xe0000000;}else assert(regs->u.w.ax==0x4f02 && regs->u.w.bx==(0x144|BIT(14)));regs->u.w.ax=0x004f;return 0;}
int sys_privctl(endpoint_t ep,int req,void *arg){struct minix_mem_range *range=arg;assert(range->mr_limit-range->mr_base==sizeof pixels);return 0;}
void *vm_map_phys(endpoint_t ep,void *base,size_t length){assert(length==sizeof pixels);return pixels;}
void panic(const char *fmt,...){assert(0);}
int main(void){assert(vg_init(0x144)==pixels);return 0;}
''')


    def test_dave_score_reader_bounds_its_input_and_restores_valid_record(self):
        self.compile_and_run(COMMUNITY/'victorsnunes--FEUP-LCOM/proj', [], r'''#include "scoreboard.h"
#include <assert.h>
static unsigned reads;
static char *bounded_fgets(char *buffer,int size,FILE *file){assert(size<=50);reads++;return fgets(buffer,size,file);}
#define fgets bounded_fgets
#include "scoreboard.c"
#undef fgets
FILE *lcom_compat_fopen(const char *name,const char *mode){
 assert(strcmp(mode,"r")==0);FILE *file=tmpfile();assert(file);fputs("2147483647 ABCDEFGHIJ 31 12 99 23 59\n",file);rewind(file);return file;
}
int main(void){read_scores_from_file();assert(reads>=1);assert(record_points[0]==2147483647);assert(strcmp(record_names[0],"ABCDEFGHIJ")==0);assert(record_days[0]==31 && record_months[0]==12 && record_years[0]==99 && record_hours[0]==23 && record_minutes[0]==59);return 0;}
''')

    def test_whack_a_mole_flush_drains_ready_bytes_and_stops_when_empty(self):
        self.compile_and_run(COMMUNITY/'sofiagermer--WHACK-A-MOLE/src', ['serial_port.c'], r'''#include "serial_port.h"
#include "uart_const.h"
#include <assert.h>
static unsigned remaining,reads;
int util_sys_inb(int port,uint8_t *value){
 if(port==COM1_ADDR+LSR){*value=remaining?REC_DATA:0;return 0;}
 assert(port==COM1_ADDR+RBR && remaining);remaining--;reads++;*value=0x42;return 0;
}
uint32_t micros_to_ticks(uint32_t us){assert(0);return 0;}
int tickdelay(clock_t ticks){assert(0);return 0;}
int main(void){assert(ser_flush_rx()==0);assert(!reads);remaining=3;assert(ser_flush_rx()==0);assert(reads==3 && remaining==0);return 0;}
''')

    def test_bullet_man_rgb24_pixels_do_not_read_a_fourth_byte(self):
        self.compile_and_run(COMMUNITY/'JoaoCarlosPires--feup-lcom/proj/src', [], r'''#include "vbe.c"
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
uint16_t hres=2,vres=1,bits_per_pixel=24;
int main(void){
 size_t page=sysconf(_SC_PAGESIZE);uint8_t *mem=mmap(NULL,page*2,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);assert(mem!=MAP_FAILED);assert(!mprotect(mem+page,page,PROT_NONE));
 uint8_t *rgb=mem+page-3;rgb[0]=0x12;rgb[1]=0x34;rgb[2]=0x56;
 uint8_t target[8]={0};video_mem=(char *)target;
 assert(vg_draw_pixel(0,0,(uint32_t *)rgb)==0);assert(memcmp(target,rgb,3)==0);
 rgb[0]=0xff;rgb[1]=0x78;rgb[2]=0xff;target[3]=9;target[4]=9;target[5]=9;
 assert(vg_draw_pixel_transparent(1,0,(uint32_t *)rgb)==0);assert(target[3]==9 && target[4]==0x78 && target[5]==9);assert(target[6]==0);
 assert(!munmap(mem,page*2));return 0;
}''')

    def test_super_tux_background_wraps_rows_within_the_viewport(self):
        self.compile_and_run(COMMUNITY/'catarina03--FEUP-LCOM/proj/src', [], r'''#include "grafics.c"
#include <assert.h>
int main(void){
 uint16_t source[]={1,2,3,4,5,6,7,8,9,10};uint16_t output[10]={0};
 aux_buffer=(char *)output;x_res=4;y_res=2;bytes_per_pixel=2;
 Background background={0};background.sprite.width=5;background.sprite.height=2;background.sprite.bytes=(uint8_t *)source;background.x=4;
 const uint16_t expected[]={5,1,2,3,10,6,7,8};
 assert(draw_background(&background)==0);assert(memcmp(output,expected,sizeof expected)==0);assert(output[8]==0 && output[9]==0);
 memset(output,0,sizeof output);background.x=-1;
 assert(draw_background(&background)==0);assert(memcmp(output,expected,sizeof expected)==0);assert(output[8]==0 && output[9]==0);return 0;
}''')

    def test_lerbirinto_rtc_update_formats_complete_timestamp(self):
        self.compile_and_run(COMMUNITY/'bdmendes--feup-lcom/2020_og/proj/src', [], r'''#include "lib/devices/rtc/rtc.c"
#include <assert.h>
static uint8_t reg;
int sys_outb(int port,uint32_t data){assert(port==RTC_ADDR_REG);reg=data;return 0;}
int util_sys_inb(int port,uint8_t *value){
 assert(port==RTC_DATA_REG);
 const uint8_t bytes[]={0x56,0,0x34,0,0x12,0,0,0x16,0x10,0x26,0,0,RTC_REG_C_UF};
 assert(reg<sizeof bytes);*value=bytes[reg];return 0;
}
void delay_milli_seconds(unsigned delay){assert(0);}
uint8_t convert_bcd_to_binary(uint8_t value){return (value>>4)*10+(value&15);}
int main(void){
 const char expected[]="16/10/2026 12:34:56\n";
 assert(sizeof current_time>=sizeof expected);
 rtc_ih();assert(strcmp(current_time,expected)==0);return 0;
}''')

    def test_space_lander_rtc_update_polarity_matches_the_uip_bit(self):
        self.compile_and_run(COMMUNITY/'kiko-g--feup-lcom/proj/src', ['rtc.c'], r'''#include "rtc.h"
#include <assert.h>
static uint32_t value;
#undef sys_inb
int sys_outb(int port,uint32_t data){assert(port==RTC_ADDR_REG && data==RTC_A);return 0;}
int sys_inb(int port,uint32_t *data){assert(port==RTC_DATA_REG);*data=value;return 0;}
int main(void){
 value=0;assert(!rtc_is_updating());
 value=A_UIP;assert(rtc_is_updating());
 value=0x26;assert(!rtc_is_updating());return 0;
}''')

    def test_space_invaders_reis_highscore_dates_use_record_indices(self):
        self.compile_and_run(COMMUNITY/'DiogoOliveiraReis--FEUP-LCOM/proj/src', ['db.c'], r'''#include "db.h"
#include <assert.h>
#undef fopen
#undef remove
int score = 130;
FILE *lcom_compat_fopen(const char *path, const char *mode) {
    return fopen(strrchr(path, '/') + 1, mode);
}
int getDate(unsigned *d,unsigned *m,unsigned *y){*d=16;*m=10;*y=2026;return 0;}
int getTime(unsigned *h,unsigned *m,unsigned *s){*h=12;*m=0;*s=0;return 0;}
int main(void) {
    char directory[]="/tmp/lcom-reis-score-XXXXXX";assert(mkdtemp(directory));assert(chdir(directory)==0);
    FILE *f=fopen("highscores.txt","w");assert(f);
    fputs("120\n100\n80\n50\n50\n30\n30\n10\n10\n10\n",f);fclose(f);
    f=fopen("scores.txt","w");assert(f);
    for(int i=0;i<10;i++)fputs("16 10 2026\n",f);fclose(f);
    assert(writeHighscore()==0);
    const int expected[]={130,120,100,80,50,50,30,30,10,10};
    f=fopen("highscores.txt","r");assert(f);
    for(int i=0;i<10;i++){int actual=-1;assert(fscanf(f,"%d",&actual)==1);assert(actual==expected[i]);}
    fclose(f);remove("highscores.txt");remove("scores.txt");assert(chdir("/")==0);assert(rmdir(directory)==0);return 0;
}''', source_flags=['-fsanitize=bounds','-fsanitize-undefined-trap-on-error'])

    def test_euler_mouse_commands_consume_acks_before_first_cursor_packet(self):
        self.compile_and_run(COMMUNITY/'MurielPinho--FEUP-LCOM-1920/proj/src',
                             ['ps2.c','mouse.c','menu.c'],r'''#include "ps2.h"
#include "sprite.h"
#include "mouse.h"
#include "menu.h"
#include "macros.h"
#include <assert.h>
static uint8_t queue[64];static int count,index_,mouse_command,commands,retry_next,silent;
extern Mouse *mouse;extern MenuObject *menu;
extern uint8_t mouseData;
int util_sys_inb(int port,uint8_t *value){
 if(port==KBC_STAT_REG){*value=index_<count?(OBF|BIT(5)):0;return 0;}
 if(port==KBC_OUT_BUF){assert(index_<count);*value=queue[index_++];return 0;}
 assert(0);return 1;
}
int sys_outb(int port,uint32_t value){
 if(port==KBC_CMD_REG){mouse_command=value==WRITE_MOUSE_BYTE;return 0;}
 if(port==KBC_IN_BUF && mouse_command){commands++;mouse_command=0;if(!silent)queue[count++]=retry_next?(retry_next=0,NACK):ACK;return 0;}
 return 0;
}
uint32_t micros_to_ticks(uint32_t us){return 0;}
int tickdelay(clock_t ticks){return 0;}
uint8_t minix_get_dflt_kbc_cmd_byte(void){return 0x47;}
int main(void){
 assert(mouse_enable_data_report()==0);assert(commands==2 && index_==count);
 Sprite cursor={.x=400,.y=300,.width=40,.height=40};Mouse pointer={.sprite=&cursor};mouse=&pointer;
 MenuObject selection={.mState=Selecting,.menuOption=1};menu=&selection;
 /* The very first movement packet must move by all7/-9 units. */
 uint8_t packet[]={0x18,0xF9,9};
 for(int i=0;i<3;i++){mouseData=packet[i];if(mouse_evaluate_bytes())handleMouse();}
 assert(cursor.x==393 && cursor.y==291 && !pointer.lb_status);
 /* Reset also sends commands, so a menu/game round-trip must leave no ACK. */
 assert(mouse_reset()==0 && commands==4 && index_==count);
 retry_next=1;assert(mouse_enable_data_report()==0 && commands==7 && index_==count);
 silent=1;assert(mouse_enable_data_report()!=0 && commands<=10);return 0;
}''')

    def test_portal_cursor_frame_restores_the_previous_inset_origin(self):
        project=COMMUNITY/'BrunoRosendo--Portal2D/proj/Src'
        self.compile_and_run(project,['sprites.c'],r'''#include "$GAME_SOURCE$"
#include <assert.h>
static uint16_t frame[48];
unsigned get_h_res(void){return 8;}unsigned get_v_res(void){return 6;}
enum xpm_image_type get_type(void){return XPM_5_6_5;}
uint32_t xpm_transparency_color(enum xpm_image_type t){return 0xffff;}
int vg_draw_pixel(uint16_t x,uint16_t y,uint16_t c){assert(x<8 && y<6);frame[y*8+x]=c;return 0;}
int vg_draw_from_background(uint16_t x,uint16_t y){if(x<8&&y<6)frame[y*8+x]=0;return 0;}
void draw_time(Sprite* sprites){memset(sprites,0,19*sizeof(Sprite));}
void buffer_screen(void){}void flip_page(void){}
void load_background(void){abort();}void erase_everything(void){abort();}
void stash_RBR(void){abort();}
void draw_menu(void){abort();}void erase_menu(void){abort();}int exit_menu(void){abort();}
FILE *lcom_compat_fopen(const char*p,const char*m){abort();}
void animate_asprite(ASprite *a){abort();}
void destroy_asprite(ASprite *a){abort();}
ASprite* create_portal(portal_type t,int x,int y){abort();}
uint8_t *xpm_load(xpm_map_t p,enum xpm_image_type t,xpm_image_t *i){abort();}
int main(void){
 uint16_t colors[]={0xffff,0x1234,0x2345,0x3456,0x4567,0x5678};
 old_cursor=(Sprite){.x=7,.y=5,.width=3,.height=2,.map=(uint8_t*)colors};draw_cursor(&old_cursor);
 assert(frame[4*8+6]==0x1234 && frame[5*8+7]==0x5678);
 Sprite next={.x=1,.y=1,.width=3,.height=2,.map=(uint8_t*)colors};cursor=&next;game_state=MENU;
 draw_new_frame();
 /* Old edge glyph must be completely gone, new glyph must remain. */
 for(int y=4;y<6;y++)for(int x=5;x<8;x++)assert(frame[y*8+x]==0);
 assert(frame[1*8+2]==0x1234 && frame[2*8+3]==0x5678);
 assert(next.x==1 && next.y==1);return 0;
}'''.replace('$GAME_SOURCE$',str(project/'game.c')))

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
            build = subprocess.run([str(cli), 'compat', 'build', str(project), '--output', str(folder / 'native')],
                                   capture_output=True, text=True, timeout=120)
            self.assertEqual(build.returncode, 0, build.stdout + build.stderr)
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
int (sys_int86)(reg86_t *reg) {return 0;}
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
int (sys_int86)(reg86_t *regs) { abort(); }
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


    def test_galactic_menu_precedes_first_guest_input(self):
        self.compile_and_run(COMMUNITY/'davidrsfalcao--LCOM-projeto/src',['galatic_war.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
static unsigned started, graphics, subscribed, menus, exited, unsubscribed;
int khook_id=1;
extern char irq_set;
int galactic_student_entry(void);
void sef_startup(void){started++;}
void *vg_init(unsigned short mode){assert(started==1&&mode==0x105);graphics++;return &graphics;}
int kbd_subscribe_int(void){assert(graphics==1);subscribed++;return 0;}
int driver_receive(endpoint_t who,message*m,int*status){(void)who;(void)m;(void)status;assert(0&&"The original menu must be entered before waiting for an unprompted first key");return 0;}
int kbd_scan_c(int *key){(void)key;assert(0&&"Startup must not discard a guest key");return 0;}
int menu(void){assert(started==1&&graphics==1&&subscribed==1&&irq_set==BIT(1));menus++;return 1;}
int game(void){assert(0&&"Choosing Exit must not start a round");return 1;}
int vg_exit(void){assert(menus==1);exited++;return 0;}
int timer_unsubscribe_int(void){assert(exited==1);unsubscribed++;return 0;}
int kbd_unsubscribe_int(void){assert(unsubscribed==1);unsubscribed++;return 0;}
int main(void){assert(galactic_student_entry()==0);assert(menus==1&&exited==1&&unsubscribed==2);return 0;}
''',source_flags=['-Dmain=galactic_student_entry','-DLCOM_COMPAT_LEGACY=1','-Wno-implicit-function-declaration'])

    def test_galactic_xpm_renderer_uses_own_buffer_declarations(self):
        self.compile_and_run(COMMUNITY/'davidrsfalcao--LCOM-projeto/src',['vbe.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "vbe.h"
static char pixels[16];char*getSBuffer(void){return pixels;}unsigned getHRes(void){return 4;}
int main(void){char*map[]={"2 2 2","a 1","b 2","ab","ba"};int w=0,h=0;memset(pixels,7,sizeof pixels);assert(xpm_cre(&h,&w,1,1,map)==0);assert(w==2&&h==2);assert(pixels[5]==1&&pixels[6]==2&&pixels[9]==2&&pixels[10]==1);assert(xpm_del(&h,&w,1,1)==0);assert(!pixels[5]&&!pixels[6]&&!pixels[9]&&!pixels[10]);assert(pixels[0]==7&&pixels[15]==7);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Wno-implicit-function-declaration'])

    def test_galactic_vg_init_returns_mapped_virtual_buffer(self):
        self.compile_and_run(COMMUNITY/'davidrsfalcao--LCOM-projeto/src',['video_gr.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "video_gr.h"
static char framebuffer[1024*768];
void*lcom_compat_legacy_lm_init(void){return (void*)0x1000;}
int vbe_set_mode(unsigned short function,unsigned short mode){assert(function==0x4f02&&mode==0x105);return 0;}
int sys_privctl(endpoint_t endpoint,int request,void*arg){struct mem_range*r=arg;assert(r->mr_base==0xe0000000u&&r->mr_limit-r->mr_base==sizeof framebuffer);return 0;}
void*vm_map_phys(endpoint_t endpoint,void*address,size_t size){assert((uintptr_t)address==0xe0000000u&&size==sizeof framebuffer);return framebuffer;}
int lcom_compat_legacy_int86(struct reg86u*r){return 0;}void panic(const char*format,...){abort();}
int main(void){void*mapped=vg_init(0x105);assert(mapped==framebuffer&&getVideoMem()==framebuffer);assert(getSBuffer()!=NULL);free(getSBuffer());return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Wno-implicit-function-declaration'])

    def test_galactic_entity_renderers_forward_xpm_line_arrays(self):
        self.compile_and_run(COMMUNITY/'davidrsfalcao--LCOM-projeto/src',['space_ship.c','asteroid.c','cursor.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#pragma GCC diagnostic error "-Wincompatible-pointer-types"
#include "space_ship.h"
#include "asteroid.h"
#include "cursor.h"
static char*map[]={"1 1 1","a 3","a"};static unsigned drawn;
int xpm_cre(int*h,int*w,unsigned short x,unsigned short y,char**sprite){assert(sprite==map);assert(x==(drawn==0?40:drawn==1?60:80)&&y==(drawn==0?70:drawn==1?80:90));*h=*w=1;drawn++;return 0;}
int main(void){space_ship ship={40,70,0,0,map};asteroid first={60,80,0,0,map},hidden={60,0,0,0,map};draw_spaceship(ship);draw_asteroids(first,hidden);cursor pointer={80,90,0,0,0,map};draw_cursor(pointer);assert(drawn==3);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Werror=implicit-function-declaration'])

    def test_galactic_each_xpm_redraw_releases_decoded_pixels(self):
        self.compile_and_run(COMMUNITY/'davidrsfalcao--LCOM-projeto/src',['vbe.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "vbe.h"
static char framebuffer[16];static void*allocation;static unsigned allocated,released;
void*tracked_malloc(size_t n){assert(!allocation);allocation=malloc(n);assert(allocation);allocated++;return allocation;}
void tracked_free(void*p){assert(p==allocation);free(p);allocation=NULL;released++;}
char*getSBuffer(void){return framebuffer;}unsigned getHRes(void){return 4;}
int main(void){char*map[]={"2 2 2","a 1","b 2","ab","ba"};for(unsigned frame=0;frame<50;frame++){int w=0,h=0;assert(xpm_cre(&h,&w,1,1,map)==0);assert(!allocation);assert(framebuffer[5]==1&&framebuffer[6]==2&&framebuffer[9]==2&&framebuffer[10]==1);}assert(allocated==50&&released==50);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmalloc=tracked_malloc','-Dfree=tracked_free','-Wno-implicit-function-declaration'])

    def test_pool_float_constants_and_actual_pockets_and_wall_bounce(self):
        self.compile_and_run(COMMUNITY/'BSonnemberg--LCOM/proj/src',['pool.c', 'physics.c', 'vector2.c'],r'''#include <lcom/lcf.h>
#include <float.h>
#include <assert.h>
#include "pool.h"
int main(void){ball pockets[6];memset(pockets,0x55,sizeof pockets);game.pockets=pockets;setup_pockets();double xs[]={45,45,399,399,758,758},ys[]={154,552,154,552,154,552};for(unsigned i=0;i<6;i++){assert(pockets[i].mass==FLT_MAX&&pockets[i].radius==6&&pockets[i].friction_coef==0);assert(pockets[i].pos.x==xs[i]&&pockets[i].pos.y==ys[i]);assert(pockets[i].force.x==0&&pockets[i].force.y==0&&pockets[i].acc.x==0&&pockets[i].acc.y==0&&pockets[i].vel.x==0&&pockets[i].vel.y==0);}bool hit=false;ball moving={.pos={50,300},.vel={-20,0},.radius=20};handle_wallcollision(&moving,&hit);assert(hit&&moving.pos.x==62&&moving.pos.y==300&&moving.vel.x==20);hit=false;ball centred={.pos={300,300},.radius=20};handle_wallcollision(&centred,&hit);assert(!hit&&centred.pos.x==300&&centred.pos.y==300);return 0;}
''',source_flags=['-O2'])

    def test_pool_first_controller_irq_is_received_before_esc_loop_test(self):
        self.compile_and_run(COMMUNITY/'BSonnemberg--LCOM/proj/src',['controller.c'],r'''#include "controller.h"
#include <assert.h>
static unsigned received, parsed;
int timer_subscribe_int(uint8_t*p){*p=0;return 0;}
int mouse_subscribe_int(uint8_t*p){*p=1;return 0;}
int kbd_subscribe_int(uint8_t*p){*p=2;return 0;}
int timer_set_frequency(uint8_t a,uint32_t b){(void)a;(void)b;return 0;}
int driver_receive(int endpoint,message*m,int*status){assert(endpoint==ANY); assert(++received==1);m->m_source=HARDWARE;m->m_notify.interrupts=BIT(2);*status=IPC_STATUS_NOTIFY;return 0;}
void kbc_ih(void){}
int kbd_parse_scancode(scancode*s){parsed++;s->buf[0]=ESC_BREAK;s->make=false;return 0;}
int main(void){assert(subscribe_all_int()==0);init_controller();assert(received==1 && parsed==1);return 0;}

uint16_t refresh_time;
void get_shot_force_dir(void){abort();}void handle_event(void){abort();}
int kbd_unsubscribe_int(void){abort();}int mouse_unsubscribe_int(void){abort();}
void render_about_view(void){abort();}void render_game_view(void){abort();}void render_menu_view(void){abort();}
int timer_unsubscribe_int(void){abort();}void update_physics(void){abort();}void mouse_ih(void){abort();}
int mouse_parse_packet(struct packet*p){(void)p;abort();}void timer_int_handler(void){abort();}
''',source_flags=['-O2'])

    def test_classic_whack_mouse_ack_waits_until_obf_and_bounds_timeout(self):
        self.compile_and_run(COMMUNITY/'SergioCarvalhais02--LCOM-proj',['mouse.c'],r'''#include "mouse.h"
#include <assert.h>
static unsigned status_reads,data_reads,delays;static int ready_after;
int util_sys_inb(int port,uint8_t*v){if(port==KBC_ST_REG){*v=(int)++status_reads>ready_after?KBC_ST_OBF:0;return 0;}assert(port==KBC_OUT_BUF);assert((int)status_reads>ready_after);data_reads++;*v=MOUSE_ACK;return 0;}
uint32_t micros_to_ticks(uint32_t micros){assert(micros>0);return 1;}
int tickdelay(clock_t ticks){assert(ticks>0);delays++;return 0;}
int sys_irqsetpolicy(int a,int b,int*c){(void)a;(void)b;(void)c;return 0;}
int sys_irqrmpolicy(int*a){(void)a;return 0;}
int sys_outb(int a,uint32_t b){(void)a;(void)b;return 0;}
int kbc_issue_cmd(uint8_t a){(void)a;return 0;}
int main(void){uint8_t ack=0;ready_after=2;assert(get_mouse_ack(&ack)==0);assert(ack==MOUSE_ACK && status_reads==3 && data_reads==1 && delays==2);status_reads=data_reads=delays=0;ready_after=99;assert(get_mouse_ack(&ack)==1);assert(status_reads==3 && data_reads==0 && delays==3);return 0;}
''',source_flags=['-O2'])

    def test_wordlcom_dictionary_owns_heap_words_and_rejects_outside_range(self):
        project=COMMUNITY/'gwealm--FEUP-LCOM/proj/src'
        self.compile_and_run(project,['game/wordlist.c','vector/vector.c','utils.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "game/wordlist.h"
FILE *lcom_compat_fopen(const char*name,const char*mode){assert(!strcmp(name,"/home/lcom/labs/proj/src/wordlist.txt")&&!strcmp(mode,"r"));FILE*f=tmpfile();assert(f);fputs("aback\nabase\n",f);rewind(f);return f;}
/* Two words fit the original16-slot vector. No reallocation should be needed. */
int reallocarr(void*p,size_t n,size_t size){(void)p;(void)n;(void)size;abort();}
int main(void){assert(game_wordlist_init("original argument")==0);assert(game_wordlist_contains("aback"));assert(game_wordlist_contains("abase"));assert(!game_wordlist_contains("aaaaa"));assert(!game_wordlist_contains("abacl"));assert(!game_wordlist_contains("zzzzz"));assert(game_wordlist_cleanup()==0);return 0;}
''',source_flags=['-O1'],include_dirs=[project/'include'])

    def test_pong_value_fields_initialize_ui_entities_and_digits(self):
        project=COMMUNITY/'IMS-up2021--FEUP_LCOM/proj/src'
        self.compile_and_run(project,['ui.c','entity.c','draw.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "ui.h"
#include "entity.h"
#include "draw.h"
extern Digit digits[10];
static unsigned loaded;
uint8_t*xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*image){assert(map&&type==XPM_INDEXED);uint8_t*p=malloc(6);assert(p);memset(p,loaded++,6);*image=(xpm_image_t){.width=3,.height=2,.size=6,.bytes=p};return p;}
static void clicked(void){}
int main(void){static const char*const tiny[]={"3 2 1",". 7","...","..."};Button*b=create_button(10,20,(xpm_map_t)tiny,clicked);assert(b&&b->pos_x==10&&b->pos_y==20&&b->width==3&&b->height==2&&b->on_click==clicked);Cursor*c=create_cursor(30,40,(xpm_map_t)tiny);assert(c&&c->pos_x==30&&c->pos_y==40&&c->width==3&&c->height==2);Ball*ball=create_ball((xpm_map_t)tiny);assert(ball&&ball->x_pos==512&&ball->y_pos==384&&ball->width==3&&ball->height==2&&ball->radius==32&&ball->vX==5&&ball->vY==-5);Wall*top=create_wall(true);Wall*bottom=create_wall(false);assert(top&&bottom&&top->y_pos==0&&bottom->y_pos==758&&top->width==1024&&bottom->height==10);Player*p1=create_player(1);Player*p2=create_player(2);assert(p1&&p2&&p1->number==1&&p2->number==2&&p1->x_pos==10&&p2->x_pos==1004&&p1->height==100);unsigned before=loaded;init_digits();assert(loaded==before+12);for(unsigned i=0;i<10;i++){assert(digits[i].width==3&&digits[i].height==2&&digits[i].sprite[0]==before+i);free((void*)digits[i].sprite);}free((void*)b->sprite);free(b);free((void*)c->sprite);free(c);free(ball->sprite);free(ball);free(top);free(bottom);free(p1);free(p2);return 0;}
''',source_flags=['-O2'])

    def test_wordlcom_graphics_cleanup_releases_each_owned_image_once(self):
        project=COMMUNITY/'gwealm--FEUP-LCOM/proj/src'
        self.compile_and_run(project,['graphics/graphics.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "graphics/graphics.h"
#include "events/events.h"
#include "state/state.h"
static unsigned loads, freed_images, freed_arrays;
static void* loaded_images[45];
void observed_free(void*p){bool image=false;for(unsigned i=0;i<45;i++)if(loaded_images[i]==p){assert(p);loaded_images[i]=NULL;image=true;break;}if(image)freed_images++;else freed_arrays++;free(p);}
void game_draw(void){abort();}void menu_draw(void){abort();}state_t state_get(void){abort();}
uint16_t mouse_get_x(void){abort();}uint16_t mouse_get_y(void){abort();}
int vbe_fill(uint16_t color){(void)color;abort();}int vbe_flush(void){abort();}
int vbe_draw_xpm(uint16_t x,uint16_t y,xpm_image_t*image){(void)x;(void)y;(void)image;abort();}

uint8_t*xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*image){assert(map&&type==XPM_5_6_5);uint8_t*p=malloc(2);assert(p);memset(p,7,2);*image=(xpm_image_t){.width=1,.height=1,.size=2,.bytes=p};loaded_images[loads++]=p;return p;}
void event_on(enum event_type type,void(*callback)(struct event*)){assert(type==TIMER_TICK&&callback);}
void event_unregister(enum event_type type,void(*callback)(struct event*)){assert(type==TIMER_TICK&&callback);}
int main(void){assert(graphics_init()==0);assert(loads==45);assert(graphics_cleanup()==0);assert(freed_images==45&&freed_arrays==2);return 0;}
''',source_flags=['-O1','-Dfree=observed_free'],include_dirs=[project/'include'])

    def test_daedalus_live_movement_advances_coordinates_before_outer_collision_check(self):
        self.compile_and_run(COMMUNITY/'josealeixopc--LCOM-FEUP/proj/code',['sprite.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "sprite.h"
int main(void){Sprite sprite={.xi=60,.yi=600,.xf=100,.yf=655,.width=40,.height=55,.xspeed=10,.yspeed=0};Doll doll={.sprite=&sprite};moveDoll(&doll,0.1f);assert(sprite.xi==61&&sprite.yi==600&&sprite.xf==101&&sprite.yf==655);sprite.xspeed=0;sprite.yspeed=-10;moveDoll(&doll,0.1f);assert(sprite.xi==61&&sprite.yi==599&&sprite.xf==101&&sprite.yf==654);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Werror=return-type'])

    def test_commodore_public_mode_initializer_maps_and_fills_the_original_framebuffer(self):
        self.compile_and_run(COMMUNITY/'pedro93--LCOM/src',['video_gr.c'],'#define LCOM_COMPAT_LEGACY 1\n#include <lcom/compat.h>\n#include <assert.h>\n#include "video_gr.h"\n#include "vbe.h"\nstatic struct { unsigned char before[8],frame[1024*768],after[8]; } guarded;\n#define frame guarded.frame\nstatic unsigned mapped,set;\nint lcom_compat_legacy_int86(struct reg86u*r){assert(r->u.b.intno==0x10 && r->u.w.ax==0x4f02 && r->u.w.bx==(0x4000|0x105));set++;return 0;}\nint vbe_get_mode_info(unsigned short mode,vbe_mode_info_t*i){assert(mode==0x105);memset(i,0,sizeof *i);i->XResolution=1024;i->YResolution=768;i->BitsPerPixel=8;i->PhysBasePtr=0xe0000000u;return 0;}\nint sys_privctl(endpoint_t ep,int req,void*arg){struct mem_range*r=arg;assert(r->mr_base==0xe0000000u&&r->mr_limit-r->mr_base==sizeof frame);return 0;}\nvoid*vm_map_phys(endpoint_t ep,void*phys,size_t size){assert((uintptr_t)phys==0xe0000000u&&size==sizeof frame);mapped++;return frame;}\nvoid panic(const char*fmt,...){abort();}\nint main(void){memset(&guarded,0x5a,sizeof guarded);vg_init(0x105);assert(mapped==1&&set==1);vg_fill(37);assert(frame[0]==37&&frame[sizeof frame-1]==37);assert(vg_set_pixel(2,1,30)==0);for(size_t i=0;i<sizeof frame;i++)assert(frame[i]==37);unsigned failures=0;assert(vg_set_pixel(0,0,12)==0);if(frame[0]!=12||frame[1]!=37)failures|=1;assert(vg_set_pixel(1023,767,42)==0);if(frame[sizeof frame-1]!=42)failures|=2;if(vg_set_pixel(1024,0,19)!=-1||vg_set_pixel(0,768,21)!=-1)failures|=4;for(size_t i=0;i<8;i++)if(guarded.before[i]!=0x5a||guarded.after[i]!=0x5a)failures|=8;if(failures)fprintf(stderr,"pixel failures first=1,last=2,edge=4,canary=8: %u\\n",failures);assert(failures==0);return 0;}\n',source_flags=['-DLCOM_COMPAT_LEGACY=1','-include',str(COMMUNITY/'pedro93--LCOM/src/video_gr.h')])

    def test_daedalus_sentence_uses_declared_glyph_renderer(self):
        self.compile_and_run(COMMUNITY/'josealeixopc--LCOM-FEUP/proj/code',['sprite.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "sprite.h"
#include "video_gr.h"
int getIntFromRGB(int r,int g,int b){assert(r==224&&g==192&&b==64);return 0x1234;}
static Bitmap image;static unsigned loaded,drawn,deleted;
Bitmap*loadBitmap(const char*path){const char*name=strrchr(path,'/');assert(name&&!strcmp(name+1,loaded++==0?"0.bmp":"1.bmp"));return &image;}
void drawBitmapIg(Bitmap*b,int x,int y,Alignment a,int color){assert(b==&image&&a==ALIGN_LEFT&&color==0x1234);assert(x==(drawn==0?10:50)&&y==(drawn==0?20:21));drawn++;}
void deleteBitmap(Bitmap*b){assert(b==&image);deleted++;}
int main(void){char text[]="0 1";writeSentece(10,20,text);assert(loaded==2&&drawn==2&&deleted==2);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Wno-implicit-function-declaration'])

    def test_outpost_native_pixel_writer_preserves_high_addresses_and_pixel_widths(self):
        project=COMMUNITY/'Ptozin--2L.EIC018-LCOM/proj/src'
        self.compile_and_run(project,[],'#include <lcom/lcf.h>\n#include <assert.h>\n#include <stdint.h>\n#include <stdlib.h>\n#include <string.h>\nstatic uint8_t*expected;\nstatic void*observed_copy(void*destination,const void*source,size_t size){assert(destination==expected&&"pixel writer truncated its actual framebuffer pointer");uint8_t*d=destination;const uint8_t*s=source;for(size_t i=0;i<size;i++)d[i]=s[i];return destination;}\n#define memcpy observed_copy\n#include "libraries/graphics/src/graphics.c"\n#undef memcpy\nint main(void){uint8_t*allocation=malloc(64);assert(allocation);\n#if UINTPTR_MAX>0xffffffffu\nassert((uintptr_t)allocation>0xffffffffu);\n#endif\nfor(unsigned test=0;test<2;test++){unsigned bytes=test?3:4;memset(allocation,0x5a,64);double_buffer=allocation+8;vbe_mem_info=(vbe_mode_info_t){.XResolution=3,.YResolution=2,.BitsPerPixel=bytes*8};expected=double_buffer+4*bytes;assert(set_pixel(1,1,0x11223344u)==0);for(unsigned i=0;i<bytes;i++)assert(expected[i]==(uint8_t)(0x11223344u>>(i*8)));expected=double_buffer+5*bytes;assert(set_pixel(2,1,0xaabbccddu)==0);for(unsigned i=0;i<bytes;i++)assert(expected[i]==(uint8_t)(0xaabbccddu>>(i*8)));for(unsigned i=0;i<8+4*bytes;i++)assert(allocation[i]==0x5a);for(unsigned i=8+6*bytes;i<64;i++)assert(allocation[i]==0x5a);}free(allocation);return 0;}\n\n',include_dirs=sorted({p.parent for p in project.rglob('*.h')}))

    def test_daedalus_new_game_starts_with_released_direction_keys(self):
        self.compile_and_run(COMMUNITY/'josealeixopc--LCOM-FEUP/proj/code',['Daedalus.c','sprite.c'],'#define LCOM_COMPAT_LEGACY 1\n#include <lcom/compat.h>\n#include <assert.h>\n#include <stdbool.h>\n#include "Daedalus.h"\nvoid*poison_malloc(size_t n){void*p=malloc(n);assert(p);memset(p,0xa5,n);return p;}\nint getIntFromRGB(int r,int g,int b){return (r<<11)|(g<<5)|b;}\nstatic Bitmap image={.bitmapInfoHeader={.width=40,.height=55}};\nBitmap*loadBitmap(const char*path){assert(path);return &image;}\nint rtc_subscribe(void){return 256;}int timer_subscribe_int(void){return 1;}int kbc_subscribe(void){return 2;}\nvoid*lcom_compat_legacy_lm_init(void){return &image;}\nDate*createDate(void){static Date d={.day=17,.month=10,.year=2026};return &d;}\nvoid*vg_init(unsigned short mode){assert(mode==0x117);return &image;}\nCrosshair*newCrosshair(void){static Crosshair c;return &c;}\nMaze*createMaze(const char*path,int x,int y){static Maze maps[2];static unsigned count;assert(count<2);maps[count]=(Maze){.xiPos=x,.yiPos=y};return &maps[count++];}\nint main(void){Daedalus*d=initDaedalus();assert(d);for(unsigned i=0;i<sizeof d->activeKeys;i++)assert(d->activeKeys[i]==0&&"fresh game must not inherit arbitrary held keys from malloc");d->pressed_key=KEY_D;updateDaedalusDoll(d);assert(d->doll->sprite->xspeed==10&&d->doll->sprite->yspeed==0);moveDoll(d->doll,0.1f);assert(d->doll->sprite->xi==61&&d->doll->sprite->yi==600);d->pressed_key=BREAK_KEY_D;updateDaedalusDoll(d);assert(d->doll->sprite->xspeed==0&&d->doll->sprite->yspeed==0);d->pressed_key=KEY_W;updateDaedalusDoll(d);assert(d->doll->sprite->xspeed==0&&d->doll->sprite->yspeed==-10);moveDoll(d->doll,0.1f);assert(d->doll->sprite->xi==61&&d->doll->sprite->yi==599);free(d->doll->sprite);free(d->doll);free(d);return 0;}\n',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmalloc=poison_malloc','-O2'])

    def test_daedalus_crosshair_allocates_and_releases_its_complete_owned_graph(self):
        self.compile_and_run(COMMUNITY/'josealeixopc--LCOM-FEUP/proj/code',['mouse.c','sprite.c'],'#define LCOM_COMPAT_LEGACY 1\n#include <lcom/compat.h>\n#include <assert.h>\n#include <stdbool.h>\n#include "mouse.h"\nstruct allocation {uint8_t*p;size_t size;bool freed;};static struct allocation blocks[8];static unsigned allocated,images_released;\nvoid*guard_malloc(size_t n){assert(allocated<8);uint8_t*p=malloc(n+32);assert(p);memset(p,0x5a,n+32);blocks[allocated++]=(struct allocation){p,n,false};return p;}\nvoid guard_free(void*p){unsigned i;for(i=0;i<allocated;i++)if(blocks[i].p==p)break;assert(i<allocated&&!blocks[i].freed);for(unsigned j=0;j<32;j++)assert(blocks[i].p[blocks[i].size+j]==0x5a&&"constructor wrote past its requested object allocation");blocks[i].freed=true;free(p);}\nint getIntFromRGB(int r,int g,int b){return (r<<11)|(g<<5)|b;}\nstatic Bitmap image={.bitmapInfoHeader={.width=4,.height=3}};\nBitmap*loadBitmap(const char*path){assert(strstr(path,"Crosshair.bmp"));return &image;}\nvoid deleteBitmap(Bitmap*p){assert(p==&image&&images_released++==0);}\nint main(void){Crosshair*c=newCrosshair();assert(c&&c->packet&&c->sprite&&allocated==3);assert(c->sprite->width==4&&c->sprite->height==3);for(unsigned i=0;i<allocated;i++)for(unsigned j=0;j<32;j++)assert(blocks[i].p[blocks[i].size+j]==0x5a&&"constructor wrote past its requested object allocation");destroyCrosshair(c);assert(images_released==1&&"crosshair must release its owned sprite bitmap");for(unsigned i=0;i<allocated;i++)assert(blocks[i].freed&&"crosshair must release every owned allocation once");return 0;}\n',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmalloc=guard_malloc','-Dfree=guard_free','-O2'])


    def test_elemental_mouse_setup_reads_ack_before_deciding_to_retry(self):
        self.compile_and_run(COMMUNITY/'joaooliveira-11--feup-lcom/proj/src',['devices/mouse/mouse.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "devices/mouse/mouse.h"\nstatic unsigned writes,reads,delays;\nint write_KBC_INPUT(uint8_t port,uint8_t value){assert(port==(writes%2?0x60:0x64));assert(value==(writes%2?0xf4:0xd4));writes++;return 0;}\nint util_sys_inb(int port,uint8_t*value){assert(port==0x60&&writes==2*(reads+1));*value=reads++?0xfa:0xfe;return 0;}\nuint32_t micros_to_ticks(uint32_t us){assert(us==20000);return 2;}\nint tickdelay(clock_t ticks){assert(ticks==2);delays++;return 0;}\nint main(void){assert(mouse_write_input(0xf4)==0);assert(writes==4&&reads==2&&delays==1);return 0;}\n',source_flags=['-O2'])


    def test_tower_engineering_setup_consumes_ack_before_first_movement_packet(self):
        self.compile_and_run(COMMUNITY/'PedroPF1234--Tower-Engineering-LCOM-23-24/proj/src',['Devices/Mouse/mouse.c','Devices/KeyBoard/kbc.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "Devices/Mouse/mouse.h"\nstatic uint8_t bytes[16];static unsigned head,tail,writes;\nint util_sys_inb(int port,uint8_t*out){if(port==0x64){*out=head<tail?0x21:0;return 0;}assert(port==0x60&&head<tail);*out=bytes[head++];return 0;}\nint sys_outb(int port,uint32_t value){if(writes++==0){assert(port==0x64&&value==0xd4);}else{assert(port==0x60&&value==0xf4);bytes[tail++]=0xfa;}return 0;}\nint sys_irqsetpolicy(int irq,int policy,int*id){assert(irq==12&&*id==2);*id=1;return 0;}\nuint32_t micros_to_ticks(uint32_t us){return 1;}\nint tickdelay(clock_t ticks){assert(!"controller is ready without delay");return 1;}\nint main(void){uint8_t bit;assert(mouse_initiate(&bit)==0&&bit==2);bytes[tail++]=0x28;bytes[tail++]=0;bytes[tail++]=0xe7;for(unsigned i=0;i<3;i++)mouse_ih();struct packet p;assert(mouse_get_info(&p));assert(p.bytes[0]==0x28&&p.delta_x==0&&p.delta_y==-25&&!p.lb&&"command ACK must not become the first movement packet header");assert(head==tail);return 0;}\n',source_flags=['-O2'])

    def test_tower_engineering_renderer_retains_every_object_once_in_depth_order(self):
        self.compile_and_run(COMMUNITY/'PedroPF1234--Tower-Engineering-LCOM-23-24/proj/src',['GameObjects/gameobject.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "GameObjects/gameobject.h"\nstatic int drawn[16];static unsigned count;\nint draw_sprite(Sprite*s){assert(count<16);drawn[count++]=s->x;return 0;}\nint main(void){init_render_pipeline();Sprite a={0},b={0},c={0};GameObject*first=create_gameobject_from_sprite(&a,101,10,0,0,10);renderGameObjects();assert(count==1&&drawn[0]==101&&"first object must appear once");GameObject*last=create_gameobject_from_sprite(&b,202,20,0,0,20);GameObject*front=create_gameobject_from_sprite(&c,303,30,0,0,5);count=0;renderGameObjects();assert(count==3&&drawn[0]==303&&drawn[1]==101&&drawn[2]==202&&"insertion must retain every object in depth order");hideGameObject(first);count=0;renderGameObjects();assert(count==2&&drawn[0]==303&&drawn[1]==202);destroy_gameobject_safe_sprite(first);destroy_gameobject_safe_sprite(last);destroy_gameobject_safe_sprite(front);count=0;renderGameObjects();assert(count==0);return 0;}\n',source_flags=['-O2'])

    def test_tower_engineering_menu_replacement_keeps_rendered_artwork_alive(self):
        self.compile_and_run(COMMUNITY/'PedroPF1234--Tower-Engineering-LCOM-23-24/proj/src',['Game/Menu/menu.c','Game/DataStructure/button.c','GameObjects/gameobject.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "Game/Menu/menu.h"\n#include "Game/gamestates.h"\n#include "Game/DataStructure/button.h"\nGameState state=MAIN_MENU;ScreenInfo screen={.xres=1280,.yres=1024};bool playing=false,last_pressed_was_mouse=true;\nstatic MouseDevice md;static KeyboardDevice kd;MouseDevice*mouse_device=&md;KeyboardDevice*keyboard_device=&kd;\nstatic Sprite* sprites[128];static bool alive[128];static unsigned used,drawn;\nSprite*create_sprite(xpm_map_t p,int16_t x,int16_t y,bool square,bool visible){(void)p;assert(used<128);Sprite*s=calloc(1,sizeof *s);s->width=100;s->height=40;s->x=x;s->y=y;s->square_shape=square;s->is_visible=visible;sprites[used]=s;alive[used++]=true;return s;}\nvoid destroy_sprite(Sprite*s){for(unsigned i=0;i<used;i++)if(sprites[i]==s){assert(alive[i]);alive[i]=false;return;}assert(!"destroy unknown sprite");}\nint draw_sprite(Sprite*s){for(unsigned i=0;i<used;i++)if(sprites[i]==s){assert(alive[i]&&"menu renderer received destroyed button artwork");if(s->is_visible)drawn++;return 0;}assert(!"draw unknown sprite");return 1;}\nvoid resetDevicesChangingScreens(void){md.left_button_is_pressed=false;}\nvoid enterGame(bool multi,int arena){(void)multi;(void)arena;}\nvoid enterInstructions(void){}\nstatic void frame(int x,int y,bool pressed){md.mouse->x=x;md.mouse->y=y;md.left_button_is_pressed=pressed;updateMenu();drawn=0;renderGameObjects();assert(state==GAME||drawn>0);}\nint main(void){init_render_pipeline();GameObject cursor={0};md.mouse=&cursor;initializeMenu();frame(639,662,true);assert(state==SELECT_GAME);frame(339,311,false);frame(339,311,true);frame(339,311,false);frame(639,311,true);frame(639,311,false);frame(889,811,true);assert(state==GAME);return 0;}\n',source_flags=['-O2'])
    def test_tower_engineering_shared_animation_and_rotated_art_have_single_owners(self):
        self.compile_and_run(COMMUNITY/'PedroPF1234--Tower-Engineering-LCOM-23-24/proj/src',['ImageAssets/sprite.c','GameObjects/gameobject.c','Game/Gameplay/Weapon/weapon.c','Game/Gameplay/Tower/towers.c','Game/Gameplay/Enemy/enemy.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "GameObjects/gameobject.h"\n#include "Game/Gameplay/Weapon/weapon.h"\n#include "Game/Gameplay/Tower/towers.h"\nstatic void* owned[8192];static bool live[8192];static unsigned n;\nvoid* checked_malloc(size_t size){assert(n<8192);void*p=malloc(size?size:1);assert(p);owned[n]=p;live[n++]=true;return p;}\nstatic unsigned index_of(void*p){for(unsigned i=0;i<n;i++)if(owned[i]==p)return i;assert(!"free/render must use an owned allocation");return 0;}\nvoid checked_free(void*p){if(!p)return;unsigned i=index_of(p);assert(live[i]&&"owning sprite was freed twice");live[i]=false;}\nuint8_t*xpm_load(xpm_map_t p,enum xpm_image_type type,xpm_image_t*img){(void)p;(void)type;img->width=2;img->height=2;img->size=12;return checked_malloc(12);}\nint vg_draw_xpm(int16_t x,int16_t y,uint16_t w,uint16_t h,uint8_t*bytes,bool square){(void)x;(void)y;(void)w;(void)h;(void)square;assert(live[index_of(bytes)]&&"game object must not destroy artwork owned by its animation");return 0;}\nint main(void){init_render_pipeline();char*dummy[]={"unused"};char**frames[]={dummy,dummy};AnimatedSprite*animation=create_animated_sprite(frames,2,1);AnimatedGameObject*object=create_animated_gameobject(animation,5,6,0);destroy_animated_gameobject(object);assert(draw_sprite(getSpriteArray(&animation->sprites,0))==0);destroy_animated_sprite(animation);\nWeapon*weapon=initializeWeapon(100,100);GameObject position={.x=100,.y=100};AnimatedGameObject animated={.gameObject=&position};Player player={.player=&animated,.current_direction=DOWN};updateWeapon(weapon,&player,100,200);destroyWeapon(weapon);\ninitializeDifferentTowerSprites();TowerBase*t=initializeTower(100,100);TowerArray towers=newTowerArray(1);pushTowerArray(&towers,t);mountTurret(t,CROSSBOW);Enemy enemy={.x=100,.y=101};Enemy*items[]={&enemy};EnemyArray enemies={.enemies=items,.length=1,.capacity=1};rotateTowersTowardsTarget(&towers,&enemies);hideTowers(&towers);mountTurret(t,CROSSBOW);rotateTowersTowardsTarget(&towers,&enemies);unmountTurret(t);destroyTower(t);\nfor(unsigned i=0;i<n;i++)free(owned[i]);return 0;}\n',source_flags=['-O2','-Dmalloc=checked_malloc','-Dfree=checked_free'])
    def test_tower_engineering_vram_cleanup_releases_the_original_mapping(self):
        self.compile_and_run(COMMUNITY/'PedroPF1234--Tower-Engineering-LCOM-23-24/proj/src',['Devices/Graphics/graphics.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "Devices/Graphics/graphics.h"\nstatic vbe_mode_info_t mode={.XResolution=8,.YResolution=6,.BitsPerPixel=24,.PhysBasePtr=0xe0000000,.RedMaskSize=8,.GreenMaskSize=8,.BlueMaskSize=8};\nstatic unsigned char framebuffer[8*6*3*2];static bool mapped,unmapped;\nvoid*lm_alloc(size_t bytes,mmap_t*map){assert(bytes==sizeof(mode));map->virt=&mode;map->phys=0x1000;map->size=sizeof(mode);return &mode;}\nlcom_compat_bool_t lm_free(const mmap_t*map){assert(map->virt==&mode);return true;}\nint (sys_int86)(struct reg86*r){(void)r;return 0;}\nint sys_privctl(endpoint_t who,int req,void*arg){(void)who;(void)req;(void)arg;return 0;}\nvoid*vm_map_phys(endpoint_t who,void*address,size_t bytes){(void)who;(void)address;assert(bytes==sizeof(framebuffer));mapped=true;return framebuffer;}\nint vm_unmap_phys(endpoint_t who,void*address,size_t bytes){(void)who;assert(mapped&&!unmapped);assert(address==framebuffer&&bytes==sizeof(framebuffer));unmapped=true;return 0;}\nvoid panic(const char*format,...){(void)format;abort();}\nint main(void){assert(vg_init(0x11b)==framebuffer);assert(vg_free()==0);assert(unmapped&&"mapped VRAM must be released through vm_unmap_phys");return 0;}\n',source_flags=['-O2'])

    def test_elo_battleship_controller_header_accepts_real_menu_input(self):
        self.compile_and_run(COMMUNITY/'Eloymtm--LCOM/proj/src',['controller/devicesController.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "controller/devicesController.h"\nstatic unsigned highlights,waiting,instructions;\nint drawSelected(int selected,uint32_t color){assert(selected>=0&&selected<=2);highlights++;return 0;}\nint drawWaiting(bool first){assert(first);waiting++;return 0;}\nint drawInstr(void){instructions++;return 0;}\nint main(void){Board b={.selected=0};Game g={.state=MENU,.board1=&b,.isPlayer1=true};int t=9;handleMenuState(&g,&b,DOWN,&t);assert(b.selected==1&&highlights==2);handleMenuState(&g,&b,ENTER,&t);assert(g.state==RULES&&t==0&&instructions==1);g.state=MENU;handleMenuState(&g,&b,DOWN,&t);assert(b.selected==2);handleMenuState(&g,&b,ENTER,&t);assert(g.state==END);g.state=MENU;handleMenuState(&g,&b,DOWN,&t);assert(b.selected==0);handleMenuState(&g,&b,ENTER,&t);assert(g.state==WAIT&&waiting==1);return 0;}\n',source_flags=['-O2'])

    def test_elo_battleship_vram_allocation_uses_pixel_bytes(self):
        self.compile_and_run(COMMUNITY/'Eloymtm--LCOM/proj/src',['labs/video.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "labs/video.h"\nstatic unsigned char mapped[1152*864*4];static bool called;\nint vbe_get_mode_info(uint16_t mode,vbe_mode_info_t*info){assert(mode==0x14c);memset(info,0,sizeof *info);info->XResolution=1152;info->YResolution=864;info->BitsPerPixel=32;info->BytesPerScanLine=1152*4;info->PhysBasePtr=0xe0000000;return 0;}\nint sys_privctl(endpoint_t who,int req,void*arg){(void)who;(void)req;struct minix_mem_range*r=arg;assert(r->mr_limit-r->mr_base==sizeof(mapped)&&"VRAM range must count bytes, not bits");return 0;}\nvoid*vm_map_phys(endpoint_t who,void*addr,size_t size){(void)who;(void)addr;assert(size==sizeof(mapped));called=true;return mapped;}\nint main(void){assert(allocVirtMem(0x14c)==0);assert(called);reset_video();return 0;}\n',source_flags=['-O2'])

    def test_simon_alarm_disable_writes_register_byte_and_preserves_other_bits(self):
        self.compile_and_run(COMMUNITY/'lessthelonely--SimonSaysVs/proj',['rtc.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "rtc.h"\nstatic unsigned writes;\nint util_sys_inb(int p,uint8_t*v){assert(p==RTC_DATA_REG);*v=0x7b;return 0;}\nint(sys_outb)(int p,uint32_t v){if(p==RTC_ADDR_REG)assert(v==REGISTER_B);else{assert(p==RTC_DATA_REG);assert(v==0x5b&&"disabling alarm must write the register byte with AIE cleared, never a stack pointer");writes++;}return 0;}\nint main(void){disable_alarm_int();assert(writes==1);return 0;}\n',source_flags=['-O2','-Wno-int-conversion'])
    def test_simon_highscore_date_includes_terminator_in_owned_allocation(self):
        self.compile_and_run(COMMUNITY/'lessthelonely--SimonSaysVs/proj',['highscore.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include <sys/mman.h>\n#include <unistd.h>\n#include "highscore.h"\nstatic void*region;static size_t page;static size_t reserved;\nvoid*simon_date_allocate(size_t bytes){page=sysconf(_SC_PAGESIZE);region=mmap(NULL,page*2,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);assert(region!=MAP_FAILED);assert(mprotect((uint8_t*)region+page,page,PROT_NONE)==0);reserved=bytes;return(uint8_t*)region+page-bytes;}\nstruct date get_date(void){return(struct date){.day=17,.month=10,.year=26};}\nint main(void){char*date=getDate();assert(strcmp(date,"17/10/2026")==0&&reserved>=strlen(date)+1);assert(munmap(region,page*2)==0);return 0;}\n',source_flags=['-O2','-Dmalloc=simon_date_allocate'])


    @unittest.skipUnless(shutil.which('emcc') and shutil.which('node'), 'Emscripten and Node are required for the source portability boundary')
    def test_paint_video_mode_type_imports_its_owning_header_on_wasm(self):
        import tempfile, subprocess
        from pathlib import Path
        project=COMMUNITY/'Guilherme-Soares-Sequeira--feup-lcom-proj/proj'
        with tempfile.TemporaryDirectory(prefix='paint-mode-header-') as temporary:
            folder=Path(temporary);harness=folder/'main.c';harness.write_text('#include <lcom/lcf.h>\n#include <assert.h>\nstatic uint8_t vram[48];\nextern vbe_mode_info_t vg_mode_info;\nint vbe_get_mode_info(uint16_t mode,vbe_mode_info_t*out){assert(mode==0x115);*out=(vbe_mode_info_t){.XResolution=4,.YResolution=3,.BitsPerPixel=32,.PhysBasePtr=0xe0000000};return 0;}\nint sys_privctl(endpoint_t self,int request,void*arg){assert(self==SELF&&request==SYS_PRIV_ADD_MEM);struct minix_mem_range*r=arg;assert(r->mr_limit-r->mr_base==48);return 0;}\nvoid*vm_map_phys(endpoint_t self,void*phys,size_t size){assert(self==SELF&&(uintptr_t)phys==0xe0000000&&size==48);return vram;}\nint(sys_int86)(struct reg86*r){assert(r->intno==0x10&&r->ax==0x4f02&&(r->bx&0x3fff)==0x115);r->ax=0x4f;return 0;}\nvoid panic(const char*format,...){(void)format;abort();}\nint main(void){void*back=vg_init(0x115);assert(back&&back!=vram&&vg_mode_info.XResolution==4);free(back);return 0;}\n')
            command=['emcc','-O2','-flto','-std=gnu11','-I',str(ROOT/'sdk/compat/include'),'-I',str(ROOT/'sdk/include'),'-I',str(project),str(project/'src/devices/video/video_gr.c'),str(harness),'-Wl,--gc-sections','-sENVIRONMENT=node','-sEXIT_RUNTIME=1','-o',str(folder/'run.cjs')]
            compile=subprocess.run(command,capture_output=True,text=True)
            self.assertEqual(compile.returncode,0,compile.stdout+compile.stderr)
            run=subprocess.run(['node',str(folder/'run.cjs')],capture_output=True,text=True)
            self.assertEqual(run.returncode,0,run.stdout+run.stderr)


    def test_super_cold_collision_pairs_stop_at_the_list_tail(self):
        self.compile_and_run(COMMUNITY/'brunogomes30--LCOM/proj',['game.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "game.h"\nstatic Sprite*expected[3];static unsigned pairs;\nint checkColision(Sprite*a,Sprite*b){assert(pairs<3);static const unsigned order[3][2]={{0,1},{0,2},{1,2}};assert(a==expected[order[pairs][0]]&&b==expected[order[pairs][1]]);pairs++;return 0;}\nFILE*lcom_compat_fopen(const char*path,const char*mode){(void)path;(void)mode;abort();}\nDate getDate(void){abort();}\nint main(void){Game game={0};Sprite sprites[3]={{0},{0},{0}};EntitiesLinkedElement nodes[3]={{0},{0},{0}};for(unsigned i=0;i<3;i++){expected[i]=&sprites[i];nodes[i].type=SPRITE;nodes[i].data=&sprites[i];nodes[i].next=i<2?&nodes[i+1]:NULL;}game.entitiesList.firstElement=nodes;handleCollisions(&game);assert(pairs==3);pairs=0;nodes[0].next=NULL;handleCollisions(&game);assert(pairs==0);game.entitiesList.firstElement=NULL;handleCollisions(&game);assert(pairs==0);return 0;}\n',source_flags=['-O2','-fsanitize=null','-fsanitize-undefined-trap-on-error'])

    def test_super_cold_first_launch_has_no_phantom_saved_highscore(self):
        self.compile_and_run(COMMUNITY/'brunogomes30--LCOM/proj',['game.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "game.h"\nstatic unsigned attempt;\nFILE*lcom_compat_fopen(const char*path,const char*mode){assert(strcmp(path,"score.txt")==0&&strcmp(mode,"r")==0);if(attempt++==0)return NULL;FILE*f=tmpfile();assert(f);fputs("150 17-10-26 9-32-21",f);rewind(f);return f;}\nint main(void){Game game;memset(&game,1,sizeof game);loadHighScore(&game);assert(game.highScore==0&&"a missing saved score must not expose uninitialized heap contents as a previous highscore");loadHighScore(&game);assert(game.highScore==150&&game.dateHighScore.day==17&&game.dateHighScore.month==10&&game.dateHighScore.year==26&&game.dateHighScore.hours==9&&game.dateHighScore.minutes==32&&game.dateHighScore.seconds==21);return 0;}\n',source_flags=['-O2'])


    def test_rooster_normal_win_formats_the_complete_original_player_name(self):
        self.compile_and_run(COMMUNITY / 'anatdiass--LCOM_1920/proj/src', ['game.c'], '#include <lcom/lcf.h>\n#include <assert.h>\n#include <signal.h>\n#include "game.h"\nint nrRecords;static unsigned added;\nstatic void overflow(int sig){(void)sig;fputs("Original winning-score name formatting overflowed its destination\\n",stderr);_Exit(96);}\nbool verifyIsRecord(int seconds){assert(seconds==12);return true;}\nvoid addRecord(char*name,int seconds){assert(seconds==12);assert(!strcmp(name,nrRecords==0?"Jogador 1":"Jogador 4"));added++;}\nint main(void){signal(SIGABRT,overflow);signal(SIGILL,overflow);signal(SIGTRAP,overflow);int board[]={1,1,1,2,2,0,0,0,0};nrRecords=0;assert(gameOver(board,12)==0);nrRecords=3;assert(gameOver(board,12)==0);assert(added==2);puts("Actual single-player win submits complete original names for first and full-table records");return 0;}\n', source_flags=['-O2', '-D_FORTIFY_SOURCE=2', '-fstack-protector-strong'])
    def test_rooster_record_owner_preserves_three_digit_times_and_full_table_order(self):
        self.compile_and_run(COMMUNITY / 'anatdiass--LCOM_1920/proj/src', ['highscores.c'], '#include <lcom/lcf.h>\n#include <assert.h>\n#include <signal.h>\n#include "highscores.h"\nextern Record records[3];extern int nrRecords;\nstatic void empty_access(int sig){(void)sig;fputs("Empty score table read a nonexistent time slot\\n",stderr);_Exit(99);}\nstatic void overflow(int sig){(void)sig;fputs("Original score owner overflowed a normal three-digit game duration\\n",stderr);_Exit(97);}\nint main(void){signal(SIGABRT,overflow);signal(SIGILL,overflow);signal(SIGTRAP,overflow);signal(SIGSEGV,empty_access);if(getMaxTemp(NULL,0)!=0)return 99;nrRecords=0;addRecord("Jogador 1",123);assert(nrRecords==1&&!strcmp(records[0].name,"Jogador 1")&&!strcmp(records[0].seconds,"123"));nrRecords=0;addRecord("Jogador 1",20);addRecord("Jogador 2",10);addRecord("Jogador 3",30);assert(nrRecords==3);if(verifyIsRecord(31)){fputs("Full three-record table accepted a slower game as a new record\\n",stderr);return 98;}assert(verifyIsRecord(9));addRecord("Jogador 4",9);assert(nrRecords==3&&!strcmp(records[2].seconds,"9")&&!strcmp(records[2].name,"Jogador 4"));int times[3];getSecondsArray(times);assert(times[0]==20&&times[1]==10&&times[2]==9);puts("Actual score owner retains123seconds, rejects slower full-table records and replaces the longest game with a faster record");return 0;}\n', source_flags=['-O2', '-D_FORTIFY_SOURCE=2', '-fstack-protector-strong'])

    def test_rooster_date_digits_and_leading_zero_time_reach_actual_glyph_renderer(self):
        self.compile_and_run(COMMUNITY/'anatdiass--LCOM_1920/proj/src',['xpmHandler.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "xpmHandler.h"\nextern xpm_image_t number_0,number_1,number_2,number_3,number_4,number_5,number_6,number_7,number_8,number_9,barra_img,doisPontos;\nint global_counter;static const char*expected;static unsigned index_,kind;\nstatic void images(void){xpm_image_t*digits[]={&number_0,&number_1,&number_2,&number_3,&number_4,&number_5,&number_6,&number_7,&number_8,&number_9};for(unsigned i=0;i<10;i++){digits[i]->bytes=(uint8_t*)(uintptr_t)(\'0\'+i);digits[i]->width=16;}barra_img.bytes=(uint8_t*)(uintptr_t)\'/\';barra_img.width=16;doisPontos.bytes=(uint8_t*)(uintptr_t)\':\';doisPontos.width=16;}\nint vg_draw_img(xpm_image_t image,uint16_t x,uint16_t y){assert(index_<strlen(expected));assert((uintptr_t)image.bytes==(unsigned char)expected[index_]);unsigned xx=kind==0?(index_<10?25+index_*8:630+(index_-10)*8):640+index_*8;assert(x==xx&&y==(kind==0?5:180));index_++;return 0;}\n\nstatic unsigned sample;int getDayRTC(void){return sample?5:17;}int getMonthRTC(void){return sample?2:10;}int getYearRTC(void){return 26;}int getHoursRTC(void){return sample?3:9;}int getMinutesRTC(void){return sample?4:7;}\nint main(void){images();kind=0;expected="17/10/202609:07";printDate();assert(index_==strlen(expected));sample=1;index_=0;expected="05/02/202603:04";printDate();assert(index_==strlen(expected));return 0;}\n',source_flags=['-O2','-D_FORTIFY_SOURCE=2','-fstack-protector-strong'])
    def test_rooster_elapsed_counter_digits_and_long_round_reach_actual_glyph_renderer(self):
        self.compile_and_run(COMMUNITY/'anatdiass--LCOM_1920/proj/src',['xpmHandler.c'],'#include <lcom/lcf.h>\n#include <assert.h>\n#include "xpmHandler.h"\nextern xpm_image_t number_0,number_1,number_2,number_3,number_4,number_5,number_6,number_7,number_8,number_9,barra_img,doisPontos;\nint global_counter;static const char*expected;static unsigned index_,kind;\nstatic void images(void){xpm_image_t*digits[]={&number_0,&number_1,&number_2,&number_3,&number_4,&number_5,&number_6,&number_7,&number_8,&number_9};for(unsigned i=0;i<10;i++){digits[i]->bytes=(uint8_t*)(uintptr_t)(\'0\'+i);digits[i]->width=16;}barra_img.bytes=(uint8_t*)(uintptr_t)\'/\';barra_img.width=16;doisPontos.bytes=(uint8_t*)(uintptr_t)\':\';doisPontos.width=16;}\nint vg_draw_img(xpm_image_t image,uint16_t x,uint16_t y){assert(index_<strlen(expected));assert((uintptr_t)image.bytes==(unsigned char)expected[index_]);unsigned xx=kind==0?(index_<10?25+index_*8:630+(index_-10)*8):640+index_*8;assert(x==xx&&y==(kind==0?5:180));index_++;return 0;}\n\nint main(void){images();kind=1;expected="01:01";global_counter=61*60;printCounter();assert(index_==strlen(expected));index_=0;expected="123:45";global_counter=123*3600+45*60;printCounter();assert(index_==strlen(expected));return 0;}\n',source_flags=['-O2','-D_FORTIFY_SOURCE=2','-fstack-protector-strong'])


    def test_super_cold_new_guns_start_on_the_authored_idle_frame_on_poisoned_heap(self):
        self.compile_and_run(COMMUNITY/'brunogomes30--LCOM/proj',['gun.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "gun.h"
xpm_image_t pistol_0_image,pistol_1_image,pistol_2_image,pistol_3_image;static Sprite sprites[2];static unsigned created;
void*poisoned_malloc(size_t size){void*p=malloc(size);assert(p);memset(p,0xA5,size);return p;}
Sprite*createSprite(xpm_image_t*image,int x,int y){assert(created<2);Sprite*s=&sprites[created++];s->image=image;s->x=x;s->y=y;return s;}
int main(void){Game game={0};Gun*guns[]={getPistol(&game,1,10,20),getSMG(&game,1,30,40)};for(unsigned i=0;i<2;i++){Gun*g=guns[i];assert(g->current_image==0&&"A new gun must not index its four authored animation frames with uninitialized heap bytes");updateGunAnimation(g);assert(g->sprite->image==&pistol_0_image);g->isShooting=1;g->current_animation_tick=5;updateGunAnimation(g);assert(g->sprite->image==&pistol_1_image);free(g);}return 0;}
''',source_flags=['-O2','-Dmalloc=poisoned_malloc'])

    def test_dino_driver_pixel_edges_preserve_valid_pixels_and_frame_canaries(self):
        self.compile_and_run(COMMUNITY / 'ban08--lcom-minix-driver-game/proj/src', ['video_card.c'], '#include <lcom/lcf.h>\n#include <assert.h>\n#include "video_card.h"\nextern uint8_t*frame_buffer;extern vbe_mode_info_t mode_info;\nint main(void){uint8_t memory[24],snapshot[24];memset(memory,0xA5,sizeof memory);frame_buffer=memory+2;mode_info.XResolution=2;mode_info.YResolution=2;mode_info.BitsPerPixel=24;assert(vg_draw_pixel(1,1,0x123456)==0);for(unsigned i=0;i<sizeof memory;i++){uint8_t expected=i==11?0x56:i==12?0x34:i==13?0x12:0xA5;assert(memory[i]==expected);}memcpy(snapshot,memory,sizeof memory);assert(vg_draw_pixel(2,0,0x112233)==1&&"x equal to framebuffer width is outside");assert(vg_draw_pixel(0,2,0x112233)==1&&"y equal to framebuffer height is outside");assert(vg_draw_pixel(65535,65535,0x112233)==1);assert(memcmp(memory,snapshot,sizeof memory)==0&&"clipped writes preserve neighboring pixels and both framebuffer canaries");return 0;}\n', source_flags=['-O2', '-D_FORTIFY_SOURCE=2'])

    def test_sueca_card_selection_owns_a_non_reserved_round_state(self):
        self.compile_and_run(COMMUNITY / 'pchmelo--LCOM_Projeto/src', ['prog/functions/composer.c', 'prog/process/teclado_process.c', 'prog/game_rules/rules.c'], '#include <assert.h>\n#include "prog/functions/composer.h"\nextern game_round current_game_round;\nextern Player player1;\nextern int selected_card_player_1,roundSuit;\nstatic unsigned sent;\nint enviar_byte(uint8_t value){assert(value==0);sent++;return 0;}\nint main(void){\n assert(current_game_round==WHAIT_GAME_ROUND);\n current_game_round=PLAYER_1;player1.numCards=2;\n player1.playerHand[0]=(Card){.suit=0};player1.playerHand[1]=(Card){.suit=1};\n roundSuit=0;selected_card_player_1=1;int keyboard=0,mouse=1;\n process_select_card_P1(ENTER_BREAK,&keyboard,&mouse,2);\n assert(current_game_round==PLAYER_1&&sent==0);\n process_select_card_P1(RIGHT_ARROW_BREAK,&keyboard,&mouse,2);\n assert(selected_card_player_1==0&&keyboard==1&&mouse==0);\n process_select_card_P1(ENTER_BREAK,&keyboard,&mouse,2);\n assert(current_game_round==PLAYER_1_CHOOSE&&sent==1);\n return 0;}\n', source_flags=['-O2'])

    def test_card_table_rtc_header_resolves_its_owned_utilities(self):
        self.compile_and_run(COMMUNITY / 'pchmelo--LCOM_Projeto/src', ['dispositivos/rtc/rtc.c'], '#include <assert.h>\n#include "dispositivos/rtc/rtc.h"\nextern rtc_struct rtc_time;\nstatic unsigned reads;int rtc_read_output(uint8_t*data,uint8_t command){static const uint8_t commands[]={4,10,0,2,4},values[]={0,0,0x12,0x34,0x15};assert(reads<5&&command==commands[reads]);*data=values[reads++];return 0;}\nint main(void){assert(rtc_read_set()==0);assert(rtc_time.seconds==0x12&&rtc_time.minutes==0x34&&rtc_time.hours==0x15);return 0;}\n', source_flags=['-O2'])

    def test_vs_draw_packet_consumer_preserves_byte_sized_mouse_owner(self):
        p=COMMUNITY/'6ernardo--feup-LCOM/proj/src'
        s=(p/'main.c').read_text()
        import re
        decls='\n'.join(re.findall(r'extern (?:int|uint8_t) (?:mouse_bytes\[3\]|byte_index);',s))
        h='#include "controller/mouse/mouse.h"\n#include "controller/video/graphics.h"\n#include <assert.h>\nextern uint8_t current_byte;\nextern MouseInfo mouse_info;\n\nvbe_mode_info_t mode_info;\nint main(void){mode_info.XResolution=800;mode_info.YResolution=600;current_byte=0x28;mouse_sync_bytes();consume();current_byte=20;mouse_sync_bytes();consume();current_byte=236;mouse_sync_bytes();consume();assert(mouse_info.x==120 && mouse_info.y==120);return 0;}\n'
        h=h.replace('int main(void)',decls+'\nvoid consume(void){if(byte_index==3){byte_index=0;mouse_bytes_to_packet();}}\nint main(void)')
        self.compile_and_run(p,['controller/mouse/mouse.c'],h,source_flags=['-O2'])

    def test_vs_draw_grayscale_preserves_adjacent_rgb24_pixel_and_last_canary(self):
        self.compile_and_run(COMMUNITY/'6ernardo--feup-LCOM/proj/src',['controller/video/graphics.c'],'#include "controller/video/graphics.h"\n#include <assert.h>\n#include <string.h>\nextern vbe_mode_info_t mode_info;\nint main(void){uint8_t f[8]={0x12,0x34,0x56,0x78,0x9a,0xbc,0xa5,0x5a};mode_info.XResolution=2;mode_info.YResolution=1;mode_info.BitsPerPixel=24;assert(convert_to_gray(0,0,f)==0);assert(f[0]==52&&f[1]==52&&f[2]==52);assert(f[3]==0x78&&f[6]==0xa5&&f[7]==0x5a);assert(convert_to_gray(1,0,f)==0);assert(f[3]==154&&f[4]==154&&f[5]==154&&f[6]==0xa5&&f[7]==0x5a);uint8_t copy[8];memcpy(copy,f,8);assert(convert_to_gray(2,0,f)==1&&memcmp(copy,f,8)==0);return 0;}\n',source_flags=['-O2'])

    def test_vs_draw_rectangle_commits_at_current_release_without_another_packet(self):
        s=(COMMUNITY/'6ernardo--feup-LCOM/proj/src/main.c').read_text()
        a=s.index('                if (leftButtonPressed && tool == RECTANGLE');b=s.index('                if (!mouse_info.left_click && draw)',a)
        e=s[s.index('              mouse_bytes_to_packet();'):s.index('              int x = getX();')]
        after=s.index('              prevX = x;');end=s.index('\n            }',after)
        h='#include <stdbool.h>\n#define BIT(n) (1u<<(n))\n#include <assert.h>\nunsigned drawingColor;int leftButtonPressed,first_x,first_y,prevX,prevY,tool=1;enum{RECTANGLE=1};bool lbwaspressed;struct{bool left_click;}mouse_info;static unsigned calls;\nvoid invert_backup_draw(void){}\nvoid processRectangle(int a,int b,int x,int y,unsigned c){assert(a==200&&b==150&&x==350&&y==300&&c==0);calls++;}\nvoid mouse_bytes_to_packet(void){}\nunsigned char mouse_bytes[3];\n'+'void event(bool down,int x,int y){mouse_info.left_click=down;mouse_bytes[0]=down;\n'+e+s[a:b]+s[after:end]+'}'+'\nint main(void){event(true,200,150);event(true,350,300);event(false,350,300);assert(calls==1&&"Rectangle must commit at the actual release position without another mouse event");return 0;}\n'
        self.compile_and_run(COMMUNITY/'6ernardo--feup-LCOM/proj/src',[],h)

    def test_lowlife_original_text_glyph_loader_preserves_line_terminators(self):
        p=COMMUNITY/'JFernandes2612--FEUP-LCOM-2021/proj'
        s=(p/'graphics.c').read_text();a=s.index('int file_to_xpm(');b=s.index('\nint paint_xpm(',a)
        asset=str(p/'sprites/xpm_files/A.xpm')
        h='#include <lcom/lcf.h>\n#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include <assert.h>\n#undef fopen\nstatic void *last;static size_t last_size;\nstatic void *checked_realloc(void*p,size_t n){void*r=realloc(p,n);last=r;last_size=n;return r;}\nstatic size_t checked_strlen(const char*p){if(p==last)assert(memchr(p,0,last_size)!=NULL && "text read exceeded requested realloc size");return strlen(p);}\n#define realloc checked_realloc\n#define strlen checked_strlen\n'+s[a:b]+'\n#define ASSET_PATH '+__import__('json').dumps(asset)+'\n'+'int file_to_xpm(const char*,xpm_image_t*);\nint main(void){xpm_image_t img={0};assert(file_to_xpm(ASSET_PATH,&img)==0);assert(img.width==11 && img.height==14);free(img.bytes);return 0;}\n'+'\n#include '+__import__('json').dumps(str(ROOT/'sdk/compat/src/xpm.c'))+'\n'
        self.compile_and_run(p,[],h)

    def test_lowlife_singleplayer_mouse_input_does_not_read_absent_player_two(self):
        p=COMMUNITY/'JFernandes2612--FEUP-LCOM-2021/proj'
        s=(p/'game_helper.c').read_text();a=s.index('void process_mouse_input(');b=s.index('\nvoid ',a+5)
        h='#include "game_helper.h"\n#include "mouse.h"\nextern struct packet mouse_pack;\n'+s[a:b]+'#include "game_helper.h"\n#include "mouse.h"\n#include <assert.h>\nstruct packet mouse_pack;static unsigned motions;\nvoid update_mouse_position(struct Cursor*c,int16_t dx,int16_t dy){(void)c;(void)dx;(void)dy;motions++;}\nvoid send_data_uart(uint8_t c){(void)c;abort();}\nstruct Entity*create_spearman(uint8_t x,uint8_t y){(void)x;(void)y;abort();}\nvoid create_enemies(struct Game g){(void)g;abort();}\nint grid_add(struct Entity*e){(void)e;abort();}\nint create_background(void){abort();}\nint main(void){struct SpearMan s={0};struct Entity e={.entity_info=&s};struct Cursor c={0};struct Game g={.cursor=&c,.player_1=&e,.player_2=NULL,.player_num=PLAYER_1};g.game_state.menu=IN_GAME;g.game_state.gamemode=SINGLEPLAYER;mouse_pack.lb=true;process_mouse_input(&g);mouse_pack.lb=false;mouse_pack.rb=true;process_mouse_input(&g);assert(motions==2&&g.action_request_player_1==0);return 0;}\n'
        self.compile_and_run(p,[],h,source_flags=['-O0','-fsanitize=null','-fsanitize-undefined-trap-on-error'])

    def test_memory_card_redraw_reuses_twenty_owned_front_and_back_images(self):
        h='#include "cards.h"\n#include <assert.h>\n#include <string.h>\nstatic unsigned loads,draws;dot_t pto;int pair_counter;\nint rand_(void){return 7;}\nuint8_t*counted_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*img){loads++;return xpm_load(map,type,img);}\nint draw_pix_map(xpm_image_t img,uint16_t x,uint16_t y){assert(img.width==65&&img.height==90&&img.bytes!=NULL);(void)x;(void)y;draws++;return 0;}\nint main(void){card_t deck[10];assert(createDeck(deck)==0);assert(loads==20&&"Ten cards must own one back and their selected front, without fallthrough allocations");for(unsigned n=0;n<100;n++)for(unsigned i=0;i<10;i++){assert(drawCartaFrente(deck[i])==0);assert(drawCartaVerso(deck[i])==0);}assert(loads==20&&draws==2000&&"Frame redraws must use owned images rather than allocate and leak two new images per card");for(unsigned i=0;i<10;i++){free(deck[i].frente.bytes);free(deck[i].verso.bytes);}return 0;}\n'+'\n#include '+__import__('json').dumps(str(ROOT/'sdk/compat/src/xpm.c'))+'\n'
        self.compile_and_run(COMMUNITY/'joaoDossena--MIEIC-LCOM/proj/src',['cards.c'],h,source_flags=['-O2','-Dxpm_load=counted_load'])

    def test_bowcannon_first_trigger_creates_a_projectile(self):
        p=COMMUNITY/'Jumaruba--FEUP-LCOM20/proj/scr'
        s=(p/'game.c').read_text();a=s.index('void handle_shot(');b=s.index('\nvoid get_angle(',a)
        h='#include <lcom/lcf.h>\n#include "sprite.h"\n#include "graphic.h"\n#include "game.h"\n#include <assert.h>\n#include <math.h>\nSprite *ball,*cannon,*cannon2;\nbool first=true,doing_shot=true,shot=true,draw;\nfloat segundos=1,angulo=0;int contador;\n'+s[a:b]+'#include <lcom/lcf.h>\n#include "sprite.h"\n#include "graphic.h"\n#include "game.h"\n#include <assert.h>\n#include <stdlib.h>\nextern Sprite*ball,*cannon,*cannon2;extern bool doing_shot,shot;extern bool first;static bool released;static unsigned frees,draws,created;static Sprite fresh;\nbool checkCollision(void){return 0;}\nvoid get_angle(void){}\nvoid destroy_sprite(Sprite*s){assert(s==ball);released=true;frees++;}\nint drawSprite(Sprite*s,int x,int y){(void)s;(void)x;(void)y;assert(!released&&"An ended projectile must not be rendered after its owning destructor");draws++;return 0;}\nxpm_map_t get_game_ball_xpm(void){return NULL;}\nSprite*create_sprite(xpm_map_t a,int b,int c,int d,int e){(void)a;(void)b;(void)c;(void)d;(void)e;created++;return &fresh;}\nint main(void){Sprite b={.x=100,.y=703,.xspeed=35},c={.width=50,.height=50};ball=&b;cannon=&c;cannon2=&c;first=false;doing_shot=false;shot=true;handle_shot();assert(created==1&&draws==0&&doing_shot&&"The first real trigger must create a projectile without requiring a discarded setup click");return 0;}\n'
        self.compile_and_run(p,[],h)

    def test_bowcannon_cursor_accepts_independent_horizontal_and_vertical_packets(self):
        p=COMMUNITY/'Jumaruba--FEUP-LCOM20/proj/scr'
        s=(p/'sprite.c').read_text();a=s.index('void animateMouse(');b=s.index('\nvoid drawMenu(',a)
        h='#include <lcom/lcf.h>\n#include "sprite.h"\n'+s[a:b]+'#include <lcom/lcf.h>\n#include "sprite.h"\n#include <assert.h>\nstatic unsigned draws;\nint drawSprite(Sprite*s,int x,int y){(void)s;assert(x==20&&y==(draws?30:0));draws++;return 0;}\nint main(void){int x=0,y=0;struct packet p={.delta_x=20};animateMouse(NULL,&x,&y,&p);assert(x==20&&y==0);p.delta_x=0;p.delta_y=-30;animateMouse(NULL,&x,&y,&p);assert(x==20&&y==-30&&draws==2);return 0;}\n'
        self.compile_and_run(p,[],h)

    def test_bowcannon_finished_projectile_is_not_rendered_after_its_destructor(self):
        p=COMMUNITY/'Jumaruba--FEUP-LCOM20/proj/scr'
        s=(p/'game.c').read_text();a=s.index('void handle_shot(');b=s.index('\nvoid get_angle(',a)
        h='#include <lcom/lcf.h>\n#include "sprite.h"\n#include "graphic.h"\n#include "game.h"\n#include <assert.h>\n#include <math.h>\nSprite *ball,*cannon,*cannon2;\nbool first=true,doing_shot=true,shot=true,draw;\nfloat segundos=1,angulo=0;int contador;\n'+s[a:b]+'#include <lcom/lcf.h>\n#include "sprite.h"\n#include "graphic.h"\n#include "game.h"\n#include <assert.h>\n#include <stdlib.h>\nextern Sprite*ball,*cannon,*cannon2;extern bool doing_shot,shot;static bool released;static unsigned frees,draws;\nbool checkCollision(void){return 0;}\nvoid get_angle(void){}\nvoid destroy_sprite(Sprite*s){assert(s==ball);released=true;frees++;}\nint drawSprite(Sprite*s,int x,int y){(void)s;(void)x;(void)y;assert(!released&&"An ended projectile must not be rendered after its owning destructor");draws++;return 0;}\nxpm_map_t get_game_ball_xpm(void){abort();}\nSprite*create_sprite(xpm_map_t a,int b,int c,int d,int e){(void)a;(void)b;(void)c;(void)d;(void)e;abort();}\nint main(void){Sprite b={.x=100,.y=703,.xspeed=35},c={.width=50,.height=50};ball=&b;cannon=&c;cannon2=&c;handle_shot();assert(frees==1&&draws==0&&!doing_shot&&!shot);return 0;}\n'
        self.compile_and_run(p,[],h)

    def test_el_memory_cleanup_releases_each_original_sprite_once(self):
        self.compile_and_run(COMMUNITY/'LuisRelvas--ProjectLCOM',
            ['game/modes/menu.c','game/sprite/sprite.c'], r'''#include <lcom/lcf.h>
#include <assert.h>
#include "game/sprite/sprite.h"
extern Sprite *back;
void del_sprites(void);
static void *pixels,*sprite;
static unsigned pixel_frees,sprite_frees;
void memory_checked_free(void *pointer){
    if(pointer==pixels)assert(++pixel_frees==1 && "sprite pixels must be released once");
    else {assert(pointer==sprite);assert(++sprite_frees==1 && "sprite must be released once");}
}
int main(void){
    back=calloc(1,sizeof *back);assert(back);back->colors=calloc(4,sizeof *back->colors);assert(back->colors);
    pixels=back->colors;sprite=back;del_sprites();
    assert(pixel_frees==1 && sprite_frees==1);free(pixels);free(sprite);return 0;
}
''', source_flags=['-O2','-Dfree=memory_checked_free'])

    def test_el_memory_successful_teardown_releases_rtc_and_mouse(self):
        self.compile_and_run(COMMUNITY/'LuisRelvas--ProjectLCOM',['main.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
int turn_off(void);
static unsigned calls;
int vg_exit(void){assert(calls==0);calls=1;return 0;}
void del_sprites(void){assert(calls==1);calls=2;}
int timer_unsubscribe_interrupts(void){assert(calls==2);calls=3;return 0;}
int keyboard_unsubscribe_interrupts(void){assert(calls==3);calls=4;return 0;}
int mouse_unsubscribe_interrupts(void){assert(calls==4);calls=5;return 0;}
int rtc_unsubscribe_interrupts(void){assert(calls==5);calls=6;return 0;}
int mouse_write(uint8_t command){assert(calls==6 && command==0xf5);calls=7;return 0;}
int main(void){assert(turn_off()==0 && "successful cleanup must return success");assert(calls==7);return 0;}
''',source_flags=['-O2','-Dmain=memory_student_entry'])

if __name__ == '__main__':
    unittest.main()
