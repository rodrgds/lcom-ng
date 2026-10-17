#!/usr/bin/env python3
"""Actual student source regressions found while expanding the arcade."""
import unittest
import os
import tempfile
from pathlib import Path
import community_source_regressions as existing
COMMUNITY = existing.COMMUNITY

class ExpansionSourceRegressions(unittest.TestCase):
    def test_five_points_pong_native_pixels_retain_the_full_framebuffer_address(self):
     self.compile_and_run(COMMUNITY/'miguelazevedolopes--FEUP-LCOM/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdint.h>
static unsigned char*frame;static unsigned writes;
static void*checked_copy(void*dest,const void*source,size_t n){assert((uintptr_t)dest>=(uintptr_t)frame&&(uintptr_t)dest+n<=(uintptr_t)frame+48&&"pixel writes retain the native framebuffer pointer");writes++;return memcpy(dest,source,n);}
#define memcpy checked_copy
#include "v.c"
#undef memcpy
int main(void){frame=malloc(48);assert(frame&&(uintptr_t)frame>UINT32_MAX);memset(frame,0xa5,48);video_mem=(char*)frame;x_resolution=8;y_resolution=6;bits_per_pixel=8;vg_colour_pixel(1,1,0x32);vg_colour_pixel(7,5,0x51);vg_colour_pixel(8,5,0xff);assert(writes==2&&frame[9]==0x32&&frame[47]==0x51&&frame[0]==0xa5);free(frame);return 0;}
''')

    def test_five_points_pong_mouse_irq_storage_holds_a_complete_three_byte_packet(self):
     import re,os
     from unittest.mock import patch
     project=COMMUNITY/'miguelazevedolopes--FEUP-LCOM/src';text=(project/'game.c').read_text(encoding='latin1');decl=re.search(r'uint8_t bytes\[\d+\];',text).group(0);start=text.index('                if(initialMenuFlag)',text.index('if (msg.m_notify.interrupts & mouse_bitmask)'));brace=text.index('{',start);end=brace+1;depth=1
     while depth:
      if text[end]=='{':depth+=1
      elif text[end]=='}':depth-=1
      end+=1
     block=text[start:end]
     with patch.dict(os.environ,{'CC':os.environ.get('CC','cc')+' -fsanitize=bounds -fno-sanitize-recover=bounds'}):
      self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "mouse.c"
'''+decl+r'''
static uint8_t size;static bool initialMenuFlag=true,ESC_flag,pauseMenuFlag,firstTime,gameStarted,maxScoreMenuFlag;static int response,moved,clicked;
void move_pointer(struct packet*p){assert(p->delta_x==20&&p->delta_y==-5&&p->bytes[0]==0x28&&p->bytes[1]==20&&p->bytes[2]==251);moved++;}
int detectMenuOptionClick(struct packet*p){clicked++;return 3;}void clearScreen(void){}
int main(void){uint8_t stream[]={0x28,20,251};for(unsigned n=0;n<3;n++){data=stream[n];
'''+block+r'''
if(n<2)assert(!moved&&!clicked);}
assert(moved==1&&clicked==1&&size==0&&ESC_flag);return 0;}
''')

    def test_zombie_score_submission_preserves_terminated_name_date_and_active_state(self):
        project=COMMUNITY/'GNeves95--lcom1819-t7g10/proj'
        source=(project/'proj.c').read_text()
        block=source.split('if(ks.enter){',1)[1].split('\n\t\t\t\t\t}',1)[0]
        self.assertIn('strcpy(currScore.player, player);',block)
        self.assertIn('sprintf(currScore.date,',block)
        harness=r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdarg.h>
#include "game.h"
#include "rtc.h"
static void writeHorCenter(const char*text,int y){(void)y;assert(!strcmp(text,"17/10/2026 08:59:01"));}
int getVRes(void){return 600;}
static char*bounded_name(char*dest,const char*src,size_t capacity){size_t bytes=strlen(src)+1;if(bytes>capacity){fputs("Postgame name copy exceeds its own field\n",stderr);exit(94);}return memcpy(dest,src,bytes);}
static int bounded_date(char*dest,size_t capacity,const char*format,...){char text[128];va_list args;va_start(args,format);int count=vsnprintf(text,sizeof text,format,args);va_end(args);assert(count>=0);if((size_t)count+1>capacity){fputs("Postgame date formatting exceeds its own field\n",stderr);exit(95);}memcpy(dest,text,(size_t)count+1);return count;}
#undef strcpy
#define strcpy(dest,src) bounded_name(dest,src,__builtin_object_size(dest,1))
#undef sprintf
#define sprintf(dest,...) bounded_date(dest,__builtin_object_size(dest,1),__VA_ARGS__)
int main(void){hscore currScore;memset(&currScore,0xa5,sizeof currScore);hero hero_state={.score=4};souto=&hero_state;char player[]="AZZ";timedate currDate={.day=17,.month=10,.year=2026,.hours=8,.minutes=59,.seconds=1};
'''+block+r'''
if(!currScore.active){fputs("Submitting a normal date overwrote the score active flag\n",stderr);return 94;}if(sizeof currScore.player<strlen(player)+1||sizeof currScore.date<strlen("17/10/2026 08:59:01")+1){fputs("Score text exceeds its owning field including the terminator\n",stderr);return 95;}assert(currScore.points==4&&!strcmp(currScore.player,"AZZ")&&!strcmp(currScore.date,"17/10/2026 08:59:01"));puts("Actual postgame submission keeps all three name letters, date, score and active flag in their owning fields");return 0;}
'''
        self.compile_and_run(project,[],harness)

    def test_star_fighter_renderer_reads_each_pixel_once_and_preserves_edges(self):
        project=COMMUNITY/'PedroCarmoPereira--LCOM-FEUP/proj/src'
        self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include "video.c"
static struct {uint32_t before[8],pixels[16*12],after[8];} frame;
static void overread(int sig){(void)sig;const char message[]="Sprite renderer read beyond the final source pixel\n";write(2,message,sizeof(message)-1);_Exit(93);}
uint32_t xpm_transparency_color(enum xpm_image_type type){assert(type==XPM_8_8_8_8);return 0xff000000;}
int main(void){signal(SIGSEGV,overread);signal(SIGBUS,overread);h_res=16;v_res=12;double_buffer=(char*)frame.pixels;long page=sysconf(_SC_PAGESIZE);assert(page>0);uint8_t*image=mmap(NULL,(size_t)page*2,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);assert(image!=MAP_FAILED);assert(mprotect(image+page,(size_t)page,PROT_NONE)==0);uint32_t*last=(uint32_t*)(image+page-4);*last=0x00112233;xpm_image_t one={.type=XPM_8_8_8_8,.width=1,.height=1,.size=4,.bytes=(uint8_t*)last};draw_sproot(one,2,3);assert(frame.pixels[3*16+2]==0x00112233);munmap(image,(size_t)page*2);
uint32_t source[]={0x00010203,0xff000000,0x00070809,0x000a0b0c,0x000d0e0f,0x00101112};xpm_image_t sprite={.type=XPM_8_8_8_8,.width=3,.height=2,.size=sizeof source,.bytes=(uint8_t*)source};const int positions[][2]={{2,3},{-1,2},{15,11},{0,-1},{16,12}};for(unsigned n=0;n<sizeof positions/sizeof positions[0];n++){for(unsigned i=0;i<8;i++)frame.before[i]=frame.after[i]=0xa5a5a5a5;for(unsigned i=0;i<16*12;i++)frame.pixels[i]=0x00555555;int left=positions[n][0],top=positions[n][1];draw_sproot(sprite,left,top);for(int y=0;y<12;y++)for(int x=0;x<16;x++){uint32_t expected=0x00555555;if(x>=left&&x<left+3&&y>=top&&y<top+2){uint32_t color=source[(y-top)*3+x-left];if(color!=0xff000000)expected=color;}assert(frame.pixels[y*16+x]==expected);}for(unsigned i=0;i<8;i++)assert(frame.before[i]==0xa5a5a5a5&&frame.after[i]==0xa5a5a5a5);}puts("Actual renderer reads exact source pixels, preserves transparency, and keeps framebuffer writes within all four edges");return 0;}
''',source_flags=['-O2'])

    def test_jumping_peanut_start_has_no_clouds_before_the_first_spawn(self):
        self.compile_and_run(COMMUNITY/'diogoyaguas--FEUP-LCOM/proj/src',['jumpingPeanut.c'],r'''#include <minix/drivers.h>
#include "jumpingPeanut.h"
#include <assert.h>
static Bitmap image;
void*pattern_malloc(size_t size){void*p=malloc(size);assert(p);memset(p,0xff,size);return p;}
Bitmap*loadBitmap(const char*path){assert(path);return &image;}
int kbd_subscribe(void){return 2;}int mouse_subscribe(void){return 4;}int timer_subscribe_int(void){return 1;}int rtc_subscribe_int(void){return 256;}
int main(void){for(unsigned round=0;round<2;round++){JumpingPeanut*p=startJumpingPeanut();assert(p&&p->life_player==3&&p->score_player==0&&p->x_player==150&&p->y_player==384);assert(p->kbd_irq==2&&p->mouse_irq==4&&p->timer_irq==1&&p->rtc_irq==256);for(unsigned i=0;i<100;i++)if(p->clouds[i]!=NULL){fprintf(stderr,"Start left cloud slot %u nonempty before any cloud spawn\n",i);return 91;}free(p);}puts("Fresh and retried games start with all100cloud slots empty before the first timer movement");return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Dmalloc=pattern_malloc'])

    def test_jumping_peanut_mouse_starts_with_no_pending_gesture(self):
        self.compile_and_run(COMMUNITY/'diogoyaguas--FEUP-LCOM/proj/src',['mouse.c'],r'''#include <minix/drivers.h>
#include "mouse.h"
#include <assert.h>
static Bitmap image;
void*pattern_malloc(size_t size){void*p=malloc(size);assert(p);memset(p,0xff,size);return p;}
Bitmap*loadBitmap(const char*path){assert(path);return &image;}
int main(void){Peanut_mouse*m=new_mouse();assert(m&&m->icon==&image&&m->x==512&&m->y==384&&m->draw==1);if(m->left_button_d||m->left_button_r||m->delta_x||m->delta_y){fputs("Mouse constructor left a phantom pending gesture before its first packet\n",stderr);return 92;}free(m);puts("Mouse state starts released and motionless before the first real packet");return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Dmalloc=pattern_malloc'])


    def test_checker_garden_frames_reuse_original_sprites_and_allow_cleanup(self):
        self.compile_and_run(COMMUNITY/'carlotaleite--LCOM-Project'/'proj/src', ['model/world.c','model/sprite.c'], r'''#include "model/world.h"
#include "model/sprite.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
vbe_mode_info_t modeInfo={.XResolution=800,.YResolution=600};static void*live[64];static int allocations,loads;
void*tracked_malloc(size_t n){void*p=malloc(n);assert(p);for(int i=0;i<64;i++)if(!live[i]){live[i]=p;allocations++;return p;}assert(!"unbounded frame allocations");return NULL;}
void tracked_free(void*p){if(!p)return;for(int i=0;i<64;i++)if(live[i]==p){live[i]=NULL;allocations--;free(p);return;}assert(!"double free or foreign asset");}
uint8_t*xpm_load(xpm_map_t p,enum xpm_image_type type,xpm_image_t*image){assert(type==XPM_8_8_8_8);memset(image,0,sizeof(*image));image->width=image->height=1;image->bytes=tracked_malloc(4);memset(image->bytes,0,4);loads++;return image->bytes;}
int vg_draw_rectangle(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint32_t color){return 0;}int paint_pixel(uint16_t x,uint16_t y,uint32_t color){return 0;}
int main(void){World*world=world_create();assert(world);for(int frame=0;frame<3;frame++)world_render(world);assert(loads==3 && allocations==7 && "repainting the world must reuse the three original sprite assets");destroy_sprites();assert(allocations==1);world_render(world);assert(loads==6 && allocations==7 && "cleaned sprite resources can be loaded for a later round");destroy_sprites();tracked_free(world);assert(allocations==0);puts("Repeated real world renders reuse sprites; cleanup releases them once and permits a later round");}
''', source_flags=['-O2','-Dmalloc=tracked_malloc','-Dfree=tracked_free'])

    def test_pixel_orchard_snake_cleanup_releases_each_owned_node_once(self):
        self.compile_and_run(COMMUNITY/'filipecardoso0--LCOM'/'proj/src', ['lib/model/src/snake_model.c'], r'''#include "lib/model/include/snake_model.h"
#include <assert.h>
#include <stdlib.h>
static void*allocations[64];static bool released[64];static int count;
void*tracked_malloc(size_t bytes){assert(count<64);void*p=malloc(bytes);assert(p);allocations[count++]=p;return p;}
void tracked_free(void*p){if(!p)return;for(int i=0;i<count;i++)if(allocations[i]==p){assert(!released[i] && "snake cleanup must release each owned body node exactly once");released[i]=true;return;}assert(!"snake cleanup must not free borrowed board positions");}
int main(void){position_t cells[4]={{0}};snake_t*single=snake_new(&cells[0]);assert(single);snake_delete(single);snake_t*many=snake_new(&cells[0]);snake_increase_size(many,&cells[1]);snake_increase_size(many,&cells[2]);snake_increase_size(many,&cells[3]);snake_delete(many);for(int i=0;i<count;i++){assert(released[i] && "every allocated node and snake must be released");free(allocations[i]);}puts("One-cell and multi-cell snakes release every owned node once while preserving borrowed positions");}
''', source_flags=['-O2','-Dmalloc=tracked_malloc','-Dfree=tracked_free'])

    def test_pixel_orchard_requests_only_the_initialized_video_range(self):
        self.compile_and_run(COMMUNITY/'filipecardoso0--LCOM'/'proj/src', ['drivers/graphic/src/graphic.c'], r'''#include "drivers/graphic/include/graphic.h"
#include <assert.h>
#include <string.h>
static bool queried;static int permissions,mappings,mode_sets;static uint8_t pixels[144];
int vbe_get_mode_info(uint16_t mode,vbe_mode_info_t*info){assert(mode==0x115);memset(info,0,sizeof(*info));info->XResolution=8;info->YResolution=6;info->BitsPerPixel=24;info->PhysBasePtr=0xe0000000;queried=true;return 0;}
int sys_privctl(endpoint_t who,int request,void*data){assert(queried && "query mode before requesting its initialized physical range");struct minix_mem_range*range=data;assert(who==SELF && request==SYS_PRIV_ADD_MEM);assert(range->mr_base==0xe0000000u && range->mr_limit==0xe0000090u);permissions++;return 0;}
void*vm_map_phys(endpoint_t who,void*address,size_t bytes){assert(permissions==1 && "only one initialized permission request is needed");assert(who==SELF && (uintptr_t)address==0xe0000000u && bytes==sizeof(pixels));mappings++;return pixels;}
int (sys_int86)(reg86_t*r){assert(mappings==1);assert(r->intno==0x10 && r->ax==0x4f02 && r->bx==0x4115);mode_sets++;return 0;}
void panic(const char*format,...){assert(!"valid initialized video range should not fail");}
int main(void){assert(vg_init(0x115)==pixels);assert(permissions==1 && mappings==1 && mode_sets==1);assert(get_hres()==8 && get_vres()==6);puts("Original video setup requests one valid physical range after mode discovery and maps the exact framebuffer");}
''', source_flags=['-O2'])
    def test_tron_maps_and_presents_bytes_not_bits(self):
        self.compile_and_run(COMMUNITY/'j-seixas--feup-lcom/proj/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "video_gr.c"
#define FRAME_BYTES (1280u*1024u*2u)
static unsigned char*frame;
int lcom_compat_legacy_int86(struct reg86u*r){return 0;}
int vbe_get_mode_info(unsigned short mode,vbe_mode_info_t*info){memset(info,0,sizeof*info);info->XResolution=1280;info->YResolution=1024;info->BitsPerPixel=16;info->PhysBasePtr=0xe0000000;return 0;}
int sys_privctl(endpoint_t ep,int request,void*arg){struct mem_range*range=arg;assert(range->mr_base==0xe0000000u&&range->mr_limit-range->mr_base==FRAME_BYTES);return 0;}
void*vm_map_phys(endpoint_t ep,void*address,size_t length){assert(length==FRAME_BYTES);return frame;}
void panic(const char*format,...){abort();}
int main(void){size_t page=(size_t)sysconf(_SC_PAGESIZE);assert(FRAME_BYTES%page==0);frame=mmap(NULL,FRAME_BYTES+page,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);assert(frame!=MAP_FAILED);assert(mprotect(frame+FRAME_BYTES,page,PROT_NONE)==0);assert(vg_init(0x11a)==frame);memset(vg_vd_get_vbuff(),0x5a,FRAME_BYTES);paint_buff();for(size_t i=0;i<FRAME_BYTES;++i)assert(frame[i]==0x5a);free(vg_vd_get_vbuff());munmap(frame,FRAME_BYTES+page);return 0;}
''')

    def test_dodgem_graphics_uses_owning_video_pointer_declaration(self):
        self.compile_and_run(COMMUNITY/'dansreis--LCOM-DODGEM/DODGEM/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "graphics.c"
static uint16_t frame[1024u*768u];
void *vg_init(unsigned short mode){assert(mode==0x117);return frame;}
int vg_exit(void){return 0;}
int main(void){graphicsStart(0x117);assert((void*)getVideoMem()==frame);assert(getVideoBuffer()!=NULL&&getTripleBuffer()!=NULL);set_pixel(17,23,0x5abc);assert(frame[23u*1024u+17u]==0x5abc);free(getVideoBuffer());free(getTripleBuffer());return 0;}
''')

    def test_dodgem_mouse_decodes_all_signed_ps2_nine_bit_motion(self):
        self.compile_and_run(COMMUNITY/'dansreis--LCOM-DODGEM/DODGEM/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "mouse.c"
static void packet(int dx,int dy,unsigned char buttons){unsigned char p[3]={0x08|buttons|(dx<0?0x10:0)|(dy<0?0x20:0),(unsigned char)dx,(unsigned char)dy};print_mouse(p);}
int main(void){StartMouse();const int values[]={-255,-200,-128,-127,-1,0,1,127,128,200,255};for(unsigned int i=0;i<sizeof(values)/sizeof(*values);++i){int d=values[i];rato->x=500;rato->y=384;packet(d,0,0);assert(rato->x==500+d&&rato->y==384);rato->x=500;rato->y=384;packet(0,d,0);assert(rato->x==500&&rato->y==384-d);}rato->x=5;rato->y=5;packet(-255,255,1);assert(rato->x==0&&rato->y==0&&rato->button==1);rato->x=1000;rato->y=740;packet(255,-255,2);assert(rato->x==1004&&rato->y==748&&rato->button==2&&rato->lastButton==1);packet(0,0,0);assert(rato->button==0&&rato->lastButton==2);free(rato);return 0;}
''')

    def test_stick_hero_checks_the_returned_framebuffer_mapping(self):
        self.compile_and_run(COMMUNITY/'rendoir--feup-lcom/src',['graphics.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "graphics.h"
static unsigned char pixels[800*600*2];
static int failures, maps, fail_map;
int sys_privctl(endpoint_t endpoint,int request,void *arg){struct mem_range *range=arg;assert(endpoint==SELF&&request==SYS_PRIV_ADD_MEM&&range->mr_base==0xe0000000u&&range->mr_limit==0xe00ea600u);return OK;}
void *vm_map_phys(endpoint_t endpoint,void *base,size_t bytes){assert(endpoint==SELF&&(uintptr_t)base==0xe0000000u&&bytes==sizeof pixels);maps++;return fail_map?MAP_FAILED:pixels;}
void panic(const char *format,...){failures++;}
int main(void){vbe_mode_info_t info={0};info.XResolution=800;info.YResolution=600;info.BitsPerPixel=16;info.PhysBasePtr=0xe0000000u;
assert(vramMap(&info)==pixels&&maps==1&&failures==0);
fail_map=1;assert(vramMap(&info)==NULL&&maps==2&&failures==1);
return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-ftrivial-auto-var-init=zero'])

    def test_asteroid_watch_rotation_allocates_every_output_pixel(self):
     self.compile_and_run(COMMUNITY/'jfcaraujo--LCOM/proj/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "game.h"
static size_t requested;static unsigned char output[4*3*3+16];
static void*pixel_malloc(size_t n){requested=n;assert(n==36&&"rotation allocation is full image size, not sizeof(size_t)");memset(output,0xa5,sizeof output);return output;}
#define malloc pixel_malloc
#include "rotation.c"
#undef malloc
int main(void){Game g={0};g.bytesPerPixel=3;g.IGNORE_COLOR=0xffc0cb;unsigned char input[36];for(unsigned i=0;i<36;i++)input[i]=i+1;xpm_image_t image={.width=4,.height=3,.bytes=input};unsigned char*r=rotate_sprite(&g,image,acos(-1)/2);assert(r==output&&requested==36);for(unsigned i=36;i<sizeof output;i++)assert(output[i]==0xa5);return 0;}
''')

    def test_asteroid_watch_sprite_rendering_clips_both_frame_edges(self):
     self.compile_and_run(COMMUNITY/'jfcaraujo--LCOM/proj/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "graphics.c"
int main(void){unsigned char storage[16+4*3*3+16],pixels[18]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};xpm_image_t im={.width=3,.height=2,.bytes=pixels};h_res=4;v_res=3;bytesPerPixel=3;second_buffer=storage+16;memset(storage,0xa5,sizeof storage);print_sprite(-1,-1,im);assert(!memcmp(second_buffer,pixels+12,6));for(unsigned i=0;i<16;i++)assert(storage[i]==0xa5);for(unsigned i=16+36;i<sizeof storage;i++)assert(storage[i]==0xa5);memset(storage,0xa5,sizeof storage);print_sprite(3,2,im);assert(!memcmp(second_buffer+33,pixels,3));for(unsigned i=16+36;i<sizeof storage;i++)assert(storage[i]==0xa5);return 0;}
''')

    def test_asteroid_watch_exit_releases_each_font_asset_once(self):
     import re
     project=COMMUNITY/'jfcaraujo--LCOM/proj/src';source=(project/'game.c').read_text();start=source.index('int exit_game(Game* game)');block=source[start:];fields=re.findall(r'free\(game->([A-Za-z0-9_]+)\.bytes\)',block);names=list(dict.fromkeys(fields))
     body=r'''#include <lcom/lcf.h>
#include <assert.h>
#include "game.h"
static void*seen[128];static unsigned count;
static void unique_free(void*p){for(unsigned i=0;i<count;i++)assert(seen[i]!=p&&"each original font image is released once");seen[count++]=p;}
int mouse_unsubscribe(void){return 0;}int keyboard_unsubscribe_int(void){return 0;}int timer_unsubscribe_int(void){return 0;}int write_mouse_command(u32_t c){(void)c;return 0;}int vg_exit(void){return 0;}
#define free unique_free
'''+block+r'''
#undef free
int main(void){Game game={0};Game*g=&game;
'''
     for i,name in enumerate(['mouse','ship','playerMov','gameplay','stats','transition','name','fireballs','asteroids','highscores']):body+='g->'+name+'=(void*)(uintptr_t)'+str(100+i)+';\n'
     for i,name in enumerate(names):body+='g->'+name+'.bytes=(void*)(uintptr_t)'+str(1000+i)+';\n'
     body+='assert(exit_game(g)==0);return 0;}\n';self.compile_and_run(project,[],body)

    def test_asteroid_watch_date_fields_reserve_the_terminator(self):
     project=COMMUNITY/'jfcaraujo--LCOM/proj/src';source=(project/'highscores.c').read_text();start=source.index('void (print_date)');end=source.index('void (print_highscores)',start);block=source[start:end]
     self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdarg.h>
#include "game.h"
static unsigned printed;static int checked_format(char*dst,size_t n,const char*fmt,...){char full[64];va_list args;va_start(args,fmt);int r=vsnprintf(full,sizeof full,fmt,args);va_end(args);assert(r>=0&&(size_t)r<n&&"formatted date needs room for terminating NUL");memcpy(dst,full,r+1);return r;}
#define sprintf(d,...) checked_format(d,__builtin_object_size(d,0),__VA_ARGS__)
#define snprintf(d,n,...) checked_format(d,n,__VA_ARGS__)
void print_string(Game*g,int x,int y,char*s,unsigned n){(void)g;(void)x;(void)y;assert(n==2);assert(s[0]>='0'&&s[0]<='9'&&s[1]>='0'&&s[1]<='9');printed++;}
int get_number_length(int n){return n<10?1:2;}int print_sprite(int16_t x,int16_t y,xpm_image_t i){(void)x;(void)y;(void)i;return 0;}
'''+block+r'''
int main(void){Game g={0};Date d={.day=31,.month=12,.year=99,.hour=23,.minute=59};print_date(&g,&d,565,275);assert(printed==5);return 0;}
''')

    def test_rush_state_allocates_every_render_layer_before_initialization(self):
     self.compile_and_run(COMMUNITY/'reeckset--LCOM_Minix_2017/rush/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "stateMachine.h"
#include "macros.h"
static unsigned calls;
static void*checked_alloc(size_t n){if(calls++==1)assert(n>=(Z_INDEX_MAX+1)*sizeof(ObjLayer)&&"all six active z layers require storage before initialization");return calloc(1,n?n:1);}
static void*checked_calloc(size_t n,size_t s){return checked_alloc(n*s);}
#define malloc checked_alloc
#define calloc checked_calloc
#include "stateMachine.c"
#include "gameObjects.c"
#undef malloc
#undef calloc
int main(void){gameState_st*g=newGameState(MAIN_MENU,0,3);assert(g&&g->state==MAIN_MENU&&g->currentHP==3);for(unsigned i=0;i<=Z_INDEX_MAX;i++)assert(g->objects[i].size==0);g->objects[Z_INDEX_MAX].size=1;assert(g->objects[0].size==0);for(unsigned i=0;i<=Z_INDEX_MAX;i++)free(g->objects[i].gameObjects);free(g->objects);free(g);return 0;}
''')

    def test_asteroid_watch_name_entry_terminates_before_highscore_copy(self):
     self.compile_and_run(COMMUNITY/'jfcaraujo--LCOM/proj/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "game.h"
static unsigned copies;
static char*checked_name_copy(char*d,const char*s){const char*end=memchr(s,0,13);assert(end&&"authored highscore strcpy requires a terminated twelve-letter name");size_t n=end-s;assert(n<=12);memcpy(d,s,n+1);copies++;return d;}
#define strcpy checked_name_copy
#include "game.c"
#include "highscores.c"
#undef strcpy
Date getCurrentDate(void){Date d={.day=17,.month=10,.year=26,.hour=9,.minute=0};return d;}
int main(void){Stats stats={0};Ship ship={0};GamePlay play={0};char typed[13],saved[13];memset(typed,0xa5,sizeof typed);Name name={.name=typed};HighScore high={.playerName={.name=saved}};Game g={.stats=&stats,.ship=&ship,.gameplay=&play,.name=&name,.highscores=&high};reset_stats(&g);const char*text="ana";for(unsigned i=0;i<3;i++){name.letter=text[i];writing_name(&g);}stats.score=40;write_new_highscore(&g,0);assert(copies==1&&high.playerName.size==3&&!strcmp(saved,"ana"));name.letter='-';writing_name(&g);assert(name.size==2&&!strcmp(typed,"an"));name.letter='z';writing_name(&g);assert(name.size==3&&!strcmp(typed,"anz"));reset_stats(&g);assert(name.size==0&&typed[0]==0);for(unsigned i=0;i<13;i++){name.letter='a';writing_name(&g);}assert(name.size==12&&typed[12]==0);return 0;}
''')

    def test_davidrocha_rtc_decodes_hour_mode_and_pm_before_bcd_conversion(self):
     self.compile_and_run(COMMUNITY/'davidrocha9--FEUP-LCOM',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "rtc.h"
static unsigned selected;static uint32_t registers[128];
int (sys_outb)(int port,uint32_t value){assert(port==0x70);selected=value;assert(selected<128);return 0;}
int (sys_inb)(int port,uint32_t*value){assert(port==0x71);*value=registers[selected];return 0;}
int lcom_compat_usleep(unsigned int micros){(void)micros;return 0;}
#include "rtc.c"
int main(void){const struct{uint8_t mode,raw;unsigned expected;}cases[]={
 {0x00,0x09,9},{0x00,0x12,0},{0x00,0x89,21},{0x00,0x92,12},
 {0x04,9,9},{0x04,12,0},{0x04,0x89,21},{0x04,0x8c,12},
 {0x02,0x00,0},{0x02,0x12,12},{0x02,0x21,21},{0x02,0x23,23},
 {0x06,0,0},{0x06,12,12},{0x06,21,21},{0x06,23,23}};
 unsigned bad_hours=0;for(unsigned i=0;i<sizeof cases/sizeof cases[0];i++){bool binary=cases[i].mode&4;memset(registers,0,sizeof registers);registers[11]=cases[i].mode;registers[4]=cases[i].raw;registers[2]=binary?58:0x58;registers[0]=binary?47:0x47;registers[9]=binary?26:0x26;registers[8]=binary?10:0x10;registers[7]=binary?17:0x17;Date d=read_date();if(d.hour!=cases[i].expected)fprintf(stderr,"mode%02x raw%02x expected%u actual%u\n",cases[i].mode,cases[i].raw,cases[i].expected,d.hour);if(d.hour!=cases[i].expected||d.hour>=24)bad_hours++;assert(d.minute==58&&d.second==47&&d.year==26&&d.month==10&&d.day==17);}assert(bad_hours==0&&"12h PM and midnight decode to valid24h hours");return 0;}
''')

    def test_rush_pause_reset_keeps_the_active_event_queue_alive(self):
     project=COMMUNITY/'reeckset--LCOM_Minix_2017/rush/src'
     def function(source,marker):
      start=source.index(marker);brace=source.index('{',start);end=brace+1;depth=1
      while depth:
       if source[end]=='{':depth+=1
       elif source[end]=='}':depth-=1
       end+=1
      return source[start:end]
     main=(project/'mainGame.c').read_text();state=(project/'stateMachine.c').read_text();engine=(project/'gameEngine.c').read_text()
     reset=function(main,'void resetGame(');keyboard=function(main,'void onKbdEvent(');dispatch=function(state,'gameState_st* eventHandler(');loop=function(engine,'gameState_st* processEvents(')
     self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "gameEngine.h"
#include "mainGame.h"
#include "macros.h"
static ev_queue_st*owner;static bool dispatching;static unsigned owner_frees,node_frees,resets,updates,frames;
static void checked_free(void*p){if(!p)return;if(p==owner){assert(!dispatching&&"reset must not free the queue still held by processEvents and mainGameLoop");owner_frees++;}else node_frees++;free(p);}
#define free checked_free
#include "ev_queue.c"
#undef free
static gameState_st paused={.state=PAUSE_MENU},menu={.state=MAIN_MENU},play={.state=GAME_PLAY};static gameState_st*current=&paused;
static double lastScore,score,g_roadSpeed,prevRoadSpeed;static bool isNight,wasNight,wasPlayBtnHovered,wasMenuBtnHovered;
#define g_maxRoadSpeed 8
void updateCurrentGameState(state_st s){current=s==MAIN_MENU?&menu:s==GAME_PLAY?&play:&paused;}
gameState_st*getCurrentState(void){return current;}
void stateMachine_free(void){resets++;}
void startStates(ev_queue_st*q,int hp){assert(q==owner&&q->size==0&&hp==3);current=&menu;}
void onMouseEvent(gameState_st*s,event_st e,ev_queue_st*q){(void)s;(void)e;(void)q;assert(0);}
void update(ev_queue_st*q,gameState_st*s){assert(q==owner&&q->size==0&&s==current);updates++;}
void drawFrame(gameState_st*s){assert(s==current);frames++;}
'''+reset+'\n'+keyboard+'\n'+dispatch+'\n'+loop+r'''
int main(void){owner=ev_queue_create();event_st back={.eventType=KEY_PRESS,.keyCode=0xb9},stale={.eventType=KEY_PRESS,.keyCode=0x81};assert(ev_queue_push(owner,back)&&ev_queue_push(owner,stale));dispatching=true;gameState_st*s=processEvents(owner,current);dispatching=false;assert(s==&menu&&current==&menu&&resets==1&&owner_frees==0&&node_frees==2&&owner->size==0&&!owner->begin&&!owner->end);assert(ev_queue_push(owner,back));dispatching=true;s=processEvents(owner,current);dispatching=false;assert(s==&play&&owner->size==0&&node_frees==3&&updates==2&&frames==2);ev_queue_free(owner);assert(owner_frees==1);return 0;}
''')

    compile_and_run = existing.StudentSourceRegressions.compile_and_run

    def test_minehunter_decodes_full_ps2_nine_bit_delta_before_clamping(self):
        self.compile_and_run(COMMUNITY/'PedroCastro--FEUP-LCOM-Minehunter/proj/code',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include "ModRato.c"
int main(void){Rato*mouse=getRato();const int deltas[]={-255,-200,-128,-127,-1,0,1,127,128,200,255};for(unsigned i=0;i<sizeof deltas/sizeof deltas[0];i++){int delta=deltas[i];mouse->x=500;mouse->y=380;mouse->packets[0]=8|(delta<0?BIT(4)|BIT(5):0);mouse->packets[1]=(unsigned char)delta;mouse->packets[2]=(unsigned char)delta;updateMouse();assert(mouse->x==500+delta);assert(mouse->y==380-delta);}free(mouse);return 0;}
''')

    def test_minehunter_failed_bios_setup_returns_null(self):
        self.compile_and_run(COMMUNITY/'PedroCastro--FEUP-LCOM-Minehunter/proj/code',['ModGrafico.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "ModGrafico.h"
#include <assert.h>
int lcom_compat_legacy_int86(struct reg86u*regs){return 1;}
int sys_privctl(endpoint_t ep,int request,void*arg){abort();}
void*vm_map_phys(endpoint_t ep,void*address,size_t length){abort();}
void panic(const char*format,...){abort();}
int main(void){assert(vg_init(0x117)==NULL);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Wno-implicit-function-declaration'])


    def test_card_memory_applies_each_complete_mouse_packet_once(self):
        self.compile_and_run(COMMUNITY/'pipe-01--LCOM-FEUP'/'proj/src',['proj.c'],r'''#include "game.h"
#include "mouse.h"
#include "rtc.h"
#include <assert.h>
#include <setjmp.h>
uint8_t scancode,read_packet;vbe_mode_info_t modeinfo;int tmr_tck_counter,counter;struct packet packet1;Mouse mouse;Date date;Player p1;card cardArray[16];bool reading_error;static jmp_buf finish;static int calls,applied,event;
xpm_image_t imgGameName,imgDeck,imgCardBack,imgMouse,imgHeart,imgVictory,imgGameOver,imgInstructions,imgMoon,imgSun,imgBackground;
uint8_t *spriteGameName,*spriteDeck,*spriteCardBack,*spriteMouse,*spriteHeart,*spriteVictory,*spriteGameOver,*spriteInstructions,*spriteMoon,*spriteSun,*spriteBackground;
uint8_t*xpm_load(xpm_map_t p,enum xpm_image_type t,xpm_image_t*i){static uint8_t pixel[4];return pixel;}
void initialize_game_cards(card*a){}void initializePlayer(void){}void create_mouse(void){}int get_date(void){return 0;}int vg_start(uint16_t m){return 0;}
int kbc_subscribe(uint8_t*b){return 0;}int timer_subscribe_int(uint8_t*b){return 0;}int mouse_subscribe(uint8_t*b){return 0;}int mouse_enable_data_reporting(void){return 0;}
int kbc_unsubscribe(void){return 0;}int timer_unsubscribe_int(void){return 0;}int mouse_unsubscribe(void){return 0;}int disable_mouse_data(void){return 0;}int vg_exit(void){return 0;}
unsigned lcom_compat_sleep(unsigned seconds){return 0;}
void clear_buffer(void){}void kbc_ih(void){}void mouse_ih(void){}void timer_int_handler(void){}
void greenScreen(void){}void drawSun(void){}void drawMoon(void){}void draw_menu(void){}void draw_game_cards(void){}void drawHearts(void){}void draw_pause(void){}void drawInstructions(void){}void draw_game_over(void){}void draw_victory(void){}void draw_mouse(void){}void flipCardOnClick(void){}int my_updateVideoMem(void){return 0;}
void print_mouse_packet(void){counter=(counter+1)%3;if(!counter){packet1.delta_x=12;packet1.delta_y=-6;}}
void update_mouse_position(int x,int y){calls++;applied+=x;}
int driver_receive(endpoint_t src,message*m,int*status){if(event++==6)longjmp(finish,1);*status=IPC_STATUS_NOTIFY;m->m_source=HARDWARE;m->m_notify.interrupts=4;return 0;}
int main(void){if(!setjmp(finish))proj_main_loop(0,NULL);assert(calls==2 && applied==24 && "two complete packets must move exactly twice, never replaying deltas on partial bytes");puts("Card memory applies each completed mouse motion exactly once");}
''',source_flags=['-O2','-Dmain=student_entry'])


    def test_mosu_renderer_clips_framebuffer_edges_and_preserves_valid_pixels(self):
        self.compile_and_run(COMMUNITY/'Rockdell--2LCOM-MOsu'/'src',['video_gr.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "vbe.h"
#include "video_gr.h"
#include <assert.h>
#include <limits.h>
#include <string.h>
static unsigned char allocation[44],vram[12];
void*guarded_malloc(size_t n){assert(n==12);memset(allocation,0xa5,sizeof(allocation));memset(allocation+16,17,n);return allocation+16;}
int lcom_compat_legacy_int86(struct reg86u*r){assert(r->u.w.ax==0x4f02);r->u.b.ah=0;return 0;}
int vbe_get_mode_info(unsigned short mode,vbe_mode_info_t*info){assert(mode==0x105);memset(info,0,sizeof(*info));info->XResolution=4;info->YResolution=3;info->BitsPerPixel=8;info->PhysBasePtr=0xe0000000;return 0;}
int sys_privctl(endpoint_t who,int request,void*range){return 0;}
void*vm_map_phys(endpoint_t who,void*address,size_t size){assert(size==sizeof(vram));return vram;}
void panic(const char*format,...){assert(!"unexpected graphics setup failure");}
int main(void){assert(vg_init(0x105)==vram);vg_paintPixel(0,0,7);vg_paintPixel(3,2,36);unsigned short clipped[][2]={{4,0},{4,2},{0,3},{3,3},{4,3},{5,10},{USHRT_MAX,0},{0,USHRT_MAX}};for(size_t i=0;i<sizeof(clipped)/sizeof(*clipped);i++)vg_paintPixel(clipped[i][0],clipped[i][1],63);for(int i=0;i<16;i++)assert(allocation[i]==0xa5 && allocation[28+i]==0xa5 && "clipped pixels must not write outside the framebuffer allocation");vg_nextFrame();for(int i=0;i<12;i++)assert(vram[i]==(i==0?7:i==11?36:17) && "valid corner pixels render; clipped right edge must not wrap into the next row");puts("Original renderer preserves valid corner colors and clips all width/height edges before address calculation");}
''',source_flags=['-O2','-DLCOM_COMPAT_LEGACY=1','-include','lcom/compat.h','-Wno-implicit-function-declaration','-Dmalloc=guarded_malloc'])


    def test_hawktype_empty_timeout_and_fast_round_have_valid_statistics(self):
        self.compile_and_run(COMMUNITY/'DanielBelem--LCOM-PROJ'/'proj/src',['main.c'],r'''#include "graphics.h"
#include <assert.h>
#include <stdarg.h>
#include <string.h>
vbe_mode_info_t cur_mode_info={.XResolution=800,.YResolution=600};uint8_t cur_scancode;extern int game_time,timer_counter;static int scenario,event,stats,started,used,wpm,accuracy;static uint8_t next_key;
int record_printf(const char*format,...){va_list a;va_start(a,format);if(!strcmp(format,"Game started with %d seconds\n")){int duration=va_arg(a,int);assert(duration==(scenario?15:60));started++;}else if(!strcmp(format,"used time: %d\n"))used=va_arg(a,int);else if(!strcmp(format,"wpm: %d\n"))wpm=va_arg(a,int);else if(!strcmp(format,"accuracy: %d.%d%%\n")){int whole=va_arg(a,int),decimal=va_arg(a,int);assert(whole==0 && decimal==0);accuracy++;stats++;}va_end(a);return 0;}
int set_frame_buffer(uint16_t mode){return 0;}int set_graphic_mode(uint16_t mode){return 0;}int vg_exit(void){return 0;}
int draw_rectangle(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint32_t c){return 0;}int draw_xpm_title(const char*s,uint16_t x,uint16_t y){return 0;}int draw_xpm_sentence(const char*s,uint16_t x,uint16_t y,const char*c){return 0;}int draw_xpm_cursor(uint16_t x,uint16_t y){return 0;}int draw_xpm_numbers(int n,uint16_t x,uint16_t y){return 0;}void swap_buffers(void){}void free_buffers(void){}
int draw_button_15(int x,int y){return 0;}int draw_button_30(int x,int y){return 0;}int draw_button_60(int x,int y){return 0;}int draw_button_eng(int x,int y){return 0;}int draw_button_por(int x,int y){return 0;}int draw_button_esp(int x,int y){return 0;}
int keyboard_subscribe_int(uint8_t*b){*b=2;return 0;}int keyboard_unsubscribe_int(void){return 0;}void kbc_ih(void){cur_scancode=next_key;}
int sys_irqsetpolicy(int irq,int policy,int*hook){return 0;}int sys_irqrmpolicy(int*hook){return 0;}int get_kbc_output(uint8_t*v,uint8_t mouse){assert(0);return 1;}
int driver_receive(endpoint_t src,message*m,int*status){assert(event<3700);*status=IPC_STATUS_NOTIFY;m->m_source=HARDWARE;m->m_notify.interrupts=2;next_key=0;int n=event++;if(n==0)next_key=scenario?0x02:0x04;else if(n==1)next_key=0x39;else if(!scenario && n<3602)m->m_notify.interrupts=1;else if(scenario && n<12)next_key=n%2?0x39:0x10;else if(n==(scenario?12:3602))m->m_notify.interrupts=0;else next_key=0x81;return 0;}
int main_interrupt_handler(void);
int main(void){for(scenario=0;scenario<2;scenario++){event=stats=started=accuracy=0;used=wpm=-1;game_time=15;timer_counter=0;cur_scancode=0;assert(main_interrupt_handler()==0);assert(started==1 && stats==1 && accuracy==1);assert(used==(scenario?1:60) && "selected duration governs elapsed time, with a one-second minimum for instant completion");assert(wpm==(scenario?300:0));}puts("Empty 60-second timeout and immediate five-word completion return finite, nonnegative round statistics");}
''',source_flags=['-O2','-Dmain=student_entry','-Dprintf=record_printf'])


    def test_hawktype_draws_only_initialized_word_list(self):
        self.compile_and_run(COMMUNITY/'DanielBelem--LCOM-PROJ'/'proj/src',['main.c'],r'''#include "graphics.h"
#include <assert.h>
#include <string.h>
struct words{char word[MAX_WORD_SIZE];int state;};extern struct words word_list[MAX_GAME_WORDS];vbe_mode_info_t cur_mode_info={.XResolution=800,.YResolution=600};static int draws;
size_t checked_strlen(const char*s){bool member=false;for(int i=0;i<MAX_GAME_WORDS;i++)if(s==word_list[i].word)member=true;assert(member && "sentence rendering cannot read beyond the initialized word array");return strlen(s);}
int draw_xpm_sentence(const char*s,uint16_t x,uint16_t y,const char*color){assert(!strcmp(color,"default"));assert(y<600);draws++;return 0;}
int draw_sentence(void);
int main(void){for(int i=0;i<MAX_GAME_WORDS;i++)strcpy(word_list[i].word,"word");assert(draw_sentence()==0 && draws==MAX_GAME_WORDS);puts("Every displayed word belongs to the initialized game word list");}
''',source_flags=['-O2','-Dmain=student_entry','-Dstrlen=checked_strlen'])


    def test_huetris_subscribes_before_enabling_and_consumes_delayed_mouse_ack(self):
        self.compile_and_run(COMMUNITY/'edualm--LCOM-Huetris/code', ['scene.c','mouse.c','kbc.c'],r'''#define LCOM_COMPAT_LEGACY 1
#define LCOM_COMPAT_LEGACY_LM_STATUS 1
#include <lcom/compat.h>
#include "scene.h"
#include "mouse.h"
#include <assert.h>
static int owned,writes,polls,reads,delays;static unsigned reply=0xfa;
void board_mouseHandler(void){}
int int_subscribe(int irq,int policy,void(*callback)()){assert(irq==12 && policy==(IRQ_REENABLE|IRQ_EXCLUSIVE));owned=1;return 2;}
int sys_outb(int port,uint32_t value){if(writes++==0)assert(port==0x64 && value==0xd4);else{assert(port==0x60 && value==0xf4);assert(owned);}return 0;}
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){if(port==0x64){*value=writes<2 || polls++<2?0:0x21;return 0;}assert(port==0x60);assert(writes==2 && reads++==0);*value=reply;return 0;}
int tickdelay(clock_t ticks){delays++;return 0;}uint32_t micros_to_ticks(uint32_t usecs){return usecs;}
int main(void){assert(scene_initMouse()==2);assert(owned && writes==2 && reads==1 && delays>=2);writes=polls=reads=delays=0;reply=0xfe;assert(mouse_enable()!=0);assert(writes==2 && reads==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-Wno-implicit-function-declaration'])

    def test_huetris_inactive_screens_drain_mouse_bytes_before_keyboard(self):
        self.compile_and_run(COMMUNITY/'edualm--LCOM-Huetris/code',['interrupt.c'],r'''#define LCOM_COMPAT_LEGACY 1
#define LCOM_COMPAT_LEGACY_LM_STATUS 1
#include <lcom/compat.h>
#include "interrupt.h"
#include <assert.h>
static int step,reads,selected;
int sys_irqsetpolicy(int irq,int policy,int*hook){*hook+=10;return 0;}int sys_irqenable(int*hook){return 0;}int sys_irqdisable(int*hook){return 0;}int sys_irqrmpolicy(int*hook){return 0;}
int driver_receive(endpoint_t source,message*m,int*status){*status=IPC_STATUS_NOTIFY;m->m_source=HARDWARE;if(step++<3)m->NOTIFY_ARG=BIT(2);else{assert(reads==3);m->NOTIFY_ARG=BIT(1);}return 0;}
int mouse_read(unsigned long*value){assert(reads<3);*value=(unsigned[]){0x08,12,0}[reads++];return 0;}
void sceneMainMenu_keyboardHandler(void){selected++;int_stop_handler();}void sceneEndGame_keyboardHandler(void){selected++;int_stop_handler();}
void board_timerHandler(void){abort();}void board_keyboardHandler(void){abort();}void board_mouseHandler(void){abort();}void timer_int_handler(void){abort();}
int main(void){int modes[]={kInterruptModeMainMenu,kInterruptModeEndGame};for(unsigned i=0;i<2;i++){step=reads=selected=0;int_init();assert(int_subscribe(0,IRQ_REENABLE,0)==0);assert(int_subscribe(1,IRQ_REENABLE,0)==1);assert(int_subscribe(12,IRQ_REENABLE,0)==2);int_setMode(modes[i]);assert(int_start_handler()==0);assert(reads==3 && selected==1);}return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-Wno-implicit-function-declaration'])

    def test_huetris_board_drains_inactive_keyboard_and_solo_mouse_input(self):
        self.compile_and_run(COMMUNITY/'edualm--LCOM-Huetris/code',['board.c'],r'''#define LCOM_COMPAT_LEGACY 1
#define LCOM_COMPAT_LEGACY_LM_STATUS 1
#include <lcom/compat.h>
#include "board.h"
#include "tetriminos.h"
#include <assert.h>
void board_init(void);void board_setPlayers(int);
static unsigned keys,mouse;
int keyboard_read(void){keys++;return 0x82;}
int mouse_read(unsigned long*value){*value=(unsigned[]){0x08,12,0}[mouse++%3];return 0;}
void scene_cleanScreen(void){abort();}void scene_endGame(int p,int s){abort();}
void sceneSingle_drawControls(void){abort();}void sceneSingle_drawBoard(void){abort();}
void sceneMultiple_drawControls(void){abort();}void sceneMultiple_drawBoard(void){abort();}
int vg_exit(void){abort();}
tetrimino tetriminos_get(void){abort();}
int main(void){board_init();board_setPlayers(1);assert(!board_pieceInGame(kBoardOne));board_keyboardHandler();assert(keys==1);for(unsigned i=0;i<3;i++)board_mouseHandler();assert(mouse==3);assert(!board_pieceInGame(kBoardOne));return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-Wno-implicit-function-declaration'])


    def test_blackjack_dealer_completion_resolves_without_keyboard(self):
        self.compile_and_run(COMMUNITY/'PedroRomao3--Computer-Laboratory_FEUP',['controller/control_menu.c','model/player.c'],r'''#include "controller/control_menu.h"
#include <assert.h>
GameState state=STAND;bool isMulti=false;uint8_t scancode=0x39;static int outcomes;
int drawMainMenu(void){return 0;}int draw_wait(void){return 0;}int drawNoWinner(void){outcomes++;return 0;}int drawWin(void){outcomes++;return 0;}int drawLoose(void){outcomes++;return 0;}int send_char(uint8_t byte){return 0;}
int main(void){p1.c1=17;p1.c2=13;pc.c1=1;pc.c2=0;g_coins.coin5=1;int balance=p1.money;ki_menu();assert(state==STAND && "a key cannot resolve before the dealer finishes");assert(ti_menu()==0 && state==STAND && outcomes==0);pc.c2=5;assert(ti_menu()==0 && state==WIN && outcomes==1 && "completed dealer hand resolves on normal timer draw");assert(p1.money==balance+10);assert(ti_menu()==0 && outcomes==2 && p1.money==balance+10);isMulti=true;state=STAND;p1.c1=17;p1.c2=13;p2.c1=1;p2.c2=5;assert(ti_menu()==0 && state==STAND);allow_stand();assert(ti_menu()==0 && state==WIN);puts("Pending dealer cards cannot resolve early; completed hands settle without an extra key, with paired readiness retained");}
''',source_flags=['-O2'])


    def test_connect4_source_contract(self):
        self.compile_and_run(COMMUNITY/'fernandorego--feup-lcom-connect4'/'proj/src',['lib/devices/rtc.c'],r'''#include "lib/devices/rtc.h"
#include <lcom/lcf.h>
#include <assert.h>
#include <string.h>
static int reg_index;
int sys_outb(int port,uint32_t value){if(port==RTC_ADDR_REG)reg_index=value;return 0;}
int util_sys_inb(int port,uint8_t*value){const uint8_t values[]={59,0,58,0,23,0,0,16,10,26,0,0,0,0};*value=values[reg_index];return 0;}
int sys_irqsetpolicy(int a,int b,int*c){return 0;}int sys_irqrmpolicy(int*a){return 0;}
int tickdelay(clock_t t){return 0;}uint32_t micros_to_ticks(uint32_t m){return m;}
int main(void){rtc_handle_update_int();assert(strcmp(rtc_get_current_date(),"16/10/2026 23:58:59\n")==0);puts("RTC date fits with terminator");return 0;}
''',source_flags=["-O2", "-D_FORTIFY_SOURCE=2"])


    def test_galactic_scores_strip_newline_and_bound_top_ten(self):
        self.compile_and_run(COMMUNITY/'golangis--LCOM-SpaceInvaders'/'proj/src', ['logic/spaceinvaders.c'], r'''#include "logic/spaceinvaders.h"
#include <assert.h>
#include <string.h>
static uint8_t *allocation;static size_t payload;
void *guarded_calloc(size_t count,size_t size){payload=count*size;allocation=calloc(1,payload+64);memset(allocation+payload,0xa5,64);return allocation;}
FILE *lcom_compat_fopen(const char*path,const char*mode){assert(strcmp(mode,"r")==0);FILE*f=tmpfile();assert(f);for(int i=0;i<12;i++)fprintf(f,"%d,28/05/2023 20:34\n",69-i);rewind(f);return f;}
int main(void){Score*scores=load_scores();assert(scores);for(int i=0;i<10;i++){assert(scores[i].points==69-i);assert(strcmp(scores[i].datetime,"28/05/2023 20:34")==0);}assert(allocation && payload==10*sizeof(Score));for(int i=0;i<64;i++)assert(allocation[payload+i]==0xa5);free(scores);puts("CSV newline never overflows timestamp; records stay inside top ten allocation");}
''', source_flags=['-O2','-D_FORTIFY_SOURCE=2','-Dcalloc=guarded_calloc'])

    def test_galactic_shot_arrays_own_only_allocation_bases(self):
        self.compile_and_run(COMMUNITY/'golangis--LCOM-SpaceInvaders'/'proj/src', ['logic/alien.c','logic/player.c','logic/shot.c'], r'''#include "logic/alien.h"
#include "logic/player.h"
#include <assert.h>
#include <string.h>
static void *live[128];static int live_count;
void *tracked_malloc(size_t size){void*p=malloc(size);assert(p);memset(p,0xa5,size);for(int i=0;i<128;i++)if(!live[i]){live[i]=p;live_count++;return p;}assert(0);return NULL;}
void tracked_free(void*p){if(!p)return;for(int i=0;i<128;i++)if(live[i]==p){live[i]=NULL;live_count--;free(p);return;}assert(!"free must receive a live allocation base");}
int main(void){
 Alien a=init_alien(0,100,200);assert(a.shots_no==0 && a.shots==NULL);
 Player*p=init_player();assert(p && p->shots_no==0 && p->shots==NULL);
 for(int cycle=0;cycle<100;cycle++){
  for(int i=0;i<4;i++){a.x_min=100+10*i;p->x_min=300+10*i;shoot_alien(&a);fire(p);}
  assert(a.shots_no==4 && p->shots_no==4);
  delete_alien_shot(&a,1);delete_player_shot(p,1);
  assert(a.shots_no==3 && p->shots_no==3);
  assert(a.shots[0].x_min==110 && a.shots[1].x_min==130 && a.shots[2].x_min==140);
  assert(p->shots[0].x_min==310 && p->shots[1].x_min==330 && p->shots[2].x_min==340);
  delete_alien_shot(&a,0);delete_player_shot(p,0);
  assert(a.shots[0].x_min==130 && p->shots[0].x_min==330);
  delete_alien_shot(&a,1);delete_player_shot(p,1);
  assert(a.shots_no==1 && p->shots_no==1);
  delete_alien_shot(&a,0);delete_player_shot(p,0);
  assert(!a.shots && !p->shots && a.shots_no==0 && p->shots_no==0);
  assert(live_count==1);
 }
 tracked_free(p);assert(live_count==0);puts("Alien/player firing and first/middle/last removal retain valid arrays without leaks");
}
''', source_flags=['-O2','-ftrivial-auto-var-init=pattern','-Dmalloc=tracked_malloc','-Dfree=tracked_free'])

    def test_wordle_framebuffer_pages_use_bytes(self):
        self.compile_and_run(COMMUNITY/'Belem17--lcom-wordle'/'project/src', ['drivers/videogr.c'], r'''#include "drivers/graphics.h"
#include <assert.h>
static int mappings;static uintptr_t base=0xe0000000;static size_t page_size=1280*1024*4;
void panic(const char*message,...){(void)message;abort();}
int vbe_get_mode_info(uint16_t mode,vbe_mode_info_t*info){*info=(vbe_mode_info_t){.XResolution=1280,.YResolution=1024,.BitsPerPixel=32,.BytesPerScanLine=5120,.PhysBasePtr=0xe0000000};return 0;}
int sys_privctl(endpoint_t who,int request,void*value){struct minix_mem_range*r=value;assert(r->mr_base==base);assert(r->mr_limit-base==3*page_size);return 0;}
void *vm_map_phys(endpoint_t who,void*address,size_t size){assert(size==page_size);assert((uintptr_t)address==base+mappings*page_size);mappings++;return(void*)0x1000;}
int main(void){assert(vg_int(0x11a)==0);assert(mappings==3);puts("Three 32-bit framebuffer pages occupy15MiB, with contiguous byte offsets");}
''', source_flags=['-O2'])

    def test_wordle_chooses_only_complete_dictionary_words(self):
        self.compile_and_run(COMMUNITY/'Belem17--lcom-wordle'/'project/src', ['game/gameLogic.c'], r'''#include "game/gameLogic.h"
#include <assert.h>
#include <limits.h>
#include <string.h>
extern char*words[1751];static int next_random;
int boundary_rand(void){return next_random;}
char*checked_copy(char*dst,const char*src,size_t size){int member=0;for(size_t i=0;i<1751;i++)if(src==words[i])member=1;assert(member && "selected pointer must belong to the original dictionary");assert(strlen(src)==5 && "selected word must fit all five cells");return strncpy(dst,src,size);}
int main(void){struct Game game={0};next_random=2002;assert(chooseWord(&game)==0);assert(strlen(game.target)==5);for(next_random=0;next_random<2003;next_random++){assert(chooseWord(&game)==0);assert(strlen(game.target)==5);}next_random=INT_MAX;assert(chooseWord(&game)==0 && strlen(game.target)==5);puts("Every random dictionary boundary selects a complete five-letter word");}
''', source_flags=['-O2','-D_FORTIFY_SOURCE=0','-Drand=boundary_rand','-Dstrncpy=checked_copy'])

    def test_wordle_key_release_does_not_erase_pending_letter(self):
        self.compile_and_run(COMMUNITY/'Belem17--lcom-wordle'/'project/src', ['proj.c'], r'''#include "game/gameLogic.h"
#include <assert.h>
static int notifications,key_index,frames;
void initialize(void){}int timer_subscribe_int(uint8_t*b){*b=10;return 0;}int timer_unsubscribe_int(void){return 0;}
int KBC_subscribe_int(uint8_t*b){*b=0;return 0;}int KBC_unsubscribe_int(void){return 0;}int MC_subscribe_int(uint8_t*b){*b=1;return 0;}int MC_unsubscribe_int(void){return 0;}
int mouse_enable_data(void){return 0;}int mouse_disable_data_reporting(void){return 0;}
int vg_enter(uint16_t m){return 0;}int vg_int(uint16_t m){return 0;}int vg_exit(void){return 0;}
void KBC_int_handler(uint8_t*b){assert(key_index<2);*b=(uint8_t[]){0x1e,0x9e}[key_index++];}
void MC_int_handler(uint8_t*b){assert(0);}void Write_to_struct_packet(uint8_t*b,struct packet*p){assert(0);}void updateMouse(struct Game*g,int y,int x){assert(0);}
int driver_receive(endpoint_t e,message*m,int*s){assert(notifications<3);*s=IPC_STATUS_NOTIFY;m->m_source=HARDWARE;m->m_notify.interrupts=1u<<(notifications++<2?0:10);return 0;}
void gameHandler(struct Game*g){assert(key_index==2 && g->signal.key_pressed);assert(g->signal.key_code==0x1e);frames++;g->state=STATE_END;}
int main(void){assert(proj_main_loop(0,NULL)==0);assert(frames==1);puts("Quick letter make/release remains pending until the next game frame");}
''', source_flags=['-O2','-Dmain=student_entry','-Wno-implicit-function-declaration'])

    def test_terraria_mining_and_placement_preserve_inventory_and_array_bounds(self):
        self.compile_and_run(COMMUNITY/'phpc99--lcom-project'/'proj', ['src/Controller/Game/CursorController.c','src/Controller/Game/MapController.c','src/Model/Elements/Element.c','src/Model/Elements/Backpack.c'], r'''#include "src/Controller/Game/CursorController.h"
#include <assert.h>
#include <string.h>
GameState gamestate=GAME_STATE;static Map board;static Player player;static Element *mined;static int live_count;
void *counted_malloc(size_t n){live_count++;return malloc(n);}
Map *new_Map(void){return &board;}void MenuInit(void){}void change_video_mode(uint16_t mode){abort();}
void poison_free(void*p){if(p)live_count--;if(p==mined){memset(p,0xa5,sizeof(Element));return;}free(p);}
void *checked_realloc(void*p,size_t n){assert(n==board.count_elements*sizeof(Element*) && "map pointer array must include every live block");return realloc(p,n);}
int main(void){mode_info.XResolution=1152;mode_info.YResolution=864;player=(Player){.position={192,450},.width=32,.height=64,.backpack=new_Backpack(9)};board.player=&player;board.count_elements=2;board.elements=malloc(2*sizeof(Element*));mined=board.elements[0]=new_Element(ELEMENT_EARTH,(Position){192,512},32,32,false);board.elements[1]=new_Element(ELEMENT_EARTH,(Position){224,512},32,32,false);MapInit();Cursor cursor={.position={200,520}};mouse_reading_t input={0};input.elem.lb=true;ButtonPressed(&cursor,input);assert(board.count_elements==1);assert(player.backpack->slots[0]==ITEM_EARTH && player.backpack->quantity[0]==1 && "mining must collect the block before its storage is freed");cursor.position=(Position){232,496};input.elem.lb=false;input.elem.rb=true;ButtonPressed(&cursor,input);assert(board.count_elements==2);assert(live_count==4 && "placement must not leak its temporary block");Element *placed=board.elements[1];assert(placed->type==ELEMENT_EARTH && placed->position.x==224 && placed->position.y==480);assert(player.backpack->quantity[0]==0 && player.backpack->slots[0]==ITEM_NONE);free(mined);free(board.elements[0]);free(placed);free(board.elements);free(player.backpack);free(get_MapController());puts("Mining collects one earth block; placing consumes it and grows the map safely");}
''', source_flags=['-O2','-Dfree=poison_free','-Drealloc=checked_realloc','-Dmalloc=counted_malloc'])

    def test_penpal_enter_is_not_an_alphabet_glyph(self):
        self.compile_and_run(COMMUNITY/'carlosverissimo3001--FEUP-LC'/'proj/src',['words.c'],r'''#include "words.h"
#include <assert.h>
extern xpm_image_t alphabet[27];static int draws;
int draw_pixmap(uint16_t x,uint16_t y,xpm_image_t*image,bool background){assert(image>=alphabet && image<alphabet+27 && "control characters have no glyph");draws++;return 0;}
int draw_pixmap_fixed_color(uint16_t x,uint16_t y,xpm_image_t*image,uint32_t color,bool background){return draw_pixmap(x,y,image,background);}
int main(void){alphabet[0].width=7;alphabet[25].width=9;alphabet[26].width=3;draw_letter(0,503,590);draw_letter_with_color(0,503,590,0);assert(get_width(0)==0 && draws==0);draw_letter('A',503,590);draw_letter_with_color('Z',503,590,0);draw_letter(' ',503,590);assert(draws==3 && get_width('A')==7 && get_width('Z')==9 && get_width(' ')==3);puts("Enter/NUL has no glyph or width; original letters and spaces retain their artwork");}
''',source_flags=['-O2'])

    def test_thinice_sprite_storage_contains_loaded_dimensions(self):
        self.compile_and_run(COMMUNITY/'jose03pedro--FEUP-LCOM'/'proj/src',['sprite.c'],r'''#include "sprite.h"
#include <assert.h>
#include <string.h>
static unsigned char *allocated;static size_t requested;static uint32_t pixels[24*32];
void *guarded_malloc(size_t n){requested=n;allocated=malloc(n+32);memset(allocated,0xa5,n+32);return allocated;}
uint8_t *xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*image){assert(map && type==XPM_8_8_8_8);image->width=24;image->height=32;return(uint8_t*)pixels;}
int main(void){char*map[]={"24 32 1 1",NULL};Sprite_t*s=sprite_create(map);assert(s && s->width==24 && s->height==32 && s->colors==pixels);for(size_t i=requested;i<requested+32;i++)assert(allocated[i]==0xa5 && "sprite fields must stay inside the allocated object");free(s);puts("Loaded sprite dimensions remain within its owned allocation");}
''',source_flags=['-O2','-Dmalloc=guarded_malloc'])

    def test_knights_check_detection_releases_only_owned_moves(self):
        self.compile_and_run(COMMUNITY/'AngeloOliveira2004--LCOM_Project'/'proj/src',['mvc/model/game.c'],r'''#include "mvc/model/game.h"
#include <assert.h>
#include <string.h>
static void*live[1024];static int count;
void *tracked_malloc(size_t n){void*p=malloc(n);assert(p);memset(p,0xa5,n);for(int i=0;i<1024;i++)if(!live[i]){live[i]=p;count++;return p;}assert(0);return NULL;}
void tracked_free(void*p){if(!p)return;for(int i=0;i<1024;i++)if(live[i]==p){live[i]=NULL;count--;free(p);return;}assert(!"check detection must never free an inline stack array or borrowed board piece");}
int main(void){struct Game game={0};init_board(&game.board);game.isWhiteTurn=true;for(int i=0;i<40;i++){assert(!is_check(&game));assert(count==0 && "temporary candidate moves must not leak");}memset(&game,0,sizeof(game));game.isWhiteTurn=true;game.board.pieces[0]=(struct Piece){.type=ROOK,.position={0,0},.isAlive=true,.isWhite=true};game.board.pieces[1]=(struct Piece){.type=KING,.position={0,7},.isAlive=true,.isWhite=false};game.board.squares[0][0]=game.board.pieces[0];game.board.squares[0][7]=game.board.pieces[1];for(int i=0;i<40;i++){assert(is_check(&game));assert(count==0 && "early check result must release every candidate move");}puts("Repeated normal and checking chess positions preserve borrowed pieces and release owned moves");}
''',source_flags=['-O2','-Dmalloc=tracked_malloc','-Dfree=tracked_free'])

    def test_aquaboy_level_path_includes_terminator_and_multidigit_level(self):
        self.compile_and_run(COMMUNITY/'davidm-g--Aquaboy-and-Lavagirl-GAME'/'proj/src',['visuals/model.c'],r'''#include "visuals/model.h"
#include <assert.h>
#include <limits.h>
extern int *levelArray;static int requested;
FILE *lcom_compat_fopen(const char*path,const char*mode){char expected[128];snprintf(expected,sizeof(expected),"/home/lcom/labs/proj/src/visuals/levels/level%d.txt",requested);assert(strcmp(path,expected)==0 && strcmp(mode,"r")==0);FILE*f=tmpfile();assert(f);fputs("7 8 9",f);rewind(f);return f;}
int main(void){int cells[1200];levelArray=cells;int levels[]={1,13,INT_MAX};for(int i=0;i<3;i++){requested=levels[i];for(int j=0;j<1200;j++)cells[j]=42;updateArrayWithLevel(requested);assert(cells[0]==7 && cells[1]==8 && cells[2]==9 && cells[3]==42);}puts("Level paths preserve every digit and terminator, and the original level reader loads cells");}
''',source_flags=['-O2','-D_FORTIFY_SOURCE=2'])

    def test_lostcurse_drained_mouse_notification_does_not_block_rendering(self):
        self.compile_and_run(COMMUNITY/'LuisGoncalves05--LostCurseOfMadness'/'proj/src',['drivers/mouse/mouse.c'],r'''#include "drivers/mouse/mouse.h"
#include <assert.h>
extern uint8_t packet_byte;static bool full;static int reads,delays;
int util_sys_inb(int port,uint8_t*value){assert(port==KBC_STATUS_REGISTER);*value=full?0x31:0x10;return 0;}
int kbc_read_command_return(uint8_t*value,bool mouse){reads++;if(!full)return 1;*value=0x29;return 0;}
uint32_t micros_to_ticks(uint32_t us){return 1;}int tickdelay(clock_t ticks){delays++;return 0;}
int main(void){mouse_int_handler();assert(reads==0 && delays==0 && "an ACK drained during setup leaves no mouse payload to poll");full=true;mouse_int_handler();assert(reads==1 && delays==0 && packet_byte==0x29);puts("Drained notification returns immediately; a real mouse packet byte is read once");}
''',source_flags=['-O2'])

    def test_joao_source_contract(self):
        self.compile_and_run(COMMUNITY/'joaoalvesss--feup-lcom'/'src',['devices/timer/timer.c'],r'''#include "devices/timer/timer.h"
#include <lcom/lcf.h>
#include <assert.h>
static int selected;static int writes;static uint32_t ports[12],values[12];
int sys_outb(int port,uint32_t value){ports[writes]=port;values[writes++]=value;return 0;}
int util_sys_inb(int port,uint8_t*value){*value=0x36;return 0;}
int util_get_LSB(uint16_t v,uint8_t*b){*b=v;return 0;}int util_get_MSB(uint16_t v,uint8_t*b){*b=v>>8;return 0;}
int main(void){for(int timer=0;timer<3;timer++){writes=0;assert(timer_set_frequency(timer,60)==0);assert(writes==4);assert(ports[1]==0x43 && values[1]==(0x36|(timer<<6)));assert(ports[2]==0x40+timer && values[2]==0xae);assert(ports[3]==0x40+timer && values[3]==0x4d);}puts("PIT command precedes selected channel divisor");}
''',source_flags=["-O2", "-D_FORTIFY_SOURCE=2"])

    def test_target_ack_source_contract(self):
        self.compile_and_run(COMMUNITY/'g0ncal0--lcom_targetShooting'/'src',['devices/kbc/mouse.c'],r'''#include "devices/kbc/mouse.h"
#include <lcom/lcf.h>
#include <assert.h>
static int reads;
int util_sys_inb(int port,uint8_t*value){if(port==0x64)*value=0x20|1;else{assert(reads<2);*value=(uint8_t[]){0xfa,0x03}[reads++];}return 0;}
int sys_outb(int port,uint32_t value){assert((port==0x60 && value==0xf2)||(port==0x64 && value==0xd4));return 0;}
uint32_t micros_to_ticks(uint32_t u){return 0;}int tickdelay(clock_t t){return 0;}
int write_get_device_id_to_mouse(uint8_t *id);
int main(void){uint8_t id=0;assert(write_get_device_id_to_mouse(&id)==0);assert(id==3);assert(reads==2);puts("Device ID follows consumed mouse ACK");}
''',source_flags=['-O2'])

    def test_mini_queue_source_contract(self):
        self.compile_and_run(COMMUNITY/'Minipoloalex--LCOM-MiniPicty'/'proj/src',['model/queue/queue.c'],r'''#include "model/queue/queue.h"
#include <lcom/lcf.h>
#include <assert.h>
#include <string.h>
static uint8_t *storage;static size_t requested;
void *guarded_malloc(size_t n){uint8_t*p=malloc(n*8+256);memset(p,0xa5,n*8+256);if(n==100){storage=p;requested=n;}return p;}
int main(void){queue_t*q=create_queue(100,1);assert(q);for(uint8_t i=0;i<100;i++)assert(push_queue(q,&i)==0);for(size_t i=requested;i<requested*8;i++)assert(storage[i]==0xa5);uint8_t last=0;assert(queue_get_back(q,&last)==0 && last==99);for(uint8_t i=0;i<100;i++){uint8_t x=255;assert(pop_queue(q,&x)==0 && x==i);}assert(is_empty_queue(q));for(uint8_t i=0;i<7;i++)assert(push_queue(q,&i)==0);assert(queue_get_back(q,&last)==0&&last==6);delete_queue(q);puts("Queue stays inside its byte allocation and preserves FIFO/tail across wrap");}
''',source_flags=['-O2', '-Dmalloc=guarded_malloc'])

    def test_pom_menu_start_releases_background_once(self):
        self.compile_and_run(COMMUNITY/'RicardoSilvaBW--FEUP--LCOM'/'proj', ['src/view/menu.c','src/controller/video/graphics.c'], r'''#include "src/view/menu.h"
#include "src/controller/video/graphics.h"
#include "src/model/sprite.h"
#include <assert.h>
#include <string.h>
FILE *lcom_compat_fopen(const char*p,const char*m){return NULL;}
static void *released[8];static int releases;static uint8_t mapped[800*600*3];uint8_t scancode;
void record_free(void*p){if(!p)return;for(int i=0;i<releases;i++)assert(released[i]!=p);assert(releases<8);released[releases++]=p;}
int vbe_get_mode_info(uint16_t mode,vbe_mode_info_t*info){memset(info,0,sizeof(*info));info->XResolution=800;info->YResolution=600;info->BytesPerScanLine=2400;info->BitsPerPixel=24;return 0;}
int sys_privctl(endpoint_t who,int request,void*p){return 0;}void *vm_map_phys(endpoint_t who,void*p,size_t n){assert(n==sizeof(mapped));return mapped;}
#undef sys_int86
int sys_int86(reg86_t*p){return 0;}
sprite*create_sprite(xpm_map_t x,int a,int b){return NULL;}void destroy_sprite(sprite**s){assert(!*s);}int draw_sprite(sprite*s){assert(0);return 0;}
void draw_text(const char*t,int x,int y){}void draw_text_centered(const char*t,int x,int y){}int text_width(const char*t){return strlen(t)*8;}
int driver_receive(endpoint_t src,message*m,int*status){static int n;assert(n++==0);*status=IPC_STATUS_NOTIFY;m->m_source=HARDWARE;m->m_notify.interrupts=2;return 0;}
void kbc_ih(void){}void process_mouse_interrupt(void){}void mouse_get_pos(int*x,int*y){*x=400;*y=280;}int mouse_get_buttons(void){return 1;}
int main(void){assert(vram_start(0x115)==0);assert(menu_run(1,2,4)==0);assert(releases==2);puts("Menu Start releases each background allocation exactly once");}
''', source_flags=['-O2','-Dfree=record_free'])


    def test_doodle_log_writes_literal_text_and_closes_once(self):
        self.compile_and_run(COMMUNITY/'Filipecordeiro--LCOM/projeto', ['log.c'], r'''#include <stdio.h>
#include <assert.h>
#include <string.h>
static FILE *owned;static int closes;
FILE *lcom_compat_fopen(const char *path,const char *mode){assert(!owned);owned=tmpfile();assert(owned);return owned;}
int checked_close(FILE *file){assert(file==owned && closes++==0);return 0;}
void Log(char *message);
int main(void){Log("100% completed\n");assert(closes==1);fflush(owned);rewind(owned);char text[256]={0};assert(fread(text,1,sizeof(text)-1,owned)>0);assert(strstr(text,"100% completed\n"));fclose(owned);return 0;}
''', source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dfclose=checked_close','-include','lcom/paths.h','-Wformat','-Werror=format-security'])

    def test_bow_failed_bios_initialization_returns_null(self):
        self.compile_and_run(COMMUNITY/'huguinho27--FEUP-LCOM/proj/src', ['video_gr.c'], r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "video_gr.h"
#include "vbe.h"
#include <assert.h>
int lcom_compat_legacy_int86(struct reg86u *regs){return 1;}
int vbe_get_mode_info(unsigned short mode,vbe_mode_info_t *info){abort();}
int sys_privctl(endpoint_t endpoint,int request,void *arg){abort();}
void *vm_map_phys(endpoint_t endpoint,void *address,size_t size){abort();}
void panic(const char *format,...){abort();}
int main(void){assert(vg_init(0x115)==NULL);return 0;}
''', source_flags=['-DLCOM_COMPAT_LEGACY=1', '-Werror=return-type'])

    def test_bow_devices_release_their_own_irq_hooks(self):
        self.compile_and_run(COMMUNITY/'huguinho27--FEUP-LCOM/proj/src',
                             ['timer.c', 'kbd.c', 'mouse.c'], r'''#include <assert.h>
static int active[32];
int sys_irqsetpolicy(int irq, int flags, int *hook) { assert(!active[*hook]); active[*hook]=irq+1; return 0; }
int sys_irqenable(int *hook) { return active[*hook] ? 0 : 1; }
int sys_irqdisable(int *hook) { return active[*hook] ? 0 : 1; }
int sys_irqrmpolicy(int *hook) { if (!active[*hook]) return 1; active[*hook]=0; return 0; }
int timer_subscribe_int(void), kbd_subscribe_int(void), mouse_subscribe_int(void);
int timer_unsubscribe_int(void), kbd_unsubscribe_int(void), mouse_unsubscribe_int(void);
int main(void) {
  for (int round=0;round<2;++round) {
    assert(timer_subscribe_int()==0 && kbd_subscribe_int()==0 && mouse_subscribe_int()==0);
    assert(active[15]==1 && active[16]==2 && active[17]==13);
    assert(mouse_unsubscribe_int()==0 && kbd_unsubscribe_int()==0 && timer_unsubscribe_int()==0);
    for(int i=0;i<32;++i) assert(!active[i]);
  }
  return 0;
}
''', source_flags=['-DLCOM_COMPAT_LEGACY=1', '-Wno-return-mismatch'])


    def test_copter_irq_masks_match_and_unsubscribe_their_own_devices(self):
        self.compile_and_run(COMMUNITY/'NunoRamos--LCOM-Copter/proj/code',
                             ['timer.c', 'keyboard.c', 'mouse.c'], r'''#include <assert.h>
static int active[32];
int sys_irqsetpolicy(int irq, int flags, int *hook) { if(active[*hook]){int i=0;while(active[i])i++;*hook=i;} active[*hook]=irq+1; return 0; }
int sys_irqenable(int *hook) { return active[*hook] ? 0 : 1; }
int sys_irqdisable(int *hook) { return active[*hook] ? 0 : 1; }
int sys_irqrmpolicy(int *hook) { if (!active[*hook]) return 1; active[*hook]=0; return 0; }
int timer_subscribe_int(void), keyboard_subscribe_int(void), mouse_subscribe_int(void);
int timer_unsubscribe_int(void), keyboard_unsubscribe_int(void), mouse_unsubscribe_int(void);
int main(void) {
 for(int round=0;round<2;round++) {
  assert(keyboard_subscribe_int()==(1<<1));
  assert(timer_subscribe_int()==(1<<5));
  assert(mouse_subscribe_int()==(1<<12));
  assert(active[1]==2 && active[5]==1 && active[12]==13);
  assert(mouse_unsubscribe_int()==0 && keyboard_unsubscribe_int()==0 && timer_unsubscribe_int()==0);
  for(int i=0;i<32;i++) assert(!active[i]);
 }
 return 0;
}
''', source_flags=['-DLCOM_COMPAT_LEGACY=1'])


    def test_copter_setup_consumes_command_acks_before_mouse_packets(self):
        self.compile_and_run(COMMUNITY/'NunoRamos--LCOM-Copter/proj/code', ['mouse.c'], r'''#include <stdint.h>
#include <assert.h>
static unsigned count, read_at;static uint8_t queue[8];
int sys_outb(int port,uint32_t value){if(port==0x60)queue[count++]=0xfa;return 0;}
int lcom_compat_sys_inb_ulong(int port,unsigned long *value){*value=port==0x64?(read_at<count?0x21:0):queue[read_at++];return 0;}
uint32_t micros_to_ticks(uint32_t us){return 0;}
int tickdelay(long ticks){return 0;}
void configure_environment(void);
int main(void){configure_environment();assert(count==2 && read_at==count);return 0;}
''', source_flags=['-DLCOM_COMPAT_LEGACY=1'])


    def test_copter_missing_high_score_starts_at_zero(self):
        self.compile_and_run(COMMUNITY/'NunoRamos--LCOM-Copter/proj/code', ['copter.c'], r'''#include <stdio.h>
#include <errno.h>
#include <assert.h>
int bestScore = 123;
FILE *lcom_compat_fopen(const char *path, const char *mode) { errno = ENOENT; return NULL; }
void loadHighScore(void);
int main(void) { loadHighScore(); assert(bestScore == 0); return 0; }
''', source_flags=['-DLCOM_COMPAT_LEGACY=1'])


    def test_color_blox_flood_stays_inside_the_board(self):
        self.compile_and_run(COMMUNITY/'gamaamaral98--FEUP-LCOM/proj', ['game.c'], r'''#include "game.h"
#include <assert.h>
int main(void) {
  Game game = {0};
  for (int x=0;x<COL;x++) for (int y=0;y<ROW;y++) game.color_map[x][y].color=2;
  for (int x=0;x<2;x++) for (int y=0;y<2;y++) game.color_map[x][y].color=1;
  game.color_map[0][0].player=true;
  calculate_player_block(&game);
  for (int x=0;x<COL;x++) for (int y=0;y<ROW;y++)
    assert(game.color_map[x][y].player == (x<2 && y<2));
  return 0;
}
''', source_flags=['-O1','-fsanitize=bounds','-fsanitize-undefined-trap-on-error'])

    def test_queens_date_draw_uses_the_declared_graphics_api(self):
        self.compile_and_run(COMMUNITY/'anaezes--Minix8queens/src', ['rtc.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include "rtc.h"
#include "video_gr.h"
#include <assert.h>
static int seen[18],count;
pixmap_t get_pixmap_digit(int n){assert(n>=0 && n<=10);seen[count++]=n;return(pixmap_t){"x",1,1};}
pixmap_t get_pixmap_small_digit(int n){assert(n>=0 && n<=10);seen[count++]=n;return(pixmap_t){"x",1,1};}
void vg_draw_pixmap(unsigned short x,unsigned short y,char*p,int w,int h){assert(w==1 && h==1 && *p=='x');}
int main(void){date_t date={.hour=23,.min=58,.sec=59,.day=16,.month=10,.year=26};show_date(&date);int expected[]={2,3,10,5,8,10,5,9,1,6,10,1,0,10,2,0,2,6};assert(count==18);for(int i=0;i<18;i++)assert(seen[i]==expected[i]);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-include','lcom/compat.h','-Werror=implicit-function-declaration'])

    def test_pokemon_score_renders_bounded_decimal_digits(self):
        self.compile_and_run(COMMUNITY/'pedroduartecosta--LCOM-FEUP/proj/source', ['score.c'],r'''#include "score.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
static char rendered[16];static int count;
void drawBitmap(Bitmap *bitmap,int x,int y,Alignment alignment){if((uintptr_t)bitmap==100)return;assert(count<6);rendered[count++]=(char)('0'+(uintptr_t)bitmap-1);rendered[count]=0;}
int main(void){Score s={0};Bitmap **digits[]={&s.score0,&s.score1,&s.score2,&s.score3,&s.score4,&s.score5,&s.score6,&s.score7,&s.score8,&s.score9};for(int i=0;i<10;i++)*digits[i]=(Bitmap*)(uintptr_t)(i+1);s.scoreBoard=(Bitmap*)100;unsigned values[]={0,9,123456,UINT_MAX};const char*expected[]={"0","9","123456","429496"};for(int i=0;i<4;i++){s.score=values[i];count=0;rendered[0]=0;drawScore(&s);assert(strcmp(rendered,expected[i])==0);}return 0;}
''',source_flags=['-include','stdlib.h','-O1','-fsanitize=bounds','-fsanitize-undefined-trap-on-error'])

    def test_pokemon_failed_vbe_query_returns_null(self):
        self.compile_and_run(COMMUNITY/'pedroduartecosta--LCOM-FEUP/proj/source', ['interface.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "vbe.h"
#include <assert.h>
int lcom_compat_legacy_int86(struct reg86u *regs){return 0;}
int vbe_get_mode_info(unsigned short mode,vbe_mode_info_t*info){return 1;}
int sys_privctl(endpoint_t endpoint,int request,void*arg){abort();}
void*vm_map_phys(endpoint_t endpoint,void*address,size_t size){abort();}
void panic(const char*format,...){abort();}
void *initGame(unsigned short mode);
int main(void){assert(initGame(0x114)==NULL);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-include','lcom/compat.h','-Werror=return-type'])

    def test_pokemon_mouse_enable_starts_before_first_ack(self):
        self.compile_and_run(COMMUNITY/'pedroduartecosta--LCOM-FEUP/proj/source', ['mouse.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "mouse.h"
#include <assert.h>
static int writes,reads;
int sys_outb(int port,uint32_t value){const int ports[]={0x64,0x64,0x60},values[]={0xa8,0xd4,0xf4};assert(writes<3 && port==ports[writes] && value==values[writes]);writes++;return 0;}
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){if(port==0x64)*value=0;else{assert(port==0x60 && writes==3);*value=0xfa;reads++;}return 0;}
int tickdelay(clock_t t){return 0;}uint32_t micros_to_ticks(uint32_t m){return 0;}
Bitmap*loadBitmap(const char*path){return NULL;}
int main(void){Mouse*m=getMouse();assert(m && m->x==400 && m->y==300);assert(writes==3 && reads==1);assert(getMouse()==m);assert(writes==3 && reads==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-include','lcom/compat.h','-O1','-Werror=uninitialized'])

    def test_pokemon_menu_uses_the_complete_mouse_snapshot(self):
        self.compile_and_run(COMMUNITY/'pedroduartecosta--LCOM-FEUP/proj/source',
                             ['MainMenuState.c'], r'''#include "MainMenuState.h"
#include <assert.h>
static Mouse pointer = {.x=400,.y=330,.lb_pressed=1};
Mouse *getMouse(void) { return &pointer; }
/* Consuming another event here would replace the completed click snapshot. */
void updateMouse(void) { pointer.lb_pressed=0; pointer.x=0; }
int mouseInsideBox(Box *box) { return pointer.x>box->x1 && pointer.x<box->x2 && pointer.y>box->y1 && pointer.y<box->y2; }
int mouse_unsubscribe(void) { return 0; }
int main(void) {
  Box play={330,300,470,390}, quit={330,410,470,500};
  MainMenuState state={.playButton=&play,.exitButton=&quit};
  assert(updateMainMenuState(&state,0)==0);
  assert(state.play==1 && state.overPlay==1 && state.overExit==0);
  return 0;
}
''', source_flags=['-DLCOM_COMPAT_LEGACY=1'])


    def test_doodle_port_reads_write_only_the_output_byte(self):
        self.compile_and_run(COMMUNITY/'Filipecordeiro--LCOM/projeto', ['kbc.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "kbc.h"
#include <assert.h>
#include <string.h>
static int fail;
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){if(fail)return 5;unsigned long byte=port==0x64?1:0x20;memcpy(value,&byte,sizeof byte);return 0;}
struct guarded_byte{unsigned char before,byte,after[16];};
static void guard(struct guarded_byte*b){assert(b->before==0xa5);for(int i=0;i<16;i++)assert(b->after[i]==0xa5);}
int tickdelay(clock_t t){return 0;}uint32_t micros_to_ticks(uint32_t m){return 0;}
int main(void){struct guarded_byte b;memset(&b,0xa5,sizeof b);assert(kbc_read_status_buf(&b.byte)==0 && b.byte==1);guard(&b);assert(kbc_read_out_buf(&b.byte)==0 && b.byte==0x20);guard(&b);assert(kbc_read_status_buf(NULL)==EINVAL);assert(kbc_read_out_buf(NULL)==1);fail=1;b.byte=0xa5;assert(kbc_read_status_buf(&b.byte)==5 && b.byte==0xa5);guard(&b);assert(kbc_read_out_buf(&b.byte)==1 && b.byte==0xa5);guard(&b);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O1','-fstack-protector-all'])

    def test_maze_renderer_clips_at_exclusive_framebuffer_bounds(self):
        self.compile_and_run(COMMUNITY/'vitormsesteves--LCOM/proj/code', [],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "video_gr.c"
#include <assert.h>
#include <string.h>
static void unchanged(const unsigned char *buffer){for(int i=0;i<24;i++)assert(buffer[i]==0);}
int main(void){h_res=4;v_res=3;bytes_per_pixel=2;unsigned char guarded[40];memset(guarded,0xa5,sizeof guarded);unsigned char*frame=guarded+8;memset(frame,0,24);draw_pixel(4,0,0x1234,(char*)frame);unchanged(frame);draw_pixel(0,3,0x1234,(char*)frame);unchanged(frame);assert(draw_rectangle(3,2,5,4,0x1234,(char*)frame)==0);for(int i=0;i<22;i++)assert(frame[i]==0);assert(frame[22]==0x34 && frame[23]==0x12);for(int i=0;i<8;i++)assert(guarded[i]==0xa5 && guarded[32+i]==0xa5);assert(draw_rectangle(8,7,6,4,0xffff,(char*)frame)==0);for(int i=0;i<8;i++)assert(guarded[i]==0xa5 && guarded[32+i]==0xa5);return 0;}
''',source_flags=['-O1'])

    def test_maze_mouse_decodes_full_signed_nine_bit_packets(self):
        self.compile_and_run(COMMUNITY/'vitormsesteves--LCOM/proj/code', ['mouse.c'],r'''#include "mouse.h"
#include <assert.h>
unsigned getH_res(void){return 800;}unsigned getV_res(void){return 600;}
int rgb(unsigned char r,unsigned char g,unsigned char b){return 255;}
Rectangle* newRectangle(int x,int y,int w,int h){static Rectangle r;return &r;}
void setRecLocation(Rectangle*r,int x,int y){r->x=x;r->y=y;}
int main(void){const int deltas[]={-255,-200,-129,-128,-1,0,1,127,128,200,255};Mouse*m=getMouse();for(unsigned i=0;i<sizeof(deltas)/sizeof(deltas[0]);i++){int dx=deltas[i],dy=deltas[(i+3)%11];m->x=400;m->y=300;m->packet[0]=8|(dx<0?16:0)|(dy<0?32:0)|1;m->packet[1]=(unsigned char)dx;m->packet[2]=(unsigned char)dy;updateMouse();assert(m->deltaX==dx);assert(m->deltaY==dy);assert(m->x==400+dx);assert(m->y==300-dy);assert(m->rect->x==m->x && m->rect->y==m->y);assert(m->leftButtonDown);assert(!m->middleButtonDown && !m->rightButtonDown);}return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-fsigned-char','-Wno-implicit-function-declaration','-O1'])

    def test_hidden_fleet_xpm_rows_fit_their_allocated_buffers(self):
        project=COMMUNITY/'vascofg--lcom/trunk'
        self.compile_and_run(project, ['xpm.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "core.h"
#include <assert.h>
#include <string.h>
#undef fopen
int aloc_xpm(void);
static void*allocations[5000];static size_t sizes[5000];static int count;
void*tracked_malloc(size_t n){void*p=malloc(n);assert(count<5000);allocations[count]=p;sizes[count++]=n;return p;}
char*bounded_fgets(char*p,int n,FILE*f){size_t capacity=0;for(int i=0;i<count;i++)if(allocations[i]==p)capacity=sizes[i];assert(capacity>0 && n>0 && (size_t)n<=capacity);return fgets(p,n,f);}
FILE*lcom_compat_fopen(const char*path,const char*mode){const char*name=strrchr(path,'/');char local[2048];snprintf(local,sizeof local,"%s/%s",ASSET_DIR,name+1);FILE*f=fopen(local,mode);assert(f);return f;}
int main(void){assert(aloc_xpm()==0);char**maps[]={battleship1_xpm,battleship2_xpm,map_xpm,won_xpm,zero_xpm,one_xpm,two_xpm,three_xpm,four_xpm,five_xpm,six_xpm,seven_xpm,eight_xpm,nine_xpm,sidebar_xpm,x_xpm,y_xpm,water_xpm,hit_xpm,sunk_xpm};const int widths[]={800,800,800,800,27,27,27,27,27,27,27,27,27,27,180,30,30,150,150,150};const int rows[]={605,605,603,603,31,31,31,31,31,31,31,31,31,31,403,33,33,89,88,89};for(int i=0;i<20;i++){int w,h,c;assert(sscanf(maps[i][0],"%d%d%d",&w,&h,&c)==3);assert(w==widths[i]);assert(h+c+1==rows[i]);for(int y=c+1;y<rows[i];y++)assert(strlen(maps[i][y])==(size_t)w);}return 0;}
'''.replace('ASSET_DIR', '"'+str(project/'res')+'"'),source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmalloc=tracked_malloc','-Dfgets=bounded_fgets', '-DASSET_DIR="'+str(project/'res')+'"','-Wno-implicit-function-declaration','-O1'])

    def test_penaltix_menu_uses_the_typed_drawing_api_in_both_header_orders(self):
        for headers in ['#include "Bitmap.h"\n#include "video_gr.h"', '#include "video_gr.h"\n#include "Bitmap.h"']:
            self.compile_and_run(COMMUNITY/'bernardomcbarbosa--feup-lcom/proj/src', ['Menu.c'], '#define LCOM_COMPAT_LEGACY 1\n'+headers+r'''
_Static_assert(_Generic(&draw_bitmap,int(*)(Bitmap*,unsigned short,unsigned short,int,short*):1,default:0),"bitmap ABI");
_Static_assert(_Generic(&draw_ball,int(*)(Bitmap*,unsigned short,unsigned short,short*,int):1,default:0),"ball ABI");
int main(void){return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Werror=implicit-function-declaration'])

    def test_penaltix_absent_ball_bitmap_is_a_no_collision_no_op(self):
        self.compile_and_run(COMMUNITY/'bernardomcbarbosa--feup-lcom/proj/src', ['Bitmap.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include "Bitmap.h"
#include <assert.h>
unsigned int rgb565(unsigned char r,unsigned char g,unsigned char b){return (r>>3)|((g>>2)<<5)|((b>>3)<<11);}
int main(void){short frame[1]={0x1234};assert(drawBitmap_ball(NULL,0,0,frame,2)==0);assert(frame[0]==0x1234);short pixel=0x4321;Bitmap bmp={0};bmp.bitmapInfoHeader.width=1;bmp.bitmapInfoHeader.height=1;bmp.bitmapData=&pixel;frame[0]=0;assert(drawBitmap_ball(&bmp,0,0,frame,2)==1 && frame[0]==pixel);frame[0]=0x1234;assert(drawBitmap_ball(&bmp,0,0,frame,0)==0 && frame[0]==pixel);pixel=rgb565(0,255,0);frame[0]=0;assert(drawBitmap_ball(&bmp,0,0,frame,2)==0 && frame[0]==0);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Wno-return-type','-Wno-implicit-function-declaration','-O0'])

    def test_battalion_logging_survives_an_unavailable_log_file(self):
        self.compile_and_run(COMMUNITY/'renatoabreu11--LCOM-BattalionTanks/Tanks/src', ['main.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include <assert.h>
#include <string.h>
extern FILE*logfd;void initLog(void);static int unavailable=1;
FILE*lcom_compat_fopen(const char*path,const char*mode){assert(strcmp(path,"home/lcom/Tanks/log.txt")==0);assert(strcmp(mode,"w")==0);return unavailable?NULL:tmpfile();}
int main(void){initLog();assert(logfd==stderr);unavailable=0;initLog();assert(logfd && logfd!=stderr);rewind(logfd);char line[200];assert(fgets(line,sizeof line,logfd));assert(strstr(line,"logging successfully initialized"));fclose(logfd);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmain=battalion_entry','-Wno-implicit-function-declaration','-O1'])

    def test_battalion_constructors_return_the_initialized_objects(self):
        self.compile_and_run(COMMUNITY/'renatoabreu11--LCOM-BattalionTanks/Tanks/src', ['font.c','bullet.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include "font.h"
#include "bullet.h"
#include <assert.h>
static void*last;
void*track_malloc(size_t n){last=malloc(n);return last;}
int main(void){Font*f=InitFont();assert(f==last && f!=NULL);assert(f->character==NULL);free(f);Bullet*b=CreateBullet(0);assert(b==last && b!=NULL);assert(b->inUse==0 && b->explosion_counter==0);free(b);b=CreateBullet(1);assert(b==last && b->inUse==1 && b->explosion_counter==0);free(b);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmalloc=track_malloc','-Wno-implicit-function-declaration','-O1'])

    def test_battalion_failed_bios_setup_returns_null(self):
        self.compile_and_run(COMMUNITY/'renatoabreu11--LCOM-BattalionTanks/Tanks/src', ['video_gr.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "video_gr.h"
#include "vbe.h"
#include <assert.h>
static int stage;
int lcom_compat_legacy_int86(struct reg86u*r){return stage==0?1:0;}
int vbe_get_mode_info(unsigned short mode,vbe_mode_info_t*info){assert(stage==1);return 1;}
int sys_privctl(endpoint_t e,int request,void*arg){abort();}
void*vm_map_phys(endpoint_t e,void*address,size_t size){abort();}
void panic(const char*format,...){abort();}
int main(void){assert(vg_init(0x117)==NULL);stage=1;assert(vg_init(0x117)==NULL);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Werror=return-type','-Wno-implicit-function-declaration'])


    def test_battle_tetris_bitmap_callers_use_the_pointer_returning_header(self):
        self.compile_and_run(COMMUNITY/'telmobarros--feup-lcom/proj/code', ['piece.c','video_gr.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "piece.h"
#include "bitmap.h"
#include <assert.h>
static uint16_t pixels[900];
uint16_t*map_Bitmap(const char*path,int*width,int*height){*width=30;*height=30;return pixels;}
int main(void){Piece*p=new_piece(200,100);assert(p && p->sprite.map==pixels);assert(p->sprite.width==30 && p->sprite.height==30);free(p);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Werror=implicit-function-declaration'])


    def test_battle_tetris_discards_invalid_sync_with_a_defined_result(self):
        self.compile_and_run(COMMUNITY/'telmobarros--feup-lcom/proj/code', ['handler.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "handler.h"
#include "mouse.h"
#include <assert.h>
extern Mouse_t mouse;static int next_byte;
int mouse_output(void){return next_byte;}
unsigned getH_res(void){return 1024;}unsigned getV_res(void){return 768;}
int main(void){mouse=(Mouse_t){.x=100,.y=100,.width=10,.height=10};next_byte=0;assert(mouse_packet_handler()==0);assert(mouse.x==100 && mouse.y==100 && mouse.left_btn==0);next_byte=0x29;assert(mouse_packet_handler()==0);next_byte=5;assert(mouse_packet_handler()==0);assert(mouse.x==100 && mouse.y==100);next_byte=254;assert(mouse_packet_handler()==0);assert(mouse.x==105 && mouse.y==102 && mouse.left_btn==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Wno-implicit-function-declaration'])


    def test_battle_tetris_missing_bitmap_returns_null_without_changing_dimensions(self):
        self.compile_and_run(COMMUNITY/'telmobarros--feup-lcom/proj/code', ['bitmap.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "bitmap.h"
#include <assert.h>
FILE*lcom_compat_fopen(const char*path,const char*mode){return NULL;}
int main(void){int width=123,height=456;assert(map_Bitmap("missing.bmp",&width,&height)==NULL);assert(width==123 && height==456);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-include',str(existing.ROOT/'sdk/compat/include/lcom/compat.h')])


    def test_pew_scores_keep_an_explicit_integer_animation_counter(self):
        self.compile_and_run(COMMUNITY/'LuisEduardoReis--LCOM-PewPewsAndExplosions', ['scores.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/compat.h>
#include "scores.h"
#include <assert.h>
static unsigned next_hue;
unsigned int hsvTorgb(unsigned int h,float s,float v){assert(h==next_hue);assert(s==1 && v==1);next_hue+=5;return 0x123456;}
int drawTextBlend(char*text,int x,int y,int blend){assert(blend==0x123456);return 0;}
int drawText(char*text,int x,int y){return 0;}
int main(void){Highscore h={.name="ACE",.score=50};AddScore(h);assert(getHighscore().score==50);DrawScores(100,100);DrawScores(100,100);assert(next_hue==10);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Werror=implicit-int','-include',str(existing.ROOT/'sdk/compat/include/lcom/compat.h')])


    def test_classic_connect_four_consumes_each_notified_keyboard_byte_once(self):
        self.compile_and_run(COMMUNITY/'hpnog--FEUP-LCOM/code', ['keyboard.c'],r'''#define LCOM_COMPAT_LEGACY 1
#define LCOM_COMPAT_LEGACY_LM_STATUS 1
#include <lcom/compat.h>
#include "keyboard.h"
#include <assert.h>
static int reads,pending=0xb9;
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){if(port==0x64){*value=1;return 0;}assert(port==0x60);reads++;*value=reads==1?pending:0;return 0;}
unsigned short mouse_int_handler(void){abort();}void printMoves(void){abort();}void timer_int_handler(void){}void draw_progressBar_15sec(int s){}
int tickdelay(clock_t ticks){return 0;}uint32_t micros_to_ticks(uint32_t usecs){return usecs;}
int main(void){message m={.m_source=HARDWARE};m.NOTIFY_ARG=BIT(9);assert(interrupt_handler(BIT(9),m,IPC_STATUS_NOTIFY)==0xb9);assert(reads==1);reads=0;pending=0x9c;assert(interrupt_handler_mouse_kbd(BIT(9),m,IPC_STATUS_NOTIFY,BIT(2),BIT(0))==0x9c);assert(reads==1);reads=0;m.NOTIFY_ARG=BIT(7);assert(interrupt_handler(BIT(9),m,IPC_STATUS_NOTIFY)==0);assert(reads==0);assert(interrupt_handler_mouse_kbd(BIT(9),m,IPC_STATUS_NOTIFY,BIT(2),BIT(0))==0);assert(reads==0);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-Wno-implicit-function-declaration'])

    def test_classic_connect_four_menu_uses_its_character_returning_keyboard_header(self):
        self.compile_and_run(COMMUNITY/'hpnog--FEUP-LCOM/code', [],r'''#define LCOM_COMPAT_LEGACY 1
#define LCOM_COMPAT_LEGACY_LM_STATUS 1
#include <lcom/compat.h>
#include <assert.h>
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
/* Two unrelated renderer definitions have no header declarations upstream. */
void draw_mainTitle(void); void draw_menu_symbols(void);
#include "interface.c"
/* This owning header must agree with the selector as seen by the actual caller.
   An implicit int call disagrees with its authored char result and traps in Wasm. */
#include "keyboard.h"
static unsigned starts,dates;
#define NOOP(name) void name(void){}
NOOP(draw_welcome) NOOP(draw_mainTitle) NOOP(draw_spaceToContinue)
NOOP(draw_options) NOOP(draw_creditsScreen) NOOP(draw_secondTitle)
NOOP(draw_menu_symbols) NOOP(readAllBitmaps_pokemon) NOOP(readAllBitmaps_universe)
void draw_date(void){dates++;}
void draw_arrow(int x,int y){}void draw_arrowCleaner(int x,int y){}
void draw_options_arrow(int x,int y){}
char waitForArrowsMenu(void){return 'E';}char waitForArrowsOptions(void){return 'E';}
int waitForSpaceBar(void){return 0;}
void startGame(unsigned char**buffer){starts++;}
int main(void){unsigned char*buffer=0;assert(main_menu(&buffer)==1);assert(starts==1 && dates==1);return 0;}
''')

    def test_classic_connect_four_failed_subscription_returns_failure_status(self):
        self.compile_and_run(COMMUNITY/'hpnog--FEUP-LCOM/code',['proj.c'],r'''#define LCOM_COMPAT_LEGACY 1
#define LCOM_COMPAT_LEGACY_LM_STATUS 1
#include <lcom/compat.h>
#include "interface.h"
#include <assert.h>
int connect_four_entry(void);
static int stage,calls;
void sef_startup(void){} int sys_enable_iop(endpoint_t endpoint){return 0;}
int timer_subscribe_int(void){calls++;return stage==0?-1:1;}
int kbd_subscribe_int(void){calls++;return stage==1?-1:2;}
int mouse_subscribe_int(void){calls++;assert(stage==2);return -1;}
int subscribe_real_time_clock(void){abort();}void readAllBitmaps_universe(void){abort();}
int waitForSpaceBar(void){abort();}int lm_init(void){abort();}
void video_initialize(void){abort();}void welcome_screen(void){abort();}
int main_menu(unsigned char**buffer){abort();}int vg_exit(void){abort();}
int timer_unsubscribe_int(void){abort();}int kbd_unsubscribe_int(void){abort();}int mouse_unsubscribe_int(void){abort();}
int unsubscribe_real_time_clock(void){abort();}
int main(void){for(stage=0;stage<3;stage++){calls=0;assert(connect_four_entry()==1);assert(calls==stage+1);}return 0;}
''',source_flags=['-Dmain=connect_four_entry','-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-Werror=return-type','-Wno-implicit-function-declaration'])

    def test_classic_connect_four_failed_bios_setup_returns_null(self):
        self.compile_and_run(COMMUNITY/'hpnog--FEUP-LCOM/code',['video_gr.c'],r'''#define LCOM_COMPAT_LEGACY 1
#define LCOM_COMPAT_LEGACY_LM_STATUS 1
#include <lcom/compat.h>
#include "video_gr.h"
#include "vbe.h"
#include <assert.h>
int lcom_compat_legacy_int86(struct reg86u*r){return 1;}
int vbe_get_mode_info(unsigned short mode,vbe_mode_info_t*info){abort();}
int sys_privctl(endpoint_t ep,int request,void*arg){abort();}
void*vm_map_phys(endpoint_t ep,void*address,size_t length){abort();}
void panic(const char*format,...){abort();}
int main(void){assert(vg_init(0x117)==NULL);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-Werror=return-type','-Wno-implicit-function-declaration'])

    def test_terminix_loader_keeps_five_scores_and_stops_at_real_eof(self):
        self.compile_and_run(COMMUNITY/'BernardoRamalho--FEUP_LCOM/proj/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
static unsigned allocations;static const char*records;
static FILE*score_open(const char*path,const char*mode){assert(strcmp(path,"/home/lcom/labs/proj/src/scores.txt")==0&&strcmp(mode,"r")==0);FILE*f=tmpfile();assert(f);fputs(records,f);rewind(f);return f;}
static void*score_alloc(size_t size){assert(allocations<5&&"the five-slot leaderboard cannot create a sixth record");allocations++;return malloc(size);}
#define fopen score_open
#define malloc score_alloc
#include "highscores.c"
#undef malloc
#undef fopen
static void run(const char*text,unsigned expected){allocations=0;records=text;for(unsigned i=0;i<5;i++)highscores[i]=NULL;read_highscores();assert(allocations==expected&&"EOF must not create phantom score records");for(unsigned i=0;i<5;i++){if(i<expected){assert(highscores[i]&&highscores[i]->score==100-(int)i&&highscores[i]->year==2020);free(highscores[i]);}else assert(highscores[i]==NULL);}}
int main(void){run("100 1 2 2020 3 4 5\n99 1 2 2020 3 4 5\n98 1 2 2020 3 4 5\n97 1 2 2020 3 4 5\n96 1 2 2020 3 4 5\n95 1 2 2020 3 4 5\n",5);run("100 1 2 2020 3 4 5\n",1);run("",0);return 0;}
''')

    def test_spacecom_renderer_keeps_exact_frame_edges_outside_pixel_writes(self):
        self.compile_and_run(COMMUNITY/'DiogoRamos9--Y2S2-LCOM/proj/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
#include "utils/video/video.c"
#include "sprites/sprite.c"
extern vbe_mode_info_t mode_info;extern uint8_t*double_buffer;
int main(void){const size_t bytes=20*12*3,page=(size_t)sysconf(_SC_PAGESIZE);uint8_t*guard=mmap(NULL,page*2,PROT_READ|PROT_WRITE,MAP_ANON|MAP_PRIVATE,-1,0);assert(guard!=MAP_FAILED&&mprotect(guard+page,page,PROT_NONE)==0);double_buffer=guard+page-bytes;memset(double_buffer,0,bytes);mode_info=(vbe_mode_info_t){.XResolution=20,.YResolution=12,.BitsPerPixel=24};assert(vg_draw_pixel(19,11,0x123456)==0);assert(double_buffer[bytes-3]==0x56&&double_buffer[bytes-2]==0x34&&double_buffer[bytes-1]==0x12);assert(vg_draw_pixel(20,0,0xffffff)==1&&"x equal to width is outside the framebuffer");assert(vg_draw_pixel(0,12,0xffffff)==1);assert(vg_draw_pixel(UINT16_MAX,0,0xffffff)==1);assert(vg_draw_pixel(0,UINT16_MAX,0xffffff)==1);uint32_t art[]={0xabcdef,0x112233,0x445566,0x778899};Sprite sprite={.width=2,.height=2,.map=art};assert(draw_sprite(&sprite,19,11)==1);assert(double_buffer[bytes-3]==0xef&&double_buffer[bytes-2]==0xcd&&double_buffer[bytes-1]==0xab);for(size_t i=0;i<bytes-3;i++)assert(double_buffer[i]==0);munmap(guard,page*2);return 0;}
''')

    def test_terminix_complete_packet_hits_latest_cursor_without_waiting_for_repaint(self):
        self.compile_and_run(COMMUNITY/'BernardoRamalho--FEUP_LCOM/proj/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "mouse.c"
static Mouse pointer;static Terminix game;
Terminix*get_current_terminix(void){return &game;}
int main(void){game.mouse=&pointer;game.mode_info=(vbe_mode_info_t){.XResolution=1152,.YResolution=864};pointer.mouse_img=(xpm_image_t){.width=22,.height=22};pointer.x=pointer.new_x=300;pointer.y=pointer.new_y=300;update_mouse_position((struct packet){.delta_y=-300});assert(pointer.new_y==600&&pointer.y==300);assert(mouse_over_1player_option()&&"completed input targets Single Player before any render tick");pointer.x=300;pointer.y=600;update_mouse_position((struct packet){.delta_y=100});assert(!mouse_over_1player_option()&&"moving outside rejects the stale displayed hotspot");update_mouse_position((struct packet){.delta_x=500,.delta_y=-100});assert(mouse_over_coop_option());Sprite sprite={.x=700,.y=500,.img={.width=200,.height=200}};assert(mouse_over_img(&sprite));update_mouse_position((struct packet){.delta_x=-500,.delta_y=300});assert(!mouse_over_coop_option()&&!mouse_over_img(&sprite));assert(pointer.x==300&&pointer.y==600&&"previous displayed coordinates remain available for cursor cleanup");return 0;}
''')

    def test_terminix_zombies_choose_only_present_targets_when_distances_tie(self):
        self.compile_and_run(COMMUNITY/'BernardoRamalho--FEUP_LCOM/proj/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include "Game.c"
static Terminix game;static unsigned painted;
Terminix*get_current_terminix(void){return &game;}
void clean_xpm(xpm_image_t img,uint16_t x,uint16_t y){}
void draw_xpm(xpm_image_t img,uint16_t x,uint16_t y){painted++;}
void save_score(int hero){assert(!"distant targets cannot end the round");}
static void step(Zombie*z,uint16_t x,uint16_t y){painted=0;update_zombie_position(z,0);assert(painted==1&&z->sprite->x==x&&z->sprite->y==y);}
int main(void){
 Chest chest={.x=500,.y=300};Hero first={.x=500,.y=100},second={.x=100,.y=300};Sprite sprite={.x=100,.y=200,.speed=2,.img={.width=1,.height=1}};Zombie zombie={.sprite=&sprite};game.chest=&chest;game.heroes[0]=&first;game.zombies[0]=&zombie;game.nr_zombies=1;game.nr_player=1;game.mode_info=(vbe_mode_info_t){.XResolution=1152,.YResolution=864};
 /* Equal chest/solo-hero distance must never target absent player two. */
 step(&zombie,102,202);
 /* Both living players are eligible, and the nearer player wins. */
 game.heroes[1]=&second;sprite.x=100;sprite.y=200;step(&zombie,100,202);
 /* A removed co-op player remains ineligible, including tied distances. */
 game.heroes[0]=NULL;sprite.x=200;sprite.y=300;chest.x=300;chest.y=300;step(&zombie,202,300);
 /* Keep ordinary strict-nearest behavior for player one and the chest. */
 game.heroes[0]=&first;game.heroes[1]=NULL;first.x=100;first.y=100;sprite.x=100;sprite.y=200;step(&zombie,100,198);
 game.heroes[0]=NULL;sprite.x=100;sprite.y=200;chest.x=100;chest.y=300;step(&zombie,100,202);
 return 0;}
''')

    def test_eye_command_keeps_its_first_ps2_ack(self):
     self.compile_and_run(COMMUNITY/'carlosfmccosta--LCOM/proj/src',[],r'''#include <lcom/lcf.h>
   #include <assert.h>
   #include "controller/KBC/mouse/mouse.c"
   static int pending,reads,writes;
   int write_byte(uint8_t byte,uint8_t port){if(port==0x64)assert(byte==0xd4);else {assert(port==0x60&&byte==0xf4);pending=1;writes++;}return 0;}
   int util_sys_inb(int port,uint8_t*value){if(port==0x64)*value=pending?0x21:0;else {assert(port==0x60);*value=pending?0xfa:0;pending=0;reads++;}return 0;}
   int tickdelay(clock_t ticks){return 0;}uint32_t micros_to_ticks(uint32_t micros){return 1;}
   int main(void){assert(mouse_write_cmd(0xf4)==0&&"one immediate PS/2 ACK completes the command");assert(reads==1&&writes==1&&pending==0);return 0;}
   ''')
    def test_green_circuit_round_reset_clears_objects_without_freeing_stack(self):
     self.compile_and_run(COMMUNITY/'diogobabo--LCOM-FEUP/proj/src',[],r'''#include "snake.h"
   #include <assert.h>
   static void reject_free(void*pointer){assert(!"round reset owns no heap allocation to free");}
   #define free reject_free
   #include "snake.c"
   #undef free
   enum STATE GameState;int counter;
   int main(void){for(unsigned i=0;i<5000;i++){memset(&array[i],0xff,sizeof array[i]);memset(&walls[i],0xff,sizeof walls[i]);}snake.bodySize=17;numObjects=90;MenuStarter();for(unsigned i=0;i<sizeof array;i++)assert(((unsigned char*)array)[i]==0);for(unsigned i=0;i<sizeof walls;i++)assert(((unsigned char*)walls)[i]==0);assert(snake.bodySize==0&&numObjects==0&&snake.x==288&&snake.y==96);return 0;}
   ''')
    def test_sketch_requests_two_frame_pages_in_bytes(self):
     self.compile_and_run(COMMUNITY/'diogotvf7--feup-lcom-project/proj/src',[],r'''#include <lcom/lcf.h>
   #include <assert.h>
   #include "controller/graphics_card/graphics_card.c"
   #define FRAME_BYTES (1152u*864u*4u)
   static unsigned char pixel;
   int vbe_get_mode_info(uint16_t mode,vbe_mode_info_t*info){assert(mode==0x14c);*info=(vbe_mode_info_t){.XResolution=1152,.YResolution=864,.BitsPerPixel=32,.BytesPerScanLine=4608,.PhysBasePtr=0xe0000000};return 0;}
   int sys_privctl(endpoint_t ep,int request,void*arg){struct minix_mem_range*r=arg;assert(r->mr_limit-r->mr_base==FRAME_BYTES*2&&"two byte-sized pages fit original VRAM");return 0;}
   void*vm_map_phys(endpoint_t ep,void*address,size_t length){assert(length==FRAME_BYTES*2);return &pixel;}
   void panic(const char*format,...){abort();}
   int main(void){assert(map_graphics_memory(0x14c)==&pixel);assert(bytes_per_pixel==4&&h_res==1152&&v_res==864);return 0;}
   ''')
    def test_eye_menu_owns_title_options_and_callbacks(self):
     self.compile_and_run(COMMUNITY/'carlosfmccosta--LCOM/proj/src',[],r'''#include <lcom/lcf.h>
   #include <assert.h>
   #include "view/menu.c"
   struct sprite {position_t position;};struct text {sprite_t*sp;};
   sprite_t*create_sprite(xpm_map_t pics[],unsigned int size,uint16_t x,uint16_t y){return calloc(1,sizeof(sprite_t));}
   void destroy_sprite(sprite_t*sp){free(sp);}position_t*get_sprite_pos(sprite_t*sp){return &sp->position;}
   uint16_t get_sprite_height(sprite_t*sp){return 20;}uint16_t get_sprite_width(sprite_t*sp){return 10;}
   text_t*create_text(uint16_t x,uint16_t y){text_t*t=calloc(1,sizeof*t);t->sp=create_sprite(NULL,0,x,y);return t;}
   sprite_t*get_text_sprite(text_t*t){return t->sp;}void destroy_text(text_t*t){destroy_sprite(t->sp);free(t);}
   static int called;static void chosen(void){called++;}static void wrong(void){abort();}
   int main(void){char title[]="Original",option[]="Play";const char*options[]={option};void(*callbacks[])(void)={chosen};menu_t*m=create_menu(title,options,callbacks,1,1024,768);assert(m);title[0]='X';option[0]='X';options[0]="Overwritten";callbacks[0]=wrong;assert(strcmp(m->title,"Original")==0&&strcmp(m->options[0],"Play")==0&&"menu retains its own strings after caller changes");get_menu_callback(m,0)();assert(called==1);destroy_menu(m);return 0;}
   ''')

    def test_green_circuit_score_update_preserves_top_five_and_file_ownership(self):
     self.compile_and_run(COMMUNITY/'diogobabo--LCOM-FEUP/proj/src',[],r'''#include <lcom/lcf.h>
   #include <assert.h>
   #include <unistd.h>
   static char path[]="/tmp/lcom-snake-scores-XXXXXX";
   static FILE*score_open(const char*name,const char*mode);
   static void*owned[8];static unsigned allocated;
   static void*score_alloc(size_t size){assert(allocated<8);return owned[allocated++]=malloc(size);}
   static void score_free(void*p){if(!p)return;for(unsigned i=0;i<allocated;i++)if(p==owned[i]){owned[i]=NULL;free(p);return;}assert(!"leaderboard only frees owned allocation starts, never stack/interior pointers");}
   #define fopen score_open
   #define malloc score_alloc
   #define free score_free
   #include "leaderboard.c"
   #undef free
   #undef malloc
   #undef fopen
   static FILE*score_open(const char*name,const char*mode){assert(strcmp(name,"/home/lcom/labs/g04/proj/src/leaderboard.txt")==0);return fopen(path,mode);}
   int main(void){int fd=mkstemp(path);assert(fd>=0);FILE*f=fdopen(fd,"w");const char seed[]="1 - 50\n2 - 40\n3 - 30\n4 - 20\n5 - 10\n";assert(fwrite(seed,1,sizeof seed,f)==sizeof seed);fclose(f);leaderboard(25);f=fopen(path,"r");int expected[]={50,40,30,25,20};for(int i=0;i<5;i++){int rank,score;assert(fscanf(f,"%d - %d",&rank,&score)==2&&rank==i+1&&score==expected[i]);}fclose(f);unlink(path);return 0;}
   ''')

    def test_eye_events_initialize_inactive_owned_payload(self):
     self.compile_and_run(COMMUNITY/'carlosfmccosta--LCOM/proj/src',[],r'''#include <lcom/lcf.h>
   #include <assert.h>
   #include "controller/event.h"
   static void*poison_malloc(size_t size){void*p=malloc(size);assert(p);memset(p,0xa5,size);return p;}
   #define malloc poison_malloc
   #include "controller/event.c"
   #undef malloc
   int main(void){struct key key={.size=1,.bytes={0x1c,0}};event_t*k=new_keyboard_event(&key);assert(k&&k->k&&k->pack==NULL&&"keyboard event owns only key payload");assert(k->k->bytes[0]==0x1c);free(k->k);free(k);struct packet packet={.lb=true,.delta_x=-200};event_t*m=new_mouse_event(&packet,2);assert(m&&m->pack&&m->k==NULL&&"mouse event owns only mouse payload");assert(m->pack->lb&&m->pack->delta_x==-200&&m->prev_mouse_btn==2);free(m->pack);free(m);return 0;}
   ''')

    def test_green_circuit_score_renderer_keeps_local_text_owned(self):
     self.compile_and_run(COMMUNITY/'diogobabo--LCOM-FEUP/proj/src',[],r'''#include "snake.h"
   #include <assert.h>
   static void reject_free(void*p){assert(!"score renderer must not free its local text array");}
   #define free reject_free
   #include "snake.c"
   #undef free
   uint8_t *font,*gameBG; xpm_image_t imgFont,imgGameBG;Character letters[40];static unsigned glyphs,clears;
   void cleanBG(unsigned int x,unsigned int y,int width,int height,xpm_image_t img,uint8_t*map){assert(x==576&&y==0);clears++;}
   int draw_letter_map(uint16_t x,uint16_t y,uint8_t*map,xpm_image_t img,unsigned int startX,unsigned int startY){glyphs++;return 0;}
   int main(void){snake.bodySize=124;drawScore();assert(glyphs==3&&clears==1);return 0;}
   ''')

    def test_eye_event_queue_keeps_distinct_payloads_through_wrap_and_growth(self):
     self.compile_and_run(COMMUNITY/'carlosfmccosta--LCOM/proj/src',[],r'''#include <lcom/lcf.h>
   #include <assert.h>
   static unsigned frees;static void record_free(void*p){if(p){frees++;free(p);}}
   #define free record_free
   #include "queue.c"
   #undef free
   struct payload {unsigned id;char text[44];};
   static struct payload value(unsigned id){struct payload p={.id=id};memset(p.text,(int)(id+1),sizeof p.text);return p;}
   static void put(queue_t*q,unsigned id){struct payload p=value(id);assert(put_queue(q,&p)==0);}
   static void get(queue_t*q,unsigned id){struct payload p,expected=value(id);assert(get_queue(q,&p)==0);assert(memcmp(&p,&expected,sizeof p)==0&&"FIFO preserves distinct event payloads");}
   int main(void){queue_t*q=new_queue(3,sizeof(struct payload));assert(q);put(q,0);put(q,1);get(q,0);put(q,2);put(q,3);for(unsigned i=4;i<21;i++)put(q,i);for(unsigned i=1;i<21;i++)get(q,i);assert(is_empty_queue(q));delete_queue(q);assert(frees==2&&"queue releases buffer and queue owner");q=new_queue(0,sizeof(struct payload));put(q,99);get(q,99);delete_queue(q);assert(frees==4);return 0;}
   ''')

    def test_sketch_random_prompt_reads_first_and_last_dictionary_lines(self):
     self.compile_and_run(COMMUNITY/'diogotvf7--feup-lcom-project/proj/src',[],r'''#include <lcom/lcf.h>
   #include <assert.h>
   #include "model/model.h"
   #undef fopen
   static unsigned choice;static FILE*prompt_open(const char*path,const char*mode){assert(strcmp(path,"/home/lcom/labs/g1/proj/src/model/test.txt")==0);return fopen("$DICTIONARY$",mode);}
   static int prompt_random(void){return choice;}
   static void*prompt_alloc(size_t n){return calloc(1,n);}
   #define fopen prompt_open
   #define rand prompt_random
   #define malloc prompt_alloc
   #include "model/model.c"
   #undef malloc
   #undef rand
   #undef fopen
   int main(void){choice=0;word_sol_number_letters=0;char*line=getRandomWord();assert(line&&strcmp(line,"elephant\n")==0&&"random index zero reads the first real dictionary prompt");assert(word_sol_number_letters==8);free(line);choice=49;word_sol_number_letters=0;line=getRandomWord();assert(line&&strcmp(line,"saxophone")==0&&"last random index reaches the final dictionary prompt");assert(word_sol_number_letters==9);free(line);return 0;}
   '''.replace('$DICTIONARY$',str(COMMUNITY/'diogotvf7--feup-lcom-project/proj/src/model/test.txt')))

    def test_sketch_reporting_sends_initial_command_before_testing_ack(self):
     self.compile_and_run(COMMUNITY/'diogotvf7--feup-lcom-project/proj/src',['controller/mouse/mouse.c'],r'''#include <lcom/lcf.h>
   #include <assert.h>
   #include "controller/mouse/mouse.h"
   static unsigned writes,reads;static uint8_t expected;
   int mouse_write(int port,uint8_t value){assert(port==(writes%2?0x60:0x64));assert(value==(writes%2?expected:0xd4));writes++;return 0;}
   int util_sys_inb(int port,uint8_t*value){assert(port==0x60&&writes==2);reads++;*value=0xfa;return 0;}
   int tickdelay(clock_t ticks){return 0;}uint32_t micros_to_ticks(uint32_t us){return 1;}
   int main(void){expected=0xf4;assert(set_data_reporting(true)==0);assert(writes==2&&reads==1&&"startup sends enable before any ACK comparison");writes=reads=0;expected=0xf5;assert(set_data_reporting(false)==0);assert(writes==2&&reads==1);return 0;}
   ''',source_flags=['-O2'])


    def test_green_circuit_menu_restores_only_valid_background_pixels(self):
        self.compile_and_run(COMMUNITY/'diogobabo--LCOM-FEUP/proj/src',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
#include "devices/graphics.c"
#include "menu.c"
enum STATE GameState;mouseInfo mouse;
uint8_t *menuPlay,*menuExit,*menu,*menuLeader,*cursor;
xpm_image_t imgMenuPlay,imgMenuExit,imgMenu,imgMenuLeader,imgCursor;
uint32_t xpm_transparency_color(enum xpm_image_type type){return UINT32_MAX;}
int main(void){
 h_res=1152;v_res=864;bytes_per_pixel=4;size_t size=(size_t)h_res*v_res*4,page=(size_t)sysconf(_SC_PAGESIZE);assert(size%page==0);menu=mmap(NULL,size+page,PROT_READ|PROT_WRITE,MAP_ANON|MAP_PRIVATE,-1,0);assert(menu!=MAP_FAILED&&mprotect(menu+size,page,PROT_NONE)==0);buffer=calloc(1,size);assert(buffer);imgMenu=(xpm_image_t){.width=1152,.height=864};for(size_t i=0;i<size/4;i++)((uint32_t*)menu)[i]=0x12345678;((uint32_t*)menu)[600*1152+300]=UINT32_MAX;
 /* The authored menu requests height600 at y500, beyond row863. */
 MenuTimerHandler();
 assert(((uint32_t*)buffer)[500*1152+275]==0x12345678);assert(((uint32_t*)buffer)[863*1152+939]==0x12345678);assert(((uint32_t*)buffer)[600*1152+300]==0&&"transparent original pixels preserve destination");assert(((uint32_t*)buffer)[499*1152+275]==0&&((uint32_t*)buffer)[500*1152+940]==0);
 memset(buffer,0,size);h_res=20;v_res=12;uint32_t small[16*10];for(unsigned i=0;i<160;i++)small[i]=0x10203040;xpm_image_t image={.width=16,.height=10};cleanBG(14,8,12,12,image,(uint8_t*)small);for(unsigned y=0;y<12;y++)for(unsigned x=0;x<20;x++)assert(((uint32_t*)buffer)[y*20+x]==((x>=14&&x<16&&y>=8&&y<10)?0x10203040:0));
 cleanBG(16,0,1,1,image,(uint8_t*)small);cleanBG(0,10,1,1,image,(uint8_t*)small);cleanBG(UINT_MAX,UINT_MAX,5,5,image,(uint8_t*)small);cleanBG(0,0,-1,-1,image,(uint8_t*)small);free(buffer);munmap(menu,size+page);return 0;}
''')

    def test_platform_duel_owning_xpm_declaration_preserves_pointer_returns(self):
        self.compile_and_run(COMMUNITY/'bm-vs--LCOM/proj/code',['game.c'],'int main(void){return 0;}',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Wno-implicit-function-declaration','-Werror=int-conversion','-O2'])

    def test_platform_duel_mouse_commands_use_data_port_and_consume_ack(self):
        self.compile_and_run(COMMUNITY/'bm-vs--LCOM/proj/code',['mouse.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "mouse.h"
#include <assert.h>
static unsigned phase,reads;static unsigned char expected,response;
int sys_outb(int port,uint32_t value){if(phase==0){assert(port==0x64&&value==0xd4);phase=1;}else{assert(phase==1&&port==0x60&&value==expected&&"PS2 commands belong to data port0x60");phase=2;}return 0;}
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){if(port==0x64){*value=phase==2?0x21:0;return 0;}assert(port==0x60&&phase==2);*value=response;phase=3;reads++;return 0;}
static void command(int(*run)(void),unsigned char byte,unsigned char reply,int status){phase=reads=0;expected=byte;response=reply;assert(run()==status);assert(phase==3&&reads==1&&"Each command consumes its ACK before packet decoding");}
int main(void){command(mouse_set_stream_mode,0xea,0xfa,0);command(mouse_enable_stream,0xf4,0xfa,0);command(mouse_set_stream_mode,0xea,0xfe,1);command(mouse_enable_stream,0xf4,0xfc,1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])

    def test_platform_duel_decodes_full_nine_bit_ps2_motion(self):
        self.compile_and_run(COMMUNITY/'bm-vs--LCOM/proj/code',['mouse.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "mouse.h"
#include <assert.h>
int main(void){const int deltas[]={-255,-128,-1,0,1,127,128,255};for(unsigned i=0;i<sizeof deltas/sizeof *deltas;i++){int delta=deltas[i];Sprite sprite={.x=400,.y=400,.width=10,.height=10};char packet[]={0x08|(delta<0?0x10:0),(unsigned char)delta,0};assert(packet_handler(packet,&sprite,NULL)==0);assert(sprite.x==400+delta&&sprite.y==400&&"X uses PS2 sign bit and an unsigned low byte");sprite.x=sprite.y=400;packet[0]=0x08|(delta<0?0x20:0);packet[1]=0;packet[2]=(unsigned char)delta;assert(packet_handler(packet,&sprite,NULL)==0);assert(sprite.x==400&&sprite.y==400-delta&&"Y uses PS2 sign bit and an unsigned low byte");}return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-fsigned-char','-O2'])


    def test_last_stand_hud_cleanup_preserves_borrowed_sprite_identifier(self):
        self.compile_and_run(COMMUNITY/'biromiro--feup-lcom/proj/src',['peripherals/video_gr.c','game/hud.c'],r'''#include "game/hud.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
static void *owned[4];
static unsigned allocations,releases;
void *checked_malloc(size_t size) {void *p=malloc(size);assert(p && allocations<4);owned[allocations++]=p;return p;}
void checked_free(void *p) {unsigned i=0;while(i<allocations && owned[i]!=p)i++;assert(i<allocations && "Destructor freed a borrowed pointer");owned[i]=NULL;releases++;free(p);}
uint8_t *xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t *image){(void)map;*image=(xpm_image_t){.width=1,.height=1,.type=type,.size=4};image->bytes=checked_malloc(4);return image->bytes;}
int main(void){const char *sprite[]={"1 1 1 1",". c #ffffff","."};char *id="borrowed-literal-id";xpm_object *object=create_sprite(sprite,id,19,23);assert(object && object->ID==id && object->x==19 && object->y==23);free_xpm_mem(object);assert(allocations==2 && releases==2);assert(strcmp(id,"borrowed-literal-id")==0);return 0;}
''',source_flags=['-O2','-Dmalloc=checked_malloc','-Dfree=checked_free'])

    def test_texture_manager_initializes_every_owned_pointer_slot(self):
        self.compile_and_run(COMMUNITY/'joaocsf--LCOM/proj/code',['texturemanager.c'],r'''#include "texturemanager.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
static unsigned allocations,deletions;
void*poison_alloc(size_t bytes){assert(bytes==sizeof(Sprite*)*SI_TOTAL);void*p=malloc(bytes);assert(p);memset(p,0xa5,bytes);allocations++;return p;}
void Sprite_Delete(Sprite*s){deletions++;abort();}
int main(void){TM_Init();assert(allocations==1);for(unsigned i=0;i<SI_TOTAL;i++)assert(TM_GetTexture(i)==NULL&&"Every texture slot must start empty before load or cleanup");assert(TM_GetTexture(SI_TOTAL)==NULL);TM_Exit();assert(deletions==0);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmalloc=poison_alloc','-include','lcom/libc.h','-Wno-int-conversion','-O2'])

    def test_top_down_shooter_font_uses_declared_void_renderer_contract(self):
        self.compile_and_run(COMMUNITY/'joaocsf--LCOM/proj/code',['font.c'],r'''#include "font.h"
#include "bmp.h"
#include "video_gr.h"
#include <assert.h>
#include <string.h>
static unsigned loads,draws;static unsigned char big[64],normal[64];
unsigned char*LoadBitmapFile(char*name,BitmapInfoHeader*info){loads++;if(strstr(name,"texto.bmp")){info->width=188;info->height=7;return big;}assert(strstr(name,"fontnormal.bmp"));info->width=282;info->height=9;return normal;}
void vg_writeFont(int x,int y,int imageWidth,int glyphWidth,int glyphHeight,char*pixels,char*text,unsigned short color){assert(x==14&&y==19&&color==0x7bef);assert(!strcmp(text,"Play"));if(draws++==0){assert(imageWidth==188&&glyphWidth==2&&glyphHeight==7&&pixels==(char*)big);}else{assert(imageWidth==282&&glyphWidth==3&&glyphHeight==9&&pixels==(char*)normal);}}
int main(void){Font_Write(14,19,"Play",0x7bef,FONT_BIG);Font_Write(14,19,"Play",0x7bef,FONT_NORMAL);assert(loads==2&&draws==2);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu89','-include','lcom/libc.h','-Werror=implicit-function-declaration','-O2'])

    def test_top_down_shooter_all_authored_units_use_declared_function_contracts(self):
        project=COMMUNITY/'joaocsf--LCOM/proj/code'
        sources=sorted(p.name for p in project.glob('*.c'))
        self.assertEqual(len(sources),28)
        self.compile_and_run(project,sources,'int main(void){return 0;}',source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu89','-include','lcom/libc.h','-Dmain=shooter_entry','-Werror=implicit-function-declaration','-O2'])

    def test_top_down_shooter_early_paths_have_defined_results(self):
        project=COMMUNITY/'joaocsf--LCOM/proj/code'
        cases=[
          ('map.c','char Map_Damage(',r'''#include <lcom/libc.h>
#include "map.h"
#include "networkmanager.h"
Vector2 dimSubImg={50,50},dimMap={1,1};char tile=8;char*mapData=&tile;
char NM_IsServer(void){return 0;}char NM_IsSinglePlayer(void){return 0;}
int Map_NetSendDamage(int x,int y,char dmg){abort();}void Map_UpdateDmg(int x,int y){abort();}
''',r'''#include <assert.h>
char Map_Damage(int,int,char);extern char tile;
int main(void){assert(Map_Damage(2,3,1)==0&&tile==8);assert(Map_Damage(-1,3,1)==0&&tile==8);return 0;}
'''),
          ('player.c','Vector2 Player_SyncPos(',r'''#include <lcom/libc.h>
#include "player.h"
#include "debug.h"
Vector2 Map_getDimensions(void){Vector2 d={100,100};return d;}
void Debug_LOG(char*t,char*v){}void Debug_LOGInt(char*t,long v){}
Vector2d Vector2_Lerp(Vector2d a,Vector2d b,double t){Vector2d d={a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t};return d;}
Vector2d Vector2_Sub(Vector2d a,Vector2d b){Vector2d d={a.x-b.x,a.y-b.y};return d;}
''',r'''#include "player.h"
#include <assert.h>
Vector2 Player_SyncPos(Player*);
int main(void){Player p={0};p.pos=(Vector2d){10,10};p.rect=(Rectangle){1,2,20,20};Vector2d rejected[]={{-1,0},{100,0},{0,-1},{0,100}};for(unsigned i=0;i<4;i++){p.newPos=rejected[i];Vector2 v=Player_SyncPos(&p);assert(v.x==0&&v.y==0&&p.pos.x==10&&p.pos.y==10&&p.rect.x==1&&p.rect.y==2);}p.newPos=(Vector2d){40,50};Vector2 v=Player_SyncPos(&p);assert(v.x==2&&v.y==3&&p.pos.x==13&&p.pos.y==14);return 0;}
'''),
          ('bullet.c','char Bullet_Update(',r'''#include <lcom/libc.h>
#include "bullet.h"
#include "enemymanager.h"
#include "enemy.h"
Enemy**EM_GetEnemies(void){return NULL;}unsigned int EM_GetEnemyCount(void){return 0;}
char Rectangle_CheckPointCollision(Rectangle r,Vector2d p){abort();}
char Map_CheckCollision(int x,int y){return x>50;}char Map_Damage(int x,int y,char dmg){return 1;}
''',r'''#include "bullet.h"
#include <assert.h>
int main(void){assert(Bullet_Update(NULL)==0);Bullet b={0};b.destroy=1;assert(Bullet_Update(&b)==0);b.destroy=0;b.vel=(Vector2d){1,2};assert(Bullet_Update(&b)==1&&b.pos.x==5&&b.pos.y==10);b.pos.x=50;assert(Bullet_Update(&b)==0);return 0;}
'''),
          ('networkmanager.c','int NM_Parser(',r'''#include <lcom/libc.h>
#include "networkmanager.h"
#include "list.h"
static List*buffer;static int playerConnected;unsigned removed;
#define Debug_LOG(...) ((void)0)
#define Debug_LOGInt(...) ((void)0)
#define List_Remove(...) (++removed)
#define NM_ListToArray(...) ((char*)0)
#define Map_ReadExplosion(...) ((void)0)
#define NM_ReadScore(...) ((void)0)
#define Player_NetReadPos(...) ((void)0)
#define Map_NetReadDamage(...) ((void)0)
''',r'''#include "list.h"
#include <stddef.h>
#include <assert.h>
int NM_Parser(ListNode*);extern unsigned removed;
int main(void){assert(NM_Parser(NULL)==1&&removed==0);ListNode n={0};assert(NM_Parser(&n)==0&&removed==1);return 0;}
'''),
          ('uart.c','inline char UART_IntHandler(',r'''#include <lcom/libc.h>
#include "uart.h"
static void*buffer;static char transEmpty;unsigned event,sent,errors;
static int read_port(unsigned p,unsigned long*v){*v=event;return 0;}
#define sys_inb(p,v) read_port(p,v)
#define Debug_LOG(...) ((void)0)
#define Debug_LOGInt(...) ((void)0)
#define UART_SendNextBuffer(...) (++sent)
#define UART_ReceiveChar(...) 0
#define UART_PrintError(...) (++errors)
#define List_Add(...) ((void)0)
''',r'''#include <assert.h>
char UART_IntHandler(unsigned);extern unsigned event,sent,errors;
int main(void){event=1;assert(UART_IntHandler(0x3f8)==1&&sent==0);event=10;assert(UART_IntHandler(0x3f8)==0&&sent==0);event=2;assert(UART_IntHandler(0x3f8)==1&&sent==1);event=6;assert(UART_IntHandler(0x3f8)==1&&errors==1);return 0;}
''')]
        for filename,signature,prefix,harness in cases:
            with self.subTest(owner=filename),tempfile.TemporaryDirectory(prefix='shooter-return-') as temporary:
                text=(project/filename).read_text();start=text.index(signature);opening=text.index('{',start);depth=1;end=opening+1
                while depth:
                    if text[end]=='{':depth+=1
                    elif text[end]=='}':depth-=1
                    end+=1
                folder=Path(temporary);(folder/'owner.c').write_text(prefix+text[start:end]+'\n')
                self.compile_and_run(folder,['owner.c'],harness,source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu89','-Werror=return-type','-Wno-macro-redefined','-O2'],include_dirs=[project])

    def test_top_down_shooter_pathfinder_chooses_lowest_cost_without_indeterminate_state(self):
        project=COMMUNITY/'joaocsf--LCOM/proj/code'
        text=(project/'map.c').read_text();body='inline Node* GetLowCost('+text.split('inline Node* GetLowCost(',1)[1].split('\nList* Map_NodeNeigh',1)[0]
        with tempfile.TemporaryDirectory(prefix='shooter-lowcost-') as temporary:
            folder=Path(temporary);(folder/'owner.c').write_text('#include <lcom/libc.h>\n#include "map.h"\n'+body)
            self.compile_and_run(folder,['owner.c'],r'''#include "map.h"
#include <stddef.h>
#include <assert.h>
int main(void){ListNode head={0};List list={&head};assert(GetLowCost(&list)==NULL);Node nodes[3]={{.tCost=30},{.tCost=2},{.tCost=12}};ListNode links[3]={{&nodes[0],&links[1]},{&nodes[1],&links[2]},{&nodes[2],NULL}};head.nextNode=links;assert(GetLowCost(&list)==&nodes[1]);nodes[0].tCost=1;assert(GetLowCost(&list)==&nodes[0]);assert(head.nextNode==links&&links[0].nextNode==&links[1]&&links[2].nextNode==NULL);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu89','-O2','-Werror=uninitialized','-ftrivial-auto-var-init=pattern'],include_dirs=[project])

    def test_top_down_shooter_mouse_decodes_complete_signed_nine_bit_deltas(self):
        self.compile_and_run(COMMUNITY/'joaocsf--LCOM/proj/code',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <lcom/libc.h>
#include "keyboard.h"
#include "mouse.c"
#include <assert.h>
int main(void){int values[]={-255,-128,-1,0,1,127,128,255};for(unsigned i=0;i<sizeof values/sizeof *values;i++){int d=values[i];mouse_init();packet[0]=8|(d<0?16:0);packet[1]=(unsigned char)d;packet[2]=0;mouse_update();Vector2d p=mouse_getPos();assert(p.x==d&&p.y==0&&"X movement must use header sign, not low-byte char sign");mouse_init();packet[0]=8|(d<0?32:0);packet[1]=0;packet[2]=(unsigned char)d;mouse_update();p=mouse_getPos();assert(p.x==0&&p.y==-d&&"Y movement must use header sign, not low-byte char sign");}return 0;}
''')


    def test_snake_x3_line_rounding_keeps_standard_math_namespace(self):
        project=COMMUNITY/'TrocaTudo95--LCOM-feup/proj/src'
        text=(project/'video_gr.c').read_text()
        signature='static short pixel_round(' if 'static short pixel_round(' in text else 'short round('
        block=text[text.index(signature):text.index('int vg_draw_xpm(')]
        prefix=r'''#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define OK 0
char pixels[100];static char*sec_buffer=pixels;static unsigned h_res=10;
'''
        harness=r'''#include <math.h>
#include <assert.h>
#include <string.h>
extern char pixels[100];
int vg_line(unsigned short,unsigned short,unsigned short,unsigned short,unsigned long);
int main(void){memset(pixels,0,sizeof pixels);assert(vg_line(2,2,6,4,7)==0);int expected[]={33,34,45,46};for(int i=0;i<100;i++){int yes=0;for(unsigned j=0;j<4;j++)if(i==expected[j])yes=1;assert(pixels[i]==(yes?7:0));}assert(round(2.5)==3.0);return 0;}
'''
        with tempfile.TemporaryDirectory(prefix='snake-line-') as temp:
            folder=Path(temp);(folder/'owner.c').write_text(prefix+block)
            self.compile_and_run(folder,['owner.c'],harness,source_flags=['-std=gnu99','-O2'])

    def test_snake_x3_direction_decodes_complete_signed_mouse_range(self):
        project=COMMUNITY/'TrocaTudo95--LCOM-feup/proj/src'
        text=(project/'game.c').read_text();block=text[text.index('int getDirection('):text.index('bool canEat(')]
        prefix=r'''#include <minix/drivers.h>
#include "game.h"
#include <assert.h>
static int gx,gy;
int mouse_get_packet(unsigned long packet[],unsigned long irq,size_t*counter){packet[0]=8|(gx<0?16:0)|(gy<0?32:0);packet[1]=gx&255;packet[2]=gy&255;return 0;}
'''
        harness=r'''#define LCOM_COMPAT_LEGACY 1
#include "game.h"
#include <assert.h>
extern int gx,gy;
int main(void){int values[]={-255,-128,-1,1,127,128,255};for(unsigned i=0;i<7;i++){bool horizontal=false;short delta=6;size_t counter=0;unsigned long packet[3];Rectangle rect={0};gx=values[i];gy=0;assert(getDirection(packet,&horizontal,&delta,0,&counter,&rect)==0);assert(horizontal&&delta==(gx<0?-6:6));horizontal=true;delta=6;gx=0;gy=values[i];assert(getDirection(packet,&horizontal,&delta,0,&counter,&rect)==0);assert(!horizontal&&delta==(gy<0?6:-6));}return 0;}
'''
        prefix=prefix.replace('static int gx,gy;','int gx,gy;')
        with tempfile.TemporaryDirectory(prefix='snake-direction-') as temp:
            folder=Path(temp);(folder/'owner.c').write_text(prefix+block)
            self.compile_and_run(folder,['owner.c'],harness,source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu99','-O2'],include_dirs=[project])

    def test_snake_x3_mouse_ack_does_not_consume_pending_escape(self):
        project=COMMUNITY/'TrocaTudo95--LCOM-feup/proj/src'
        def function(text,signature):
            start=text.index(signature);opening=text.index('{',start);end=opening+1;depth=1
            while depth:
                if text[end]=='{':depth+=1
                elif text[end]=='}':depth-=1
                end+=1
            return text[start:end]
        mouse=(project/'mouse.c').read_text();keyboard=(project/'keyboard.c').read_text()
        queue=keyboard[keyboard.index('/* Keyboard bytes encountered'):keyboard.index('int kbd_pressed_esc(')] if '/* Keyboard bytes encountered' in keyboard else ''
        prefix=r'''#include <minix/drivers.h>
#include "mouse.h"
#include "keyboard.h"
#include <assert.h>
static bool isSubscribed=true;
unsigned stage,index0;int override=-1;
static unsigned bytes[]={1,129,250};
static int input(unsigned port,unsigned long*value){if(port==STAT_REG){*value=override>=0?(unsigned)override:stage==2?(index0<2?1:index0<3?33:0):0;return 0;}assert(port==OUT_BUF);assert(stage==2&&"0xD4 does not return an ACK");assert(index0<3);*value=bytes[index0++];return 0;}
static int output(unsigned port,unsigned long value){if(stage==0){assert(port==KBC_CMD_REG&&value==WRITE_BYTE_MOUSE);stage=1;}else{assert(stage==1&&port==IN_BUF&&value==ENABLE_DATA_REPORT);stage=2;}return 0;}
#define sys_inb(p,v) input(p,v)
#define sys_outb(p,v) output(p,v)
#define tickdelay(...) 0
#define micros_to_ticks(...) 1
'''
        harness=r'''#include "mouse.h"
#include "keyboard.h"
#include <assert.h>
extern unsigned stage,index0;extern int override;
int main(void){assert(sendCmdToKBC(WRITE_BYTE_MOUSE,ENABLE_DATA_REPORT)==0);assert(stage==2&&index0==3);assert(kbd_pressed_esc()==ESC_PRESSED);assert(kbd_pressed_esc()==ESC_RELEASED);assert(kbd_pressed_esc()==OK&&index0==3);override=33;assert(kbd_pressed_esc()==OK&&index0==3);override=129;assert(kbd_pressed_esc()==OK&&index0==3);return 0;}
'''
        with tempfile.TemporaryDirectory(prefix='snake-ack-') as temp:
            folder=Path(temp);(folder/'owner.c').write_text(prefix+queue+function(mouse,'int sendCmdToKBC(')+'\n'+function(keyboard,'int kbd_pressed_esc('))
            self.compile_and_run(folder,['owner.c'],harness,source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu99','-O2'],include_dirs=[project])

    def test_snake_x3_button_only_packets_preserve_previous_heading(self):
        project=COMMUNITY/'TrocaTudo95--LCOM-feup/proj/src'
        text=(project/'game.c').read_text();block=text[text.index('int getDirection('):text.index('bool canEat(')]
        prefix=r'''#include <minix/drivers.h>
#include "game.h"
#include <assert.h>
unsigned flags;
int mouse_get_packet(unsigned long packet[],unsigned long irq,size_t*counter){packet[0]=8|flags;packet[1]=packet[2]=0;return 0;}
'''
        harness=r'''#define LCOM_COMPAT_LEGACY 1
#include "game.h"
#include <assert.h>
extern unsigned flags;
int main(void){for(unsigned direction=0;direction<4;direction++)for(unsigned buttons=0;buttons<8;buttons++){bool horizontal=direction<2;short delta=(direction%2)?-6:6;unsigned long packet[3];size_t ticks=0;Rectangle apple={0};flags=buttons;assert(getDirection(packet,&horizontal,&delta,1,&ticks,&apple)==OK);assert(horizontal==(direction<2)&&delta==((direction%2)?-6:6)&&"button-only packet must not steer the snake");assert((packet[0]&7)==buttons);}return 0;}
'''
        with tempfile.TemporaryDirectory(prefix='snake-buttons-') as temp:
            folder=Path(temp);(folder/'owner.c').write_text(prefix+block)
            self.compile_and_run(folder,['owner.c'],harness,source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu99','-O2'],include_dirs=[project])

    def test_snake_x3_packet_assembly_survives_interleaved_timer_notifications(self):
        project=COMMUNITY/'TrocaTudo95--LCOM-feup/proj/src'
        text=(project/'mouse.c').read_text();start=text.index('int mouse_get_packet(');end=text.index('void printMousePacket(',start)
        prefix=r'''#include <minix/drivers.h>
#include "mouse.h"
#include <assert.h>
static unsigned scenario,event_index,byte_index,commands,unsubscribed;
static const unsigned interleaved[]={1,16,1,16,1,16};
static const unsigned idle[]={1,1,1};
static const unsigned partial[]={16,1,1,1};
static const unsigned contiguous[]={16,16,16};
static const unsigned bytes[]={8,250,255};
int mouse_subscribe_int(void){return 4;}
int mouse_unsubscribe_int(void){unsubscribed++;return 0;}
int sendCmdToKBC(unsigned long command,unsigned long arg){assert(command==WRITE_BYTE_MOUSE);assert(arg==(commands==0?SET_STREAM:commands==1?ENABLE_DATA_REPORT:DISABLE_DATA_REPORT));commands++;return 0;}
int mouse_int_handler(unsigned long*out){assert(byte_index<3);*out=bytes[byte_index++];return 0;}
int receive_event(endpoint_t who,message*msg,int*status){if(scenario==4){assert(event_index++==0);return 5;}const unsigned*events=scenario==0?interleaved:scenario==1?idle:scenario==2?partial:contiguous;unsigned count=scenario==0?6:scenario==1?3:scenario==2?4:3;assert(event_index<count&&"packet assembly must remain bounded after three idle timer ticks");msg->m_source=HARDWARE;msg->NOTIFY_ARG=events[event_index++];*status=IPC_STATUS_NOTIFY;return 0;}
#define driver_receive receive_event
'''
        harness=r'''int main(void){for(scenario=0;scenario<5;scenario++){event_index=byte_index=commands=unsubscribed=0;size_t ticks=0;unsigned long packet[3]={99,99,99};int result=mouse_get_packet(packet,1,&ticks);if(scenario==0||scenario==3){assert(result==OK&&"three mouse bytes must not be lost to interleaved timer notifications");assert(byte_index==3&&packet[0]==8&&packet[1]==250&&packet[2]==255);assert(commands==3);assert(ticks==(scenario==0?3:0));}else{assert(result==NO_PACKETS);assert(ticks==(scenario==4?0:3)&&commands==3);}assert(unsubscribed==1);}return 0;}
'''
        with tempfile.TemporaryDirectory(prefix='snake-packet-') as temp:
            folder=Path(temp);(folder/'owner.c').write_text(prefix+text[start:end]+'\n'+harness)
            self.compile_and_run(folder,['owner.c'],'',source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu99','-O2'],include_dirs=[project])

    def test_artist_actual_name_entry_uses_char_returning_keyboard_owner(self):
        self.compile_and_run(COMMUNITY/'miguel-botelho--LCOM/proj/code',['menu.c','keyboard.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include "menu.h"
#include "keyboard.h"
#include <assert.h>
char OPTION=GET_NAME,name[11],word[11],guess_ai[11];int length,ai_or_pvp,contador_high,espaco;
static char buffer[1024*768*2];static Bitmap fonts[86];static unsigned letters;
unsigned getHRes(void){return 1024;}char*getHumanMachine(void){return buffer;}char*getScreenBuffer(void){return buffer;}char*getMouseBuffer(void){return buffer;}char*getVideoMem(void){return buffer;}
void screen_to_mouse(char*screen,char*mouse){}void mouse_to_video(char*mouse,char*video){}
void drawBitmap(Bitmap*b,int x,int y,Alignment align,char*screen){}
void drawMouse(Bitmap*b,int x,int y,Alignment align,char*screen){for(unsigned i=0;i<86;i++)if(b==&fonts[i])letters++;}
int main(void){Bitmap*keys[86];for(unsigned i=0;i<86;i++)keys[i]=&fonts[i];bitmap_struct bitmaps={0};assert(get_char(0x1e)=='A'&&get_char(0x31)=='N');printName(bitmaps,keys,0x1e);printName(bitmaps,keys,0x31);printName(bitmaps,keys,0x1e);assert(length==3&&name[0]=='A'&&name[1]=='N'&&name[2]=='A'&&letters==6);printName(bitmaps,keys,KEY_ENTER);assert(OPTION==HUMAN_VS_MACHINE);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu11','-O2','-Werror=implicit-function-declaration'])

    def test_artist_ten_character_name_stays_inside_original_storage(self):
        from unittest.mock import patch
        with patch.dict(os.environ,{'CC':os.environ.get('CC','cc')+' -fsanitize=bounds -fno-sanitize-recover=all'}):
            self.compile_and_run(COMMUNITY/'miguel-botelho--LCOM/proj/code',['menu.c','keyboard.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include "menu.h"
#include "keyboard.h"
#include <assert.h>
#include <string.h>
char OPTION=GET_NAME,name[11],word[11],guess_ai[11];int length,ai_or_pvp,contador_high,espaco;static unsigned frames;
static char buffer[1024*768*2];static Bitmap fonts[86];static unsigned letters;
unsigned getHRes(void){return 1024;}char*getHumanMachine(void){return buffer;}char*getScreenBuffer(void){return buffer;}char*getMouseBuffer(void){return buffer;}char*getVideoMem(void){return buffer;}
void screen_to_mouse(char*screen,char*mouse){}void mouse_to_video(char*mouse,char*video){}
void drawBitmap(Bitmap*b,int x,int y,Alignment align,char*screen){frames++;}
void drawMouse(Bitmap*b,int x,int y,Alignment align,char*screen){for(unsigned i=0;i<86;i++)if(b==&fonts[i])letters++;}
int main(void){Bitmap*keys[86];for(unsigned i=0;i<86;i++)keys[i]=&fonts[i];bitmap_struct bitmaps={0};assert(get_char(0x1e)=='A'&&get_char(0x31)=='N');for(unsigned i=0;i<10;i++)printName(bitmaps,keys,0x1e);assert(length==10);printName(bitmaps,keys,KEY_ENTER);assert(name[10]=='\0');assert(OPTION==HUMAN_VS_MACHINE);assert(frames==1&&"maximum length must enter the same original game frame as shorter names");OPTION=GET_NAME;length=0;frames=0;memset(name,0,sizeof name);for(unsigned i=0;i<10;i++){printName(bitmaps,keys,0x1e);printName(bitmaps,keys,0x9e);}assert(length==10&&OPTION==GET_NAME&&"releasing the tenth letter must not cancel name entry");printName(bitmaps,keys,KEY_ENTER);assert(name[10]=='\0'&&OPTION==HUMAN_VS_MACHINE&&frames==1);return 0;}
''',source_flags=['-include','keyboard.h','-include','mouse.h','-DLCOM_COMPAT_LEGACY=1','-std=gnu11','-O2','-Werror=implicit-function-declaration'])

    def test_artist_saved_winners_retain_record_boundaries_and_terminated_names(self):
        self.compile_and_run(COMMUNITY/'miguel-botelho--LCOM/proj/code',['read_write.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include "read_write.h"
#include <assert.h>
#include <string.h>
int main(void){position_t records[5]={{0}},readback[5];const char*names[]={"ANA","ABCDEFGHIJ","KIM\n","LENHADOR","Q"};FILE*f=tmpfile();assert(f);for(unsigned i=0;i<5;i++){records[i].day=17+i;records[i].month=10;records[i].year=26;records[i].hour=5;records[i].minutes=33;records[i].score=9893-i;strcpy(records[i].name,names[i]);write_position(f,&records[i]);}rewind(f);char expected[]="17 10 26 05 33\n09893\nANA\n";char prefix[sizeof expected]={0};assert(fread(prefix,1,sizeof expected-1,f)==sizeof expected-1);assert(!strcmp(prefix,expected)&&"newly won names need a newline before the next record");rewind(f);memset(readback,0xa5,sizeof readback);for(unsigned i=0;i<5;i++){read_position(f,&readback[i]);assert(readback[i].day==17+i&&readback[i].month==10&&readback[i].year==26&&readback[i].hour==5&&readback[i].minutes==33&&readback[i].score==9893-i);assert(memchr(readback[i].name,0,sizeof readback[i].name)&&"loaded name must terminate within its original buffer");assert(!strcmp(readback[i].name,i==2?"KIM":names[i])&&"loaded names must terminate, omit delimiters and preserve the next record");}assert(fgetc(f)==EOF);fclose(f);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-std=gnu11','-O2'])


    def test_penguin_run_keyboard_subscription_returns_status_and_preserves_notification_bit(self):
        self.compile_and_run(COMMUNITY/'SergioEstevao11--FEUP-LCOM/proj',['keyboard.c'],r'''#include <lcom/lcf.h>
#include "keyboard.h"
#include <assert.h>
static int fail;
int sys_irqsetpolicy(int irq,int policy,int*hook){assert(irq==1&&*hook==1);assert(policy==(IRQ_REENABLE|IRQ_EXCLUSIVE));return fail?-1:0;}
int main(void){uint32_t bit=99;assert(kbd_subscribe_int(&bit)==0&&"successful subscription must return status0, not hook1 which caller treats as failure");assert(bit==1);fail=1;assert(kbd_subscribe_int(&bit)==1);return 0;}
''')


    def test_penguin_run_pixel_writer_preserves_native_pointer_and_pixel_neighbors(self):
        self.compile_and_run(COMMUNITY/'SergioEstevao11--FEUP-LCOM/proj',['graphics.c'],r'''#include <lcom/lcf.h>
#include "graphics.h"
#include <assert.h>
extern void*video_buffer;extern uint16_t res_x,res_y,bytes_pixel;
int main(void){uint8_t bytes[32];memset(bytes,0xA5,sizeof bytes);video_buffer=bytes+4;res_x=4;res_y=2;bytes_pixel=3;change_color(2,1,0x123456);for(unsigned i=0;i<32;i++){uint8_t expected=i==22?0x56:i==23?0x34:i==24?0x12:0xA5;assert(bytes[i]==expected);}change_color(0,0,TrasparencyColor);assert(bytes[4]==0xA5);return 0;}
''')


    def test_artist_name_renderers_stop_at_the_saved_name_terminator(self):
      self.compile_and_run(COMMUNITY/'miguel-botelho--LCOM/proj/code',['menu.c','keyboard.c','global_variables.c'],r'''#define LCOM_COMPAT_LEGACY 1
    #include "menu.h"
    #include "keyboard.h"
    #include <assert.h>
    static char buffer[1024*768*2];static Bitmap fonts[86],digits[10];static unsigned glyphs;static int codes[64];
    unsigned getHRes(void){return 1024;}unsigned getVRes(void){return 768;}char*getHumanMachine(void){return buffer;}char*getScreenBuffer(void){return buffer;}char*getMouseBuffer(void){return buffer;}char*getVideoMem(void){return buffer;}
    void screen_to_mouse(char*a,char*b){}void mouse_to_video(char*a,char*b){}void drawBitmap(Bitmap*b,int x,int y,Alignment a,char*s){}void createBitmap(void){}
    void drawMouse(Bitmap*b,int x,int y,Alignment a,char*s){for(unsigned i=0;i<86;i++)if(b==&fonts[i])codes[glyphs++]=i;}
    int lcom_compat_remove(const char*p){return 0;}int lcom_compat_rename(const char*a,const char*b){return 0;}
    static Bitmap*keys[86];static Bitmap*numbers[10];
    static void init(void){for(unsigned i=0;i<86;i++)keys[i]=&fonts[i];for(unsigned i=0;i<10;i++)numbers[i]=&digits[i];}
    int main(void){init();position_t player={0};memcpy(player.name,"ANA\0XU",6);displayScore(numbers,&player,keys,300);assert(glyphs==3&&codes[0]==KEY_A&&codes[1]==KEY_N&&codes[2]==KEY_A);glyphs=0;displayHighScore(numbers,player,keys,300);assert(glyphs==3&&codes[0]==KEY_A&&codes[1]==KEY_N&&codes[2]==KEY_A);return 0;}
    ''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])
    def test_artist_actual_win_records_initialize_name_and_guess_tails(self):
      self.compile_and_run(COMMUNITY/'miguel-botelho--LCOM/proj/code',['menu.c','keyboard.c','global_variables.c'],r'''#define LCOM_COMPAT_LEGACY 1
    #include "menu.h"
    #include "keyboard.h"
    #include <assert.h>
    static char buffer[1024*768*2];static Bitmap fonts[86],digits[10];static unsigned glyphs;static int codes[64];
    unsigned getHRes(void){return 1024;}unsigned getVRes(void){return 768;}char*getHumanMachine(void){return buffer;}char*getScreenBuffer(void){return buffer;}char*getMouseBuffer(void){return buffer;}char*getVideoMem(void){return buffer;}
    void screen_to_mouse(char*a,char*b){}void mouse_to_video(char*a,char*b){}void drawBitmap(Bitmap*b,int x,int y,Alignment a,char*s){}void createBitmap(void){}
    void drawMouse(Bitmap*b,int x,int y,Alignment a,char*s){for(unsigned i=0;i<86;i++)if(b==&fonts[i])codes[glyphs++]=i;}
    int lcom_compat_remove(const char*p){return 0;}int lcom_compat_rename(const char*a,const char*b){return 0;}
    static Bitmap*keys[86];static Bitmap*numbers[10];
    static void init(void){for(unsigned i=0;i<86;i++)keys[i]=&fonts[i];for(unsigned i=0;i<10;i++)numbers[i]=&digits[i];}
    int main(void){init();bitmap_struct b={0};OPTION=HUMAN_VS_MACHINE;strcpy(name,"ANA");strcpy(word,"CAT");strcpy(guess_ai,"CAT0");length_word=3;contador_high=0;printMachine(b,keys,KEY_ENTER,numbers);assert(OPTION==STATIC&&!strcmp(top_highscores.first.name,"ANA"));for(unsigned i=3;i<sizeof(top_highscores.first.name);i++)assert(top_highscores.first.name[i]==0&&"WIN record must not retain patterned stack tail bytes");for(unsigned i=3;i<sizeof(top_highscores.first.word);i++)assert(top_highscores.first.word[i]==0);return 0;}
    ''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-ftrivial-auto-var-init=pattern'])

    def test_penguin_entry_dispatches_original_void_game_functions(self):
        self.compile_and_run(COMMUNITY/'pedrofaria09--LCOM/Trabalho/src', ['projeto.c'], r'''#include <minix/drivers.h>
#include <assert.h>
int student_entry(int,char**);static unsigned starts,scores,menus,boots;
void sef_startup(void){boots++;}
void start(char*dir,char*name){assert(!strcmp(dir,"saved")&&!strcmp(name,"ANA"));starts++;}
void menu(char*dir,char*name){assert(!strcmp(dir,"saved")&&!strcmp(name,"ANA"));menus++;}
void viewScore(char*dir){assert(!strcmp(dir,"saved"));scores++;}
int main(void){char*a[]={"projeto","start","saved","ANA",NULL};assert(student_entry(4,a)==0&&starts==1);a[1]="menu";assert(student_entry(4,a)==0&&menus==1);a[1]="score";assert(student_entry(3,a)==0&&scores==1);assert(boots==3);return 0;}
''', source_flags=['-std=gnu89', '-DLCOM_COMPAT_LEGACY=1', '-include', 'lcom/libc.h', '-Dmain=student_entry', '-O2', '-Werror=implicit-function-declaration'])

    def test_penguin_score_append_initializes_storage_and_preserves_csv_records(self):
        with tempfile.TemporaryDirectory() as temporary:
            project = Path(temporary)
            (project/'owner.c').write_text('#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include "highscore.h"\n')
            self.compile_and_run(project, ['owner.c'], r'''#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <unistd.h>
void addUserInfo(char*,char*,int);
int main(void){char dir[]="/tmp/penguin-scores-XXXXXX";assert(mkdtemp(dir));char path[256];snprintf(path,sizeof(path),"%s/userinfo.txt",dir);FILE*f=fopen(path,"w");assert(f);fputs("PEDRO,20",f);fclose(f);addUserInfo(dir,"ANA",25);addUserInfo(dir,"BOB",INT_MAX);f=fopen(path,"r");assert(f);char actual[256]={0};size_t n=fread(actual,1,sizeof(actual)-1,f);fclose(f);assert(n==strlen("PEDRO,20\nANA,25\nBOB,2147483647")&&!strcmp(actual,"PEDRO,20\nANA,25\nBOB,2147483647"));addUserInfo("/no-such-penguin-dir","ANA",25);unlink(path);rmdir(dir);return 0;}
''', source_flags=['-std=gnu89', '-O2', '-ftrivial-auto-var-init=pattern', '-fstack-protector-all'], include_dirs=[COMMUNITY/'pedrofaria09--LCOM/Trabalho/src'])

    def test_labwars_text_timer_allocates_complete_owned_fields(self):
        project=COMMUNITY/'dmfrodrigues--feup-lcom/proj/src'
        self.compile_and_run(project,['project/src/proj_func.c'],r'''#include <lcom/lcf.h>
#include "proj_func.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static text_timer_t timer;
static unsigned char text_storage[1024];
void*checked_malloc(size_t bytes){if(bytes!=sizeof(timer)){fprintf(stderr,"Text timer allocation %zu cannot hold %zu-byte object\n",bytes,sizeof(timer));exit(1);}memset(&timer,0xa5,sizeof(timer));return &timer;}
text_t*text_ctor(const font_t*font,const char*text){assert(font&&strcmp(text,"000s")==0);return (text_t*)text_storage;}
void text_set_color(text_t*text,uint32_t color){assert(text==(text_t*)text_storage);}
int main(void){font_t*font=(font_t*)text_storage;assert(text_timer_ctor(NULL)==NULL);assert(text_timer_ctor(font)==&timer);assert(timer.time==0&&timer.text==(text_t*)text_storage);puts("Actual text timer constructor owns complete time and text fields");return 0;}
''',
            source_flags=['-O2','-Dmalloc=checked_malloc'],
            include_dirs=[project/folder for folder in ['project/include','libs','libs/graph/include','libs/kbc/include','libs/rtc/include','libs/timer/include','libs/uart/include','libs/classes/include','libs/utils/include']])

    def test_watt_score_constructor_allocates_complete_owned_fields(self):
        self.compile_and_run(COMMUNITY/'Educorreia932--FEUP-LCOM/proj/src',
            ['math_utils.c','geometry.c'],r"""#include <lcom/lcf.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
static void *checked_calloc(size_t,size_t);
#define calloc checked_calloc
#include "ui_elements.c"
#undef calloc
static int allocations;
static void *checked_calloc(size_t count,size_t size) {
    if(allocations++==0)assert(count*size==sizeof(Score_t)&&"score allocation must own all fields, not only a pointer");
    void *p=malloc(count*size);assert(p);memset(p,0,count*size);return p;
}
Sprite_t *new_sprite(float x,float y,int count,...) {assert(x==0&&y==0&&count==1);va_list ap;va_start(ap,count);const char *file=va_arg(ap,const char*);assert(strncmp(file,"ui/",3)==0);va_end(ap);return (Sprite_t*)file;}
void free_sprite(Sprite_t *sprite) {assert(sprite);}
int main(void) {Score_t *score=new_score(12,34,123,3,0xabcd);assert(score);assert(score->x==12&&score->y==34&&score->size==3&&score->color==0xabcd&&score->value==123&&score->max_score==999);assert(score->numbers[0]->value==3&&score->numbers[1]->value==2&&score->numbers[2]->value==1);free_score(score);return 0;}
""",source_flags=['-O2'])

    def test_space_invaders_graphics_setup_does_not_reissue_bios_return_registers(self):
        self.compile_and_run(COMMUNITY/'joaotrindade--LCOM/proj',['video_gr.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <machine/int86.h>
#include <assert.h>
static unsigned calls,maps;static unsigned char framebuffer[1024*768];
void sef_startup(void){}
int lcom_compat_legacy_int86(struct reg86u*r){assert(r->u.b.intno==0x10&&r->u.w.ax==0x4f02&&r->u.w.bx==0x4105&&"BIOS output AX004F must never be resubmitted as a new INT10 request");calls++;r->u.w.ax=0x004f;return 0;}
int sys_privctl(endpoint_t ep,int request,void*arg){struct minix_mem_range*range=arg;assert(ep==SELF&&request==SYS_PRIV_ADD_MEM&&range->mr_base==0xd0000000u&&range->mr_limit-range->mr_base==sizeof framebuffer);return 0;}
void*vm_map_phys(endpoint_t ep,void*base,size_t size){assert(ep==SELF&&(uintptr_t)base==0xd0000000u&&size==sizeof framebuffer);maps++;return framebuffer;}
void panic(const char*format,...){abort();}
void*vg_init(unsigned long);
int main(void){assert((uintptr_t)vg_init(0x105)==0xd0000000u&&calls==1&&maps==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Wno-implicit-function-declaration'])

    def test_space_impact_menu_and_ship_use_the_declared_full_width_pixmap_pointer(self):
        self.compile_and_run(COMMUNITY/'joaotrindade--LCOM/proj',['video_gr.c','menu.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <machine/int86.h>
#include <assert.h>
static unsigned calls;static unsigned char framebuffer[1024*768];
void sef_startup(void){}
int lcom_compat_legacy_int86(struct reg86u*r){assert(r->u.b.intno==0x10&&r->u.w.ax==0x4f02);r->u.w.ax=0x004f;return 0;}
int sys_privctl(endpoint_t ep,int request,void*arg){return 0;}
void*vm_map_phys(endpoint_t ep,void*base,size_t size){return framebuffer;}
void panic(const char*format,...){abort();}
char*read_xpm(char*map[],int*width,int*height){char*p=malloc(1);assert(p&&(uintptr_t)p>UINT32_MAX&&"exercise a real full-width host pointer");*p=42;*width=1;*height=1;calls++;return p;}
void*vg_init(unsigned long);void drawMenu(int);void drawMainShip(int,int);
int main(void){vg_init(0x105);drawMenu(0);assert(framebuffer[256*1024+341]==42);drawMainShip(100,0);assert(framebuffer[100*1024]==42&&calls==2);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Dmain=student_entry','-Wno-implicit-function-declaration'])

    def test_space_impact_reads_original_minix_records_and_saves_the_same_binary_format(self):
        self.compile_and_run(COMMUNITY/'joaotrindade--LCOM/proj',['proj.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "proj.h"
#include <assert.h>
#undef fopen
#undef remove
extern int last_highscore;
static char save[]="/tmp/space-impact-score-XXXXXX";static int output;
FILE*lcom_compat_fopen(const char*path,const char*mode){assert(strcmp(path,"/usr/src/drivers/proj/highscores")==0);return fopen(output?save:"ORIGINAL",mode);}
int main(void){player_score records[50]={0};int fd=mkstemp(save);assert(fd>=0);close(fd);readHighScores(records);assert(last_highscore==6&&"read the six complete original forty-byte Minix records on every host ABI");assert(strcmp(records[0].nome,"JOAO")==0&&records[0].pontuacao==300);assert(records[1].pontuacao==0&&records[2].pontuacao==300&&records[3].pontuacao==400&&records[4].pontuacao==500);assert(records[4].nome[14]=='\0');assert(records[0].dia==38&&records[0].mes==18&&records[0].ano==18&&records[0].hora==21&&records[0].minuto==56);assert(strcmp(records[6].nome,"EMPTY")==0);output=1;strcpy(records[0].nome,"ANA");records[0].pontuacao=12345;last_highscore=1;writeHighScores(records);FILE*f=fopen(save,"rb");unsigned char disk[41];assert(fread(disk,1,sizeof disk,f)==40);fclose(f);assert(memcmp(disk,"ANA\0",4)==0&&disk[16]==0x39&&disk[17]==0x30&&disk[18]==0&&disk[19]==0);for(int i=4;i<16;i++)assert(disk[i]==0);readHighScores(records);assert(last_highscore==1&&strcmp(records[0].nome,"ANA")==0&&records[0].pontuacao==12345);f=fopen(save,"wb");fputc(1,f);fclose(f);readHighScores(records);assert(last_highscore==0&&strcmp(records[0].nome,"EMPTY")==0);remove(save);return 0;}
'''.replace('ORIGINAL',str(COMMUNITY/'joaotrindade--LCOM/proj/highscores')),source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Wno-implicit-function-declaration'])

    def test_space_impact_name_entry_bounds_every_make_event_and_empty_names(self):
        self.compile_and_run(COMMUNITY/'joaotrindade--LCOM/proj',['proj.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "proj.h"
#include <assert.h>
#undef fopen
#undef remove
static unsigned count,letters,limit=20;static char save[]="/tmp/space-impact-name-XXXXXX";extern player_score highscores[50];extern int last_highscore;
FILE*lcom_compat_fopen(const char*path,const char*mode){return fopen(save,mode);}
int driver_receive(endpoint_t source,message*m,int*status){assert(count<limit+2);memset(m,0,sizeof*m);m->m_source=HARDWARE;m->NOTIFY_ARG=1;*status=NOTIFY;return 0;}
unsigned char read_scancode(void){return count++<limit?0x1e:0x1c;}
void drawLetter(char letter,int x,int y,int erase){assert(letter=='A');letters++;}
void drawInterface(int id,int x,int y,int erase){}int vg_fill(unsigned long color){return 0;}
void get_data(long*d,long*m,long*y,long*h,long*n){*d=*m=*y=*h=*n=1;}
int main(void){int fd=mkstemp(save);assert(fd>=0);close(fd);player_score player={0};player.pontuacao=100;insertNome(player,1,0,(message){0});assert(letters==14&&strlen(highscores[0].nome)==14&&highscores[0].nome[14]=='\0'&&count==21);count=letters=limit=0;last_highscore=0;insertNome(player,1,0,(message){0});assert(highscores[0].nome[0]=='\0'&&letters==0&&count==1);remove(save);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-fstack-protector-all','-ftrivial-auto-var-init=pattern','-Wno-implicit-function-declaration'])



    def test_cosmic_four_keyboard_keeps_the_notification_bit_before_receiving_opaque_handle(self):
        self.compile_and_run(COMMUNITY/'hpnog--FEUP-LCOM/code',['keyboard.c'],r'''#define LCOM_COMPAT_LEGACY 1
#define LCOM_COMPAT_LEGACY_LM_STATUS 1
#include <minix/drivers.h>
#include "keyboard.h"
#include <assert.h>
int kbd_unsubscribe_int(void);
static unsigned enabled,disabled,removed;
int sys_irqsetpolicy(int irq,int policy,int*hook){assert(irq==1&&*hook==9);*hook=24;return 0;}
int sys_irqenable(int*hook){assert(*hook==24);enabled++;return 0;}
int sys_irqdisable(int*hook){assert(*hook==24);disabled++;return 0;}
int sys_irqrmpolicy(int*hook){assert(*hook==24);removed++;return 0;}
int main(void){assert(kbd_subscribe_int()==BIT(9)&&"notification bit9 must not become kernel handle24");assert(enabled==1);assert(kbd_unsubscribe_int()==0&&disabled==1&&removed==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-O2','-Wno-implicit-function-declaration'])


    def test_huetris_manager_keeps_authored_indices_and_uses_saved_kernel_handles_for_cleanup(self):
        self.compile_and_run(COMMUNITY/'edualm--LCOM-Huetris/code',['interrupt.c'],r'''#define LCOM_COMPAT_LEGACY 1
#define LCOM_COMPAT_LEGACY_LM_STATUS 1
#include <lcom/compat.h>
#include "interrupt.h"
#include <assert.h>
static unsigned sets,enables,disables,removes;static int expected[]={16,11,26,21};
int sys_irqsetpolicy(int irq,int policy,int*hook){assert(sets<4&&*hook==(sets==3?1:(int)sets));*hook=11+5*sets++;return 0;}
int sys_irqenable(int*hook){assert(*hook==11+5*enables++);return 0;}
int sys_irqdisable(int*hook){assert(disables<4&&*hook==expected[disables++]);return 0;}
int sys_irqrmpolicy(int*hook){assert(removes<4&&*hook==expected[removes++]);return 0;}
int main(void){int_init();assert(int_subscribe(0,IRQ_REENABLE,0)==0);assert(int_subscribe(1,IRQ_REENABLE,0)==1);assert(int_subscribe(12,IRQ_REENABLE,0)==2);assert(int_unsubscribe(1)==0);assert(int_subscribe(1,IRQ_REENABLE,0)==1);assert(int_unsubscribe(0)==0&&int_unsubscribe(1)==0&&int_unsubscribe(2)==0);assert(sets==4&&enables==4&&disables==4&&removes==4);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-DLCOM_COMPAT_LEGACY_LM_STATUS=1','-O2','-Wno-implicit-function-declaration'])




    def test_huetris_entry_uses_the_declared_minix_startup_contract(self):
        self.compile_and_run(COMMUNITY/'edualm--LCOM-Huetris/code',['main.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
int huetris_entry(int,char**);static int phase;
void sef_startup(void){assert(phase++==0);}void scene_init(void){assert(phase++==1);}
int main(void){char*args[]={"./huetris",NULL};assert(huetris_entry(1,args)==0&&phase==2);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Dmain=huetris_entry','-Werror=implicit-function-declaration'])

    def test_age_of_war_statistics_records_stop_at_ten_within_allocated_buffers(self):
     text=(COMMUNITY/'diogotorres97--LCOM-FEUP/Age Of War/src'/'menu.c').read_text();body=text[text.index('Statistics* InitStatistics()'):text.index('void updateStatistics(')]+text[text.index('void deleteStatistics('):text.index('//***************************************Credits Menu')]
     with tempfile.TemporaryDirectory()as temp:
      out=Path(temp);(out/'statistics.c').write_text('#include "menu.h"\n'+body)
      self.compile_and_run(out,['statistics.c'],r'''#define LCOM_COMPAT_LEGACY 1
   #include <minix/drivers.h>
   #include "menu.h"
   #include <assert.h>
   #undef fopen
   static char path[]="/tmp/age-statistics-XXXXXX";static void*pointers[64];static size_t sizes[64];static unsigned allocated;static Font font;static Bitmap image;static char pixels[1];
   void*checked_malloc(size_t n){assert(allocated<64);void*p=malloc(n+32);assert(p);memset(p,0,n+32);memset((char*)p+n,0xa5,32);pointers[allocated]=p;sizes[allocated++]=n;return p;}
   void*checked_calloc(size_t n,size_t s){return checked_malloc(n*s);}
   void checked_free(void*p){if(!p)return;for(unsigned i=0;i<allocated;i++)if(pointers[i]==p){pointers[i]=NULL;free(p);return;}assert(!"free of unowned statistics allocation");}
   char*checked_fgets(char*p,int n,FILE*f){assert(f);for(unsigned i=0;i<allocated;i++)if(pointers[i]==p){assert(n>0&&(size_t)n<=sizes[i]&&"fgets bound must fit its actual allocated record buffer");return fgets(p,n,f);}assert(!"record destination must point into an owned allocation");return NULL;}
   FILE*lcom_compat_fopen(const char*p,const char*mode){return fopen(path,mode);}
   Font*InitFont(void){return &font;}Bitmap*loadBitmap(const char*p){return &image;}const char*getImagePath(const char*p){return p;}void*initDoubleBuffer(void){return checked_malloc(1);}Font*deleteFont(Font*f){return NULL;}void deleteBitmap(Bitmap*b){}
   int main(void){int fd=mkstemp(path);assert(fd>=0);FILE*f=fdopen(fd,"w");for(int i=0;i<15;i++)fprintf(f,"PLAYER|%d\n",i);fclose(f);Statistics*s=InitStatistics();assert(s&&s->lines);unsigned count=0;while(count<11&&s->lines[count])count++;assert(count==10&&"ten reserved records require a separate in-bounds NULL sentinel");for(unsigned i=0;i<allocated;i++)if(pointers[i])for(unsigned j=0;j<32;j++)assert(((unsigned char*)pointers[i])[sizes[i]+j]==0xa5);deleteStatistics(s);unlink(path);s=InitStatistics();assert(s&&s->lines[0]==NULL);deleteStatistics(s);for(unsigned i=0;i<allocated;i++)assert(pointers[i]==NULL&&"statistics cleanup must release its own record list, records and framebuffer");return 0;}
   ''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Dmalloc=checked_malloc','-Dcalloc=checked_calloc','-Dfree=checked_free','-Dfgets=checked_fgets'],include_dirs=[COMMUNITY/'diogotorres97--LCOM-FEUP/Age Of War/src'])
    def test_age_of_war_statistics_terminate_names_and_scores_at_the_copied_length(self):
     text=(COMMUNITY/'diogotorres97--LCOM-FEUP/Age Of War/src'/'menu.c').read_text();body=text[text.index('void drawStatistics('):text.index('void deleteStatistics(')]
     with tempfile.TemporaryDirectory()as temp:
      out=Path(temp);(out/'draw.c').write_text('#include "menu.h"\n'+body)
      self.compile_and_run(out,['draw.c'],r'''#define LCOM_COMPAT_LEGACY 1
   #include <minix/drivers.h>
   #include "menu.h"
   #include <assert.h>
   static unsigned texts;static const char*expected[]={"RUI","1904","DIOGO","36"};
   void drawBitmap(Bitmap*b,int x,int y,Alignment a,char*p){}
   void drawText(Font*f,char text[],int x,int y,char*p){assert(texts<4&&!strcmp(text,expected[texts])&&"original score names/numbers must not include an uninitialized trailing character");texts++;}
   int main(void){char a[]="RUI|1904\n",b[]="DIOGO|36\n";char*lines[]={a,b,NULL};Statistics s={.lines=lines};drawStatistics(&s);assert(texts==4&&s.flag==1&&s.counter==2);return 0;}
   ''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-ftrivial-auto-var-init=pattern'],include_dirs=[COMMUNITY/'diogotorres97--LCOM-FEUP/Age Of War/src'])

    def test_pongx_subscriptions_keep_authored_notify_bits_and_cleanup_opaque_handles(self):
     self.compile_and_run((COMMUNITY/'BeeMargarida--FEUP-LCOM/proj/src'),['handler.c'],r'''#define LCOM_COMPAT_LEGACY 1
   #include <minix/drivers.h>
   #include "handler.h"
   #include <assert.h>
   static unsigned calls,enables,disables,removed;static int failure;static int ids[]={0,1,12,2},irqs[]={0,1,12,8};
   int sys_irqsetpolicy(int irq,int policy,int*h){if(failure)return 5;assert(calls<4&&*h==ids[calls]&&irq==irqs[calls]);*h=10+calls++;return 0;}
   int sys_irqenable(int*h){assert(*h>=10&&*h<=13);enables++;return 0;}
   int sys_irqdisable(int*h){assert(*h==(disables++?12:10));return 0;}
   int sys_irqrmpolicy(int*h){assert(removed<4&&*h==10+(int)removed++);return 0;}
   int main(void){assert(timer_subscribe_int()==BIT(0));assert(kbd_subscribe_int()==BIT(1));assert(mouse_subscribe_int()==BIT(12));assert(rtc_subscribe_int()==BIT(2));assert(timer_unsubscribe_int()==0&&kbd_unsubscribe_int()==0&&mouse_unsubscribe_int()==0&&rtc_unsubscribe_int()==0);assert(calls==4&&removed==4&&disables==2);failure=1;assert(timer_subscribe_int()==-1);return 0;}
   ''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmain=pongx_entry','-O2','-Wno-implicit-function-declaration'])
    def test_pongx_menu_updates_cursor_after_each_completed_packet(self):
     text=((COMMUNITY/'BeeMargarida--FEUP-LCOM/proj/src')/'handler.c').read_text(encoding='latin1');body=text[text.index('int menu(){'):text.index('void reset_game(')]
     with tempfile.TemporaryDirectory()as tmp:
      out=Path(tmp);(out/'menu.c').write_text('#include "handler.h"\nunsigned int hook_id_timer=0,hook_id_kbd=1,m_hook_id=12;unsigned char packets[3];\n'+body)
      self.compile_and_run(out,['menu.c'],r'''#define LCOM_COMPAT_LEGACY 1
   #include <minix/drivers.h>
   #include "handler.h"
   #include <assert.h>
   int menu(void);static int received,updates;
   char*read_xpm(char*map[],int*w,int*h){*w=*h=10;return calloc(1,100);}
   int timer_subscribe_int(void){return 1;}int kbd_subscribe_int(void){return 2;}int mouse_subscribe_int(void){return 4096;}
   int timer_unsubscribe_int(void){return 0;}int kbd_unsubscribe_int(void){return 0;}int mouse_unsubscribe_int(void){return 0;}
   void clear_screen(void){}int vg_exit(void){return 0;}void menu_show(Menu*m){}void actVideoMem(void){}void mouse_set_stream(void){}
   void clear_cursor(Cursor*c){}void updateCursor(Cursor*c,unsigned char*p){updates++;}void checkCursor(Menu*m){}
   void clear_footer(Footer*f){}void footer_mov(Footer*f){}void paint_footer(Footer*f){}
   int mouse_handler(unsigned char*p){return 1;}int kbd_int_handler(void){return BC_ESC;}
   int game_handler(void){assert(!"play was not selected");return 0;}int mouse_disable_stream(void){return 1;}
   int (sys_inb)(int port,uint32_t*v){*v=0;return 0;}int lcom_compat_sys_inb_ulong(int port,unsigned long*v){*v=0;return 0;}int sys_outb(int port,uint32_t value){return 0;}int tickdelay(clock_t t){return 0;}uint32_t micros_to_ticks(uint32_t u){return 0;}
   int driver_receive(endpoint_t e,message*m,int*s){m->m_source=HARDWARE;*s=NOTIFY;if(received++==0)m->NOTIFY_ARG=BIT(12);else{assert(updates==1&&"one complete packet must move the visible menu cursor without waiting for two more packets");m->NOTIFY_ARG=BIT(1);}return 0;}
   int main(void){assert(menu()==1);assert(updates==1);return 0;}
   ''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Wno-implicit-function-declaration'],include_dirs=[(COMMUNITY/'BeeMargarida--FEUP-LCOM/proj/src')])
    def test_pongx_mouse_commands_poll_status_then_consume_exact_delayed_ack(self):
     self.compile_and_run((COMMUNITY/'BeeMargarida--FEUP-LCOM/proj/src'),['mouse.c'],r'''#define LCOM_COMPAT_LEGACY 1
   #include <minix/drivers.h>
   #include "mouse.h"
   #include <assert.h>
   static unsigned commands,writes,acks,delays,polls;static int addressed,pending;static unsigned expected[]={0xea,0xf4,0xf5};
   int sys_outb(int port,uint32_t value){writes++;if(port==0x64){assert(value==0xd4&&!pending);addressed=1;}else{assert(port==0x60&&addressed&&commands<3&&value==expected[commands++]);addressed=0;pending=1;polls=2;}return 0;}
   int lcom_compat_sys_inb_ulong(int port,unsigned long*v){if(port==0x64){if(pending&&polls)polls--;*v=pending&&!polls?0x21:0;return 0;}assert(port==0x60&&pending&&!polls&&"a reply must be read only after output-buffer-ready, once per command");*v=0xfa;pending=0;acks++;return 0;}
   int tickdelay(clock_t ticks){delays++;return 0;}uint32_t micros_to_ticks(uint32_t us){return 1;}
   int main(void){mouse_set_stream();assert(commands==2&&acks==2&&!pending);assert(mouse_disable_stream()==1);assert(commands==3&&acks==3&&writes==6&&delays>=3);return 0;}
   ''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])

    def test_pongx_rtc_pointer_result_checks_update_status_and_restores_interrupts_on_io_error(self):
     text=((COMMUNITY/'BeeMargarida--FEUP-LCOM/proj/src')/'rtc.c').read_text(encoding='latin1');body=text[text.index('Date *getDateOnly(){'):text.index('void rtc_set_alarm(){')]
     with tempfile.TemporaryDirectory()as tmp:
      out=Path(tmp);(out/'date.c').write_text('#include "graphics.h"\nstatic unsigned char bcd_to_bin(unsigned char x){return (x>>4)*10+(x&15); }\n'+body)
      self.compile_and_run(out,['date.c'],r'''#define LCOM_COMPAT_LEGACY 1
   #include <minix/drivers.h>
   #include "rtc.h"
   #include <assert.h>
   static unsigned reg,disabled,enabled,checks,reads,shown;static int fail;
   void show_date(Date*d){assert(d->day==17&&d->month==10&&d->year==26);shown++;}void show_date_and_hours(Date*d){show_date(d);assert(d->hours==13&&d->minutes==45&&d->seconds==59);}
   void rtc_disable_int(void){disabled++;}void rtc_enable_int(void){enabled++;}
   int sys_outb(int port,uint32_t value){assert(port==0x70);if(fail)return 5;reg=value;return 0;}
   int lcom_compat_sys_inb_ulong(int port,unsigned long*v){assert(port==0x71);if(reg==0x0a){*v=checks++==0?0x80:0;return 0;}assert(checks&&"read update-in-progress status before reading the date");reads++;switch(reg){case 7:*v=0x17;break;case 8:*v=0x10;break;case 9:*v=0x26;break;case 4:*v=0x13;break;case 2:*v=0x45;break;case 0:*v=0x59;break;default:assert(0);}return 0;}
   int main(void){fail=1;assert(getDateOnly()==NULL);assert(disabled==1&&enabled==1);fail=0;Date*d=getDateOnly();assert(d&&d->day==17&&d->month==10&&d->year==26&&d->hours==13&&d->minutes==45&&d->seconds==59);assert(checks>=7&&reads==6&&disabled==2&&enabled==2);free(d);fail=1;assert(getDate()==1&&getDateandHours()==1);assert(disabled==4&&enabled==4);fail=0;checks=0;reads=0;assert(getDate()==0&&reads==3&&checks>=4);checks=0;reads=0;assert(getDateandHours()==0&&reads==6&&checks>=7&&shown==2);assert(disabled==6&&enabled==6);return 0;}
   ''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-ftrivial-auto-var-init=pattern','-Werror=return-type'],include_dirs=[(COMMUNITY/'BeeMargarida--FEUP-LCOM/proj/src')])

    def test_shoot_monsters_aim_decodes_the_full_signed_nine_bit_packet(self):
     self.compile_and_run((COMMUNITY/'nunottlopes--feup-lcom/proj/src'),['mouse.c'],r'''#define LCOM_COMPAT_LEGACY 1
   #include <minix/drivers.h>
   #include "mouse.h"
   #include <assert.h>
   extern int mouse_counter;extern mouse rato;static unsigned bytes[3],index_;
   int lcom_compat_sys_inb_ulong(int port,unsigned long*v){if(port==0x64)*v=0x21;else{assert(port==0x60&&index_<3);*v=bytes[index_++];}return 0;}
   int main(void){int deltas[]={-255,-200,-129,-128,-1,0,1,127,128,255};for(unsigned axis=0;axis<2;axis++)for(unsigned i=0;i<10;i++){int d=deltas[i],x=512,y=384;mouse_counter=index_=0;bytes[0]=8|(d<0?(axis?32:16):0);bytes[1]=axis?0:(unsigned char)d;bytes[2]=axis?(unsigned char)d:0;assert(mouse_game_handler(&x,&y)==0);assert(mouse_game_handler(&x,&y)==0);assert(mouse_game_handler(&x,&y)==2);assert(x==512+(axis?0:d)&&y==384-(axis?d:0)&&"full signed9bit mouse displacement must preserve direction and distance");}int x=512,y=384;mouse_counter=index_=0;bytes[0]=9;bytes[1]=bytes[2]=0;mouse_game_handler(&x,&y);mouse_game_handler(&x,&y);assert(mouse_game_handler(&x,&y)==2&&x==512&&y==384&&rato.LB==1);return 0;}
   ''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])
    def test_shoot_monsters_entry_draws_the_original_menu_before_receiving_input(self):
     text=((COMMUNITY/'nunottlopes--feup-lcom/proj/src')/'game.c').read_text();body=text[text.index('int open_game(){'):text.index('int keyboard_handler(')]
     with tempfile.TemporaryDirectory()as tmp:
      out=Path(tmp);(out/'entry.c').write_text('#include <minix/drivers.h>\n#include "game.h"\n#include "sprite.h"\n#include "video_gr.h"\n#include "kbd.h"\n#include "timer.h"\n#include "mouse.h"\nint sair,monster_current_x,monster_current_y;\n'+body)
      self.compile_and_run(out,['entry.c'],r'''#define LCOM_COMPAT_LEGACY 1
   #include <minix/drivers.h>
   #include "game.h"
   #include <assert.h>
   extern int sair;static unsigned created,draws,closed;
   void*vg_open(unsigned short mode){assert(mode==0x105);return &created;}int vg_exit(void){closed++;return 0;}
   int kbd_subscribe_int(void){return 1;}int timer_subscribe_int(void){return 2;}int mouse_subscribe_int(void){return 4;}
   int kbd_unsubscribe_int(void){return 0;}int timer_unsubscribe_int(void){return 0;}int mouse_unsubscribe_int(void){return 0;}
   int start_mouse(void){return 0;}int end_mouse(void){return 0;}void createSprites(void){created++;}void destroySprites(void){}
   unsigned long kbd_test_scan_C(void){return 0;}int keyboard_handler(unsigned long key){assert(created==1&&key==0);draws++;return 0;}int timer_handler(void){return 0;}int mouse_handler(void){return 0;}
   int driver_receive(endpoint_t from,message*m,int*s){assert(draws==1&&"fresh launch must draw the original menu before the first input notification");m->m_source=HARDWARE;m->NOTIFY_ARG=0;*s=NOTIFY;sair=-1;return 0;}
   int main(void){assert(open_game()==0&&draws==1&&closed==1);return 0;}
   ''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'],include_dirs=[(COMMUNITY/'nunottlopes--feup-lcom/proj/src')])
    def test_shoot_monsters_missing_score_file_and_failed_write_are_safe(self):
     self.compile_and_run((COMMUNITY/'nunottlopes--feup-lcom/proj/src'),['highscores.c'],r'''#define LCOM_COMPAT_LEGACY 1
   #include <minix/drivers.h>
   #include "highscores.h"
   #include <assert.h>
   #undef fopen
   static char path[]="/tmp/shoot-scores-missing-XXXXXX";extern unsigned long score1;
   FILE*lcom_compat_fopen(const char*p,const char*mode){assert(!strcmp(p,"/home/lcom/lcom//proj/src/highscores.txt"));return fopen(path,mode);}
   int main(void){int fd=mkstemp(path);assert(fd>=0);close(fd);unlink(path);score1=123;assert(readHighscoresFile()!=0&&score1==123);strcpy(path,"/nonexistent/shoot/scores");assert(writeHighscoresFile()!=0);return 0;}
   ''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])
    def test_shoot_monsters_score_text_roundtrip_matches_declared_unsigned_long_fields(self):
     self.compile_and_run((COMMUNITY/'nunottlopes--feup-lcom/proj/src'),['highscores.c'],r'''#define LCOM_COMPAT_LEGACY 1
   #include <minix/drivers.h>
   #include "highscores.h"
   #include <assert.h>
   #undef fopen
   static char path[]="/tmp/shoot-scores-roundtrip-XXXXXX";extern unsigned long score1,hours1,minutes1,day1,month1,year1,score2,hours2,minutes2,day2,month2,year2,score3,hours3,minutes3,day3,month3,year3;
   FILE*lcom_compat_fopen(const char*p,const char*mode){return fopen(path,mode);}
   int main(void){unsigned long*v[]={&score1,&hours1,&minutes1,&day1,&month1,&year1,&score2,&hours2,&minutes2,&day2,&month2,&year2,&score3,&hours3,&minutes3,&day3,&month3,&year3};int fd=mkstemp(path);assert(fd>=0);FILE*f=fdopen(fd,"w");for(unsigned i=0;i<18;i++){*v[i]=ULONG_MAX;fprintf(f,"%u\n",i*13+7);}fclose(f);assert(readHighscoresFile()==0);for(unsigned i=0;i<18;i++)assert(*v[i]==i*13+7&&"fscanf must write the whole unsigned-long field, including on64bit hosts");assert(writeHighscoresFile()==0);f=fopen(path,"r");for(unsigned i=0;i<18;i++){unsigned long n=0;assert(fscanf(f,"%lu",&n)==1&&n==i*13+7);}fclose(f);f=fopen(path,"w");fputs("99\ninvalid\n",f);fclose(f);assert(readHighscoresFile()!=0&&score1==7);unlink(path);return 0;}
   ''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])

    def test_pongx_match_uses_timer_notify_bit_for_ball_paddle_and_seconds(self):
     source=COMMUNITY/'BeeMargarida--FEUP-LCOM/proj/src';text=(source/'handler.c').read_text(encoding='latin1');body=text[text.index('int game(Bar *b1, Bar *b2, Ball *a){'):text.index('void winner(Bar *b1, Bar *b2){')]
     with tempfile.TemporaryDirectory() as tmp:
      out=Path(tmp);(out/'match.c').write_text('#include "handler.h"\nunsigned int hook_id_timer=0,hook_id_kbd=1,hook_id_rtc=2;int count_int=0;\n'+body)
      self.compile_and_run(out,['match.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "handler.h"
#include <assert.h>
static int receives,keyboard_reads,ball_moves,paddle_moves,presented,seconds,removed;
int __attribute__((aligned(16))) timer_subscribe_int(void){return BIT(0);}int kbd_subscribe_int(void){return BIT(1);}int rtc_subscribe_int(void){return BIT(2);}
int timer_unsubscribe_int(void){removed++;return 0;}int kbd_unsubscribe_int(void){removed++;return 0;}int rtc_unsubscribe_int(void){removed++;return 0;}
void rtc_set_alarm(void){}int rtc_handler(void){assert(!"no RTC event was sent");return 0;}
int kbd_int_handler(void){return keyboard_reads++?BC_ESC:KEY_W;}
int driver_receive(endpoint_t e,message*m,int*s){assert(receives<62);m->m_source=HARDWARE;*s=NOTIFY;m->NOTIFY_ARG=(receives==0||receives==61)?BIT(1):BIT(0);receives++;return 0;}
void clear_bar(Bar*b1,Bar*b2,int side){}void clear_ball(Ball*a){}void reset_game(Bar*b1,Bar*b2,Ball*a){assert(!"no score reset was requested");}
void ball_movement(Bar*b1,Bar*b2,Ball*a){a->x+=a->speed;ball_moves++;}void show_score(Bar*b1,Bar*b2){}void paint_ball(Ball*a){}void paint_bar(Bar*b){}
int movement(Bar*b){assert(b->scancode==KEY_W);b->ya-=b->speed;paddle_moves++;return 0;}void actVideoMem(void){presented++;}void show_time(int time){assert(time==59);seconds++;}
void winner(Bar*b1,Bar*b2){assert(ball_moves==60&&paddle_moves==60&&presented==60&&seconds==1&&removed==3);assert(b1->ya==34);}
int main(void){Bar left={0},right={0};left.ya=334;left.speed=5;Ball*a=calloc(1,sizeof(*a));a->speed=6;game(&left,&right,a);assert(receives==62);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Wno-implicit-function-declaration'],include_dirs=[source])

    def test_tetris_keyboard_subscription_overwrites_the_output_notification_mask(self):
        self.compile_and_run(COMMUNITY/'goncaloacteixeira--feup-lcom/proj/src',['keyboard.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
int keyboard_subscribe_int(uint32_t*mask);
int sys_irqsetpolicy(int irq,int policy,int*handle){assert(irq==1&&*handle==1);*handle=17;return 0;}
int main(void){uint32_t mask=UINT32_MAX;assert(keyboard_subscribe_int(&mask)==0);assert(mask==BIT(1)&&"subscription output must not retain prior caller bits");return 0;}
''',source_flags=['-O2'])

    def test_tetris_scancode_consumers_match_the_actual_keyboard_byte_storage(self):
        project=COMMUNITY/'goncaloacteixeira--feup-lcom/proj/src'
        for consumer in ['menus.c','tetris.c']:
            with self.subTest(consumer=consumer):
                self.compile_and_run(project,[],'#include "keyboard.c"\n#include "'+consumer+'"\nint main(void){return 0;}\n',source_flags=['-O2'])

    def test_shoot_monsters_packet_consumer_preserves_axes_across_empty_and_foreign_notifications(self):
     self.compile_and_run((COMMUNITY/'nunottlopes--feup-lcom/proj/src'),['mouse.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include "mouse.h"
#include <assert.h>
extern int mouse_counter;extern mouse rato;
static unsigned status=0x21,data,reads;static int fail_status;
int lcom_compat_sys_inb_ulong(int port,unsigned long*v){if(port==0x64){if(fail_status)return 5;*v=status;return 0;}assert(port==0x60&&(status&0x21)==0x21);*v=data;reads++;return 0;}
static int poll(int*x,int*y,unsigned flags,unsigned byte){status=flags;data=byte;return mouse_game_handler(x,y);}
static void packet(int*x,int*y,int dx,int dy){unsigned b=8|(dx<0?16:0)|(dy<0?32:0);assert(poll(x,y,0x21,b)==0);assert(poll(x,y,0x21,(unsigned char)dx)==0);assert(poll(x,y,0x21,(unsigned char)dy)==2);}
int main(void){int x=512,y=384;mouse_counter=0;for(unsigned i=0;i<3;i++)packet(&x,&y,-126,104);assert(x==134&&y==72&&"ordinary multi-packet deltas must sum exactly on both axes");
 x=512;y=384;mouse_counter=reads=0;assert(poll(&x,&y,0x21,0x38)==0);assert(mouse_counter==1);assert(poll(&x,&y,0,0)==0&&mouse_counter==1&&reads==1&&"empty output buffer must not invent a packet byte");assert(poll(&x,&y,1,0x1e)==0&&mouse_counter==1&&reads==1&&"keyboard bytes must stay in the owning buffer");assert(poll(&x,&y,0x21,(unsigned char)-200)==0);assert(mouse_counter==2);assert(poll(&x,&y,0,0)==0&&mouse_counter==2);assert(poll(&x,&y,0x21,(unsigned char)-128)==2);assert(x==312&&y==512&&reads==3);
 assert(poll(&x,&y,0x21,0x08)==0);assert(poll(&x,&y,0xa1,0xff)!=2&&mouse_counter==0&&"a parity error discards the partial packet");assert(poll(&x,&y,0x21,0)==0&&mouse_counter==0);packet(&x,&y,17,23);assert(x==329&&y==489);fail_status=1;assert(mouse_game_handler(&x,&y)!=2&&x==329&&y==489);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-ftrivial-auto-var-init=pattern'])

    def test_pongx_ball_paint_and_clear_clip_at_the_real_framebuffer_edges(self):
     self.compile_and_run(COMMUNITY/'BeeMargarida--FEUP-LCOM/proj/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <assert.h>
#include "graphics.c"
int main(void){enum{N=1024*768,GUARD=1024*32};unsigned char*allocation=malloc(N+2*GUARD);assert(allocation);memset(allocation,0xa5,N+2*GUARD);buffer=(char*)allocation+GUARD;memset(buffer,0,N);
 Ball b={0};b.side=20;b.x=100;b.y=760;paint_ball(&b);assert((unsigned char)buffer[760*1024+100]==63&&buffer[759*1024+100]==0);for(unsigned i=0;i<GUARD;i++)assert(allocation[GUARD+N+i]==0xa5&&"paint_ball must not write beyond the bottom row, matching actual native crash");clear_ball(&b);assert(buffer[760*1024+100]==0);
 memset(buffer,0,N);b.x=1018;b.y=300;paint_ball(&b);assert((unsigned char)buffer[300*1024+1018]==63&&buffer[301*1024]==0&&"right-edge pixels must not wrap into the next row");b.x=-10;b.y=400;paint_ball(&b);assert((unsigned char)buffer[400*1024]==63);for(unsigned i=0;i<GUARD;i++)assert(allocation[i]==0xa5&&allocation[GUARD+N+i]==0xa5);free(allocation);return 0;}
''')
    def test_pongx_initial_and_reset_ball_vertical_direction_are_defined(self):
     source=COMMUNITY/'BeeMargarida--FEUP-LCOM/proj/src';text=(source/'handler.c').read_text(encoding='latin1');body=text[text.index('void reset_game('):text.index('int game(Bar *b1, Bar *b2, Ball *a){')]
     with tempfile.TemporaryDirectory() as tmp:
      out=Path(tmp);(out/'init.c').write_text('#include "handler.h"\nvoid*pong_pattern_malloc(size_t);\n#define malloc pong_pattern_malloc\n'+body)
      self.compile_and_run(out,['init.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <assert.h>
#include "handler.h"
static unsigned initialized;
void*pong_pattern_malloc(size_t size){void*p=malloc(size);assert(p);memset(p,0xa5,size);return p;}
void game_show(Bar*b1,Bar*b2,Ball*a){}void actVideoMem(void){}
int game(Bar*b1,Bar*b2,Ball*a){assert(a->vert==0&&"fresh ball must have an initialized vertical direction");assert(a->x==502&&a->y==374&&a->side==20&&a->speed==6);a->vert=1;reset_game(b1,b2,a);assert(a->vert==1&&"a scored ball preserves its valid original vertical direction");initialized++;free(a);free(b1);free(b2);return 0;}
int main(void){game_handler();assert(initialized==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1'],include_dirs=[source])

    def test_walls_mouse_constructor_initializes_packet_and_motion_storage(self):
     self.compile_and_run(COMMUNITY/'JPMMaia--lcom/proj/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "boolType.h"
#include <minix/drivers.h>
static void*pattern_malloc(size_t size){void*p=malloc(size);assert(p);memset(p,0xa5,size);return p;}
#define malloc pattern_malloc
#include "MouseClass.c"
#undef malloc
int main(void){MouseClass*m=newMouse();assert(m&&m->count==0&&m->deltaX==0&&m->deltaY==0&&!m->enableMovementReport&&!m->XOverflow&&!m->YOverflow&&"the first IRQ must not index an uninitialized packet count or replay uninitialized movement");for(unsigned i=0;i<3;i++)assert(!m->keys[i]);deleteMouse(m);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])
    def test_walls_input_applies_each_complete_mouse_packet_once(self):
     self.compile_and_run(COMMUNITY/'JPMMaia--lcom/proj/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "MouseClass.c"
#include "ListClass.c"
#include "InputClass.c"
static unsigned index_,bytes[]={0x28,70,216,0x18,226,10};
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){assert(port==0x60&&index_<6);*value=bytes[index_++];return 0;}
int main(void){MouseClass mouse={0};InputClass input={0};input.Mouse=&mouse;input.MouseEvents=newList(sizeof(ms_event));input.MouseCursor=(Coord){30,30};input.x0=input.y0=30;input.screenWidth=963;input.screenHeight=707;
 InputHandleMouseInt(&input);InputHandleMouseInt(&input);assert(input.MouseCursor.x==30&&input.MouseCursor.y==30);assert(InputHandleMouseInt(&input));assert(input.MouseCursor.x==100&&input.MouseCursor.y==70);
 InputHandleMouseInt(&input);assert(input.MouseCursor.x==100&&input.MouseCursor.y==70&&"partial next packet must not replay the previous complete movement");InputHandleMouseInt(&input);assert(input.MouseCursor.x==100&&input.MouseCursor.y==70);assert(InputHandleMouseInt(&input));assert(input.MouseCursor.x==70&&input.MouseCursor.y==60&&ListIsEmpty(input.MouseEvents));deleteList(input.MouseEvents);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])
    def test_walls_menu_initialization_returns_a_defined_success_result(self):
     self.compile_and_run(COMMUNITY/'JPMMaia--lcom/proj/src',['MenuClass.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include "MenuClass.h"
#include <assert.h>
static int success,calls;static unsigned sprite;
SpriteClass*newSprite(void){return (SpriteClass*)&sprite;}
bool SpriteInitializeXPM(SpriteClass*s,char*image[]){assert(s==(SpriteClass*)&sprite);calls++;return success?true:false;}
int main(void){MenuClass*m=newMenu();success=0;assert(MenuInitialize(m,NULL,NULL,NULL,NULL)==false);deleteMenu(m);m=newMenu();success=1;assert(MenuInitialize(m,NULL,NULL,NULL,NULL)==true&&calls==2);deleteMenu(m);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Werror=return-type'])
    def test_walls_rtc_consumer_filters_latched_causes_by_enabled_interrupts(self):
     source=COMMUNITY/'JPMMaia--lcom/proj/src';text=(source/'RTCClass.c').read_text(encoding='latin1');body=text[text.index('rtc_int RTCHandleInt(RTCClass* RTC)'):text.index('/////////////////////////////// PRIVATE',text.index('rtc_int RTCHandleInt(RTCClass* RTC)'))]
     with tempfile.TemporaryDirectory() as tmp:
      out=Path(tmp);(out/'rtc-selection.c').write_text('#include <assert.h>\n#include "RTCClass.h"\nstruct rtc_class{unsigned long config[4];unsigned long date[10];};\nstatic unsigned long flags;static unsigned reads;\nstatic void RTCReadReg(unsigned long addr,unsigned long*value){assert(addr==RTC_REG_C);*value=flags;flags=0;reads++;}\nstatic void RTCSetFlag(RTCClass*r,unsigned long reg,unsigned long flag,bool enable){reads++;}\n'+body+'\nint event(unsigned long enabled,unsigned long pending){RTCClass rtc={0};rtc.config[1]=enabled;flags=pending;reads=0;int result=RTCHandleInt(&rtc);assert(reads==1&&"reading C acknowledges the interrupt exactly once");return result;}\n')
      self.compile_and_run(out,['rtc-selection.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include "RTCClass.h"
#include <assert.h>
int event(unsigned long,unsigned long);
int main(void){assert(event(RTC_B_UIE,0xd0)==rtc_update_ended&&"disabled periodic PF must not hide enabled update-ended UF");assert(event(RTC_B_UIE,0xf0)==rtc_update_ended);assert(event(RTC_B_PIE,0xd0)==rtc_periodic);assert(event(RTC_B_AIE,0xf0)==rtc_alarm);assert(event(RTC_B_UIE,0xc0)==-1);assert(event(0,0xf0)==-1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'],include_dirs=[source])


    def test_falling_words_number_renderer_frees_only_owned_strings_and_keeps_zero_digits(self):
        self.compile_and_run(COMMUNITY/'TiagooGomess--LCOM_1920/proj',['words.c'],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "sprite.h"
#include "words.h"
int x0_word,y0_word;
static Sprite digits[10];
static void *allocation;
void *falling_checked_malloc(size_t bytes){assert(!allocation);allocation=malloc(bytes);return allocation;}
void falling_checked_free(void *pointer){
 if(pointer!=allocation || pointer==NULL){fputs("FAIL: HUD renderer freed a non-owned pointer while drawing zero\n",stderr);exit(93);}
 free(pointer);allocation=NULL;
}
Sprite *sprite_A;
Sprite *sprite_B;
Sprite *sprite_C;
Sprite *sprite_D;
Sprite *sprite_E;
Sprite *sprite_F;
Sprite *sprite_G;
Sprite *sprite_H;
Sprite *sprite_I;
Sprite *sprite_J;
Sprite *sprite_K;
Sprite *sprite_L;
Sprite *sprite_M;
Sprite *sprite_N;
Sprite *sprite_O;
Sprite *sprite_P;
Sprite *sprite_Q;
Sprite *sprite_R;
Sprite *sprite_S;
Sprite *sprite_T;
Sprite *sprite_U;
Sprite *sprite_V;
Sprite *sprite_W;
Sprite *sprite_X;
Sprite *sprite_Y;
Sprite *sprite_Z;
Sprite *sprite_0 = &digits[0];
Sprite *sprite_1 = &digits[1];
Sprite *sprite_2 = &digits[2];
Sprite *sprite_3 = &digits[3];
Sprite *sprite_4 = &digits[4];
Sprite *sprite_5 = &digits[5];
Sprite *sprite_6 = &digits[6];
Sprite *sprite_7 = &digits[7];
Sprite *sprite_8 = &digits[8];
Sprite *sprite_9 = &digits[9];
static char observed[64]; static size_t drawn; static int expected_x,expected_y;
int draw_sprite(Sprite *sp,uint8_t *base) {
 (void)base; assert(sp>=digits && sp<digits+10); assert(sp->x==expected_x && sp->y==expected_y);
 observed[drawn++]=(char)('0'+(sp-digits)); observed[drawn]=0; expected_x+=45; return 0;
}
int draw_sprite_with_color(Sprite *sp,uint32_t color,uint8_t *base){(void)sp;(void)color;(void)base;return 0;}
int main(void){
 const struct{unsigned number;const char *digits;}cases[]={{0,"0"},{1,"1"},{9,"9"},{10,"10"},{60,"60"},{1024,"1024"}};
 for(unsigned repeat=0;repeat<3;repeat++)for(unsigned i=0;i<sizeof(cases)/sizeof(*cases);i++){
  drawn=0;observed[0]=0;expected_x=1150;expected_y=30;
  displayNumber(expected_x,expected_y,cases[i].number,NULL);
  assert(strcmp(observed,cases[i].digits)==0);
 }
 puts("PASS: zero and nonzero HUD digits render correctly across repeated frames with valid ownership");
}
''',source_flags=['-O2','-Dmalloc=falling_checked_malloc','-Dfree=falling_checked_free'])

    def test_falling_words_random_selection_reads_live_first_and_last_dictionary_words(self):
        project=COMMUNITY/'TiagooGomess--LCOM_1920/proj'
        self.compile_and_run(project,[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#undef fopen
static unsigned selection,released;
static FILE*falling_prompt_open(const char*path,const char*mode){assert(!strcmp(path,"/home/lcom/labs/proj/words.txt"));return fopen("$DICTIONARY$",mode);}
static int falling_prompt_random(void){return (int)selection;}
static void falling_prompt_free(void*p){released++;free(p);}
#define fopen falling_prompt_open
#define rand falling_prompt_random
#define free falling_prompt_free
#include "words.c"
#undef free
#undef rand
#undef fopen
int main(void){
 const struct{unsigned selection;const char*word;}cases[]={{0,"a\n"},{2994,"zone\n"}};
 for(unsigned i=0;i<sizeof(cases)/sizeof(*cases);i++){
  selection=cases[i].selection;released=0;char*word=get_word();
  if(released){fputs("FAIL: random index zero returned an already-freed dictionary buffer\n",stderr);return 92;}
  assert(word&&word==current_word&&word==current_word_complete);
  assert(!strcmp(word,cases[i].word)&&"first and last random indices choose real original dictionary lines");free(word);
 }
 return 0;
}
'''.replace('$DICTIONARY$',str(project/'words.txt')),source_flags=['-O2'])


    def test_humanoid_cursor_handlers_accumulate_complete_packets_until_the_frame(self):
        import re
        project=COMMUNITY/'catarinaopires--lcom_20_21/proj/src'
        text=(project/'proj.c').read_text()
        handlers=[m.group(0) for m in re.finditer(r'mouse_process_packet\(&mouse, &pMouse\);\s*[^;]+;',text)]
        self.assertTrue(handlers,'Original cursor packet handlers must be exercised')
        generated='#include <lcom/lcf.h>\n#include "video/extImages.h"\n#include "kbc/mouse.h"\nSprite *cursor;\n'
        for index,body in enumerate(handlers):
            generated+='void cursor_packet_'+str(index)+'(mouse_packet_raw input){mouse_packet_raw mouse=input;mouse_packet_processed pMouse;'+body+'}\n'
        generated+='void (*const cursor_handlers[])(mouse_packet_raw)={'+','.join('cursor_packet_'+str(i) for i in range(len(handlers)))+'};\n'
        harness=r'''#include <lcom/lcf.h>
#include <assert.h>
#include "video/extImages.h"
#include "kbc/mouse.h"
extern Sprite*cursor;
extern void(*const cursor_handlers[])(mouse_packet_raw);
static mouse_packet_raw packet(int dx,int dy,unsigned buttons){mouse_packet_raw p={0};p.bytes[0]=8|buttons|(dx<0?16:0)|(dy<0?32:0);p.bytes[1]=(uint8_t)dx;p.bytes[2]=(uint8_t)dy;return p;}
int main(void){
 video_instance instance={0};instance.mode_info.XResolution=1152;instance.mode_info.YResolution=864;
 for(unsigned handler=0;handler<$COUNT$;handler++){
  Sprite actual={0};cursor=&actual;cursor->drawing.x=576;cursor->drawing.y=432;
  cursor_handlers[handler](packet(25,-30,0));cursor_handlers[handler](packet(40,10,0));
  /* A stationary button packet before redraw must not erase pending travel. */
  cursor_handlers[handler](packet(0,0,1));
  move_sprite(cursor,1152,864,1,&instance);change_speed(cursor,0,0);
  if(cursor->drawing.x!=641||cursor->drawing.y!=452){fputs("FAIL: complete mouse packet travel lost before timer frame\n",stderr);return 94;}
  move_sprite(cursor,1152,864,1,&instance);assert(cursor->drawing.x==641&&cursor->drawing.y==452);
  cursor_handlers[handler](packet(-50,35,0));cursor_handlers[handler](packet(-15,-5,0));
  move_sprite(cursor,1152,864,1,&instance);change_speed(cursor,0,0);assert(cursor->drawing.x==576&&cursor->drawing.y==422);
 }
 Sprite ordinary={0};change_speed(&ordinary,20,30);change_speed(&ordinary,4,-7);
 assert(ordinary.xspeed==4&&ordinary.yspeed==-7&&"global velocity setter retains replacement semantics");return 0;
}
'''.replace('$COUNT$',str(len(handlers)))
        with tempfile.TemporaryDirectory() as temporary:
            source=Path(temporary);(source/'cursor_handlers.c').write_text(generated)
            self.compile_and_run(source,['cursor_handlers.c',project/'video/extImages.c',project/'kbc/mouse.c'],harness,source_flags=['-O2'],include_dirs=[project])

    def test_desktop_defender_animation_cleanup_releases_owned_objects_without_reusing_them(self):
     project=COMMUNITY/'anaines14--Project_LCOM/src'
     text=(project/'mainLoop.c').read_text(encoding='latin1');blocks=[]
     import re
     for match in re.finditer(r'DestroyAnimated\((enemies\[i\]|Missile|mouse)\);',text):
      name=match.group(1);end=text.index(name+' = NULL;',match.end())+len(name+' = NULL;')
      blocks.append(text[match.start():end])
     self.assertEqual(len(blocks),7)
     with tempfile.TemporaryDirectory() as tmp:
      out=Path(tmp);body='#include "animations.h"\n'
      for i,block in enumerate(blocks):
       body+='void clean'+str(i)+'(Animated**enemies,Animated*Missile,Animated*mouse){int i=0;'+block+'}\n'
      (out/'owner.c').write_text(body)
      self.compile_and_run(out,['owner.c',str(project/'animations.c')],r'''#include "animations.h"
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
static Animated*object;static size_t page;static unsigned released,sprites,arrays;
void tracked_free(void*p){if(p==object){assert(!released++);assert(mprotect(p,page,PROT_NONE)==0);return;}arrays++;free(p);}
void destroy_sprite(Sprite*s){assert(s);sprites++;free(s);}
'''+''.join('void clean'+str(i)+'(Animated**,Animated*,Animated*);\n' for i in range(7))+r'''
int main(void){page=sysconf(_SC_PAGESIZE);void(*clean[])(Animated**,Animated*,Animated*)={clean0,clean1,clean2,clean3,clean4,clean5,clean6};for(unsigned i=0;i<7;i++){released=sprites=arrays=0;object=mmap(NULL,page,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);assert(object!=MAP_FAILED);object->num_fig=1;object->sprites=malloc(sizeof(Sprite*));object->sprites[0]=calloc(1,sizeof(Sprite));object->current_sprite=object->sprites[0];Animated*enemies[]={object};clean[i](enemies,object,object);assert(released==1&&sprites==1&&arrays==1);if(i<4||i==6)assert(enemies[0]==NULL);munmap(object,page);}return 0;}
''',source_flags=['-O2','-Dfree=tracked_free'],include_dirs=[project])

    def test_desktop_defender_applies_mouse_motion_only_after_a_complete_packet(self):
     project=COMMUNITY/'anaines14--Project_LCOM/src';text=(project/'mainLoop.c').read_text(encoding='latin1');start=text.index('          if (msg.m_notify.interrupts & irq_mouse)');brace=text.index('{',start);end=brace+1;depth=1
     while depth:
      if text[end]=='{':depth+=1
      elif text[end]=='}':depth-=1
      end+=1
     block=text[start:end]
     self.compile_and_run(project,['mouse.c'],r'''#include <lcom/lcf.h>
#include "mouse.h"
#include <assert.h>
extern struct packet pp;extern int read_result;extern uint8_t byteNumMouse;
static unsigned reads;static uint8_t packets[]={0x08,12,6,0x38,251,253};
int(util_sys_inb)(int port,uint8_t*v){if(port==0x64){*v=0x21;return 0;}assert(port==0x60&&reads<6);*v=packets[reads++];return 0;}
int main(void){float true_x_mouse=800,true_y_mouse=600;message msg={0};int irq_mouse=BIT(12);msg.m_notify.interrupts=irq_mouse;
for(unsigned n=0;n<6;n++){
'''+block+r'''
if(n<2)assert(true_x_mouse==800&&true_y_mouse==600);if(n>=2&&n<5)assert(true_x_mouse==812&&true_y_mouse==594);}
assert(reads==6&&byteNumMouse==0&&true_x_mouse==807&&true_y_mouse==597);return 0;}
''',source_flags=['-O2'])

    def test_desktop_defender_includes_the_original_case_sensitive_clock_asset(self):
     self.compile_and_run(COMMUNITY/'anaines14--Project_LCOM/src',['mainLoop.c'],r'''int main(void){return 0;}''',source_flags=['-O2','-Werror=nonportable-include-path'],compiler_override=['clang'])

    def test_pullball_font_treats_the_font_name_as_literal_text(self):
     self.compile_and_run(COMMUNITY/'mbcsm--FEUP_LCOM/proj/code',['font.c'],r'''#include <lcom/lcf.h>
#include "font.h"
#include <assert.h>
#include <unistd.h>
uint32_t xpm_transparency_color(enum xpm_image_type type){return 0;}
uint8_t*xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*image){static uint16_t pixel;*image=(xpm_image_t){.width=1,.height=1};return (uint8_t*)&pixel;}
void draw_xpm(int x,int y,uint16_t*image,xpm_image_t info,uint16_t color){}
int main(void){FILE*f=tmpfile();assert(f);int fd=dup(STDOUT_FILENO);assert(fd>=0);fflush(stdout);assert(dup2(fileno(f),STDOUT_FILENO)>=0);printstring("",0,"w%literal",0,0);fflush(stdout);assert(dup2(fd,STDOUT_FILENO)>=0);close(fd);rewind(f);char text[32]={0};assert(fread(text,1,sizeof text-1,f)==9&&!strcmp(text,"w%literal"));fclose(f);return 0;}
''',source_flags=['-O2','-Wformat','-Werror=format-security'])

    def test_pullball_cursor_restores_its_local_backing_pixels_and_stays_inside_the_frame(self):
     from unittest.mock import patch
     import os
     with patch.dict(os.environ,{'CC':os.environ.get('CC','cc')+' -fsanitize=bounds -fno-sanitize-recover=bounds'}):
      self.compile_and_run(COMMUNITY/'mbcsm--FEUP_LCOM/proj/code',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <signal.h>
#include "cursor.c"
static uint16_t frame[1280*1024];
int get_h_res(void){return 1280;}int get_v_res(void){return 1024;}void*get_video_mem(void){return frame;}
void draw_xpm(int x,int y,uint16_t*image,xpm_image_t info,uint16_t color){}
uint8_t*xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*image){return NULL;}
uint32_t xpm_transparency_color(enum xpm_image_type type){return 0;}
int main(void){alarm(3);imgC.width=24;imgC.height=36;x_Cursor=500;y_Cursor=500;for(unsigned i=0;i<1280*1024;i++)frame[i]=(uint16_t)i;underMouse();for(int y=500;y<536;y++)for(int x=500;x<524;x++)frame[y*1280+x]=0;clearMouse();for(int y=500;y<536;y++)for(int x=500;x<524;x++)assert(frame[y*1280+x]==(uint16_t)(y*1280+x));int x=500,y=500;struct packet p={.delta_x=2000,.delta_y=-2000};updatePosition(&p,&x,&y);assert(x==1256&&y==988&&"the full cursor rectangle remains inside the screen");underMouse();clearMouse();return 0;}
''',source_flags=['-O2'])

    def test_pullball_rtc_date_strings_fit_the_requested_allocation(self):
     self.compile_and_run(COMMUNITY/'mbcsm--FEUP_LCOM/proj/code',['rtc.c'],r'''#include <lcom/lcf.h>
#include "rtc.h"
extern Date_and_Time dt;
#include <assert.h>
static unsigned char memory[128];static size_t requested;
void*checked_malloc(size_t n){assert(n<96);requested=n;memset(memory,0x5a,sizeof memory);return memory;}
int main(void){dt.year=26;dt.month=10;dt.day=17;dt.hour=12;dt.minute=34;dt.second=56;for(int print=0;print<2;print++){char*s=get_current_date_and_time(print);assert(s==(char*)memory);assert(strlen(s)<requested);for(size_t i=requested;i<requested+16;i++)assert(memory[i]==0x5a);}return 0;}
''',source_flags=['-O2','-Dmalloc=checked_malloc'])

    def test_pullball_score_reader_handles_first_run_empty_and_saved_records(self):
     self.compile_and_run(COMMUNITY/'mbcsm--FEUP_LCOM/proj/code',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <unistd.h>
#undef fopen
static FILE*local_open(const char*p,const char*m){assert(!strcmp(p,"/z.txt")||!strcmp(p,"bestScores.txt"));return fopen("scores",m);}
#define fopen local_open
#pragma clang diagnostic ignored "-Wformat-security"
#include "score.c"
char*get_current_date_and_time(bool print){return "D2026/10/17T12:34:56\n";}
int main(void){char dir[]="/tmp/pullball-reader-XXXXXX";assert(mkdtemp(dir)&&chdir(dir)==0);loadScores();loadScores();assert(!s.scoreOne&&!s.scoreFive);FILE*f=local_open("/z.txt","w");assert(f);fputs("42;\nD2026/10/17T12:34:56\n7;\nD2026/10/16T12:00:00\n",f);fclose(f);loadScores();assert(s.scoreOne==42&&s.scoreTwo==7&&!s.scoreThree&&s.scoreOneDate&&!strcmp(s.scoreOneDate,"D2026/10/17T12:34:56\n"));unlink("scores");assert(chdir("/")==0&&rmdir(dir)==0);return 0;}
''')

    def test_pullball_score_writer_uses_storage_and_preserves_the_original_record_format(self):
     self.compile_and_run(COMMUNITY/'mbcsm--FEUP_LCOM/proj/code',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <unistd.h>
#undef fopen
static FILE*local_open(const char*p,const char*m){return fopen("scores",m);}
#define fopen local_open
#pragma clang diagnostic ignored "-Wformat-security"
#include "score.c"
char*get_current_date_and_time(bool print){return "D2026/10/17T12:34:56\n";}
int main(void){char dir[]="/tmp/pullball-writer-XXXXXX";assert(mkdtemp(dir)&&chdir(dir)==0);s=(Scores){.scoreOne=42,.scoreOneDate="D2026/10/17T12:34:56\n"};saveScores();FILE*f=local_open("bestScores.txt","r");assert(f);char b[128]={0};assert(fread(b,1,127,f)==25&&!strcmp(b,"42;\nD2026/10/17T12:34:56\n"));fclose(f);unlink("scores");assert(chdir("/")==0&&rmdir(dir)==0);return 0;}
''')

    def test_pullball_stationary_right_click_does_not_create_an_infinite_zero_speed_shot(self):
     self.compile_and_run(COMMUNITY/'mbcsm--FEUP_LCOM/proj/code',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <unistd.h>
#include "game.c"
int main(void){alarm(1);bullet=NULL;shootBullet(0,0);assert(bullet==NULL);shootBullet(100,-150);assert(bullet&&bullet->posX==500&&bullet->posY==900&&bullet->speedX==-4&&bullet->speedY==-6);free(bullet);alarm(0);return 0;}
''')

    def test_pullball_new_round_reuses_dead_block_slots_without_exceeding_the_board_storage(self):
     from unittest.mock import patch
     import os
     with patch.dict(os.environ,{'CC':os.environ.get('CC','cc')+' -fsanitize=bounds -fno-sanitize-recover=bounds'}):
      self.compile_and_run(COMMUNITY/'mbcsm--FEUP_LCOM/proj/code',[],r'''#include <lcom/lcf.h>
#include <assert.h>
#include <time.h>
static int always_spawn(void){return 0;}static void seed_unused(unsigned seed){}
#define rand always_spawn
#define srand seed_unused
#include "game.c"
#undef rand
#undef srand
static uint16_t frame[1280*1024];
int get_h_res(void){return 1280;}int get_v_res(void){return 1024;}void*get_video_mem(void){return frame;}
uint16_t getpixel(int x,int y){return 0;}void changePixel(int x,int y,uint16_t color){assert(!"zero background requires no flood fill");}
void draw_xpm(int x,int y,uint16_t*image,xpm_image_t info,uint16_t color){}
int main(void){totalBlocks=sizeof blocks/sizeof blocks[0];memset(blocks,0,sizeof blocks);blocks[0]=(Block){.x=4,.y=3,.alive=true};nextLevel();assert(totalBlocks==256&&blocks[0].alive&&blocks[0].x==4&&blocks[0].y==4);unsigned alive=0;for(unsigned i=0;i<256;i++)if(blocks[i].alive)alive++;assert(alive==13&&"twelve new columns use available dead slots and preserve the prior live block");return 0;}
''')

    def test_plunder_cursor_consumes_full_ps2_movement_once_in_screen_coordinates(self):
        self.compile_and_run(COMMUNITY/'Lantaros--LCOM/proj/src',['mouse.c','game.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "game.h"
int getHorResolution(void){return 1280;}int getVerResolution(void){return 1024;}
static void check(unsigned char flags,unsigned char x,unsigned char y,int dx,int dy){mouse_t mouse={0};position_t pos={600,500};unsigned char packet[]={flags,x,y};convert_packet_struct(packet,&mouse);assert(mouse_update_position(&mouse,&pos)==0);assert(pos.x==600+dx&&pos.y==500+dy&&"relative PS2 movement must use all9bits and inverted screenY");for(unsigned i=0;i<60;i++)assert(mouse_update_position(&mouse,&pos)==0);assert(pos.x==600+dx&&pos.y==500+dy&&"one packet cannot move again during later idle timer frames");assert(mouse.left_button==(flags&1));}
int main(void){check(0x08,20,10,20,-10);check(0x08,255,128,255,-128);check(0x38,1,128,-255,128);check(0x39,255,255,-1,1);check(0x09,0,0,0,0);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])

    def test_plunder_bitmap_renderer_writes_only_visible_pixels(self):
        self.compile_and_run(COMMUNITY/'Lantaros--LCOM/proj/src',['bitmap.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "bitmap.h"
int getHorResolution(void){return 4;}int getVerResolution(void){return 3;}
int main(void){unsigned short data[]={0x11,0x12,0x13,0x14,0x21,0x22,0x23,0x24,0x31,0x32,0x33,0x34,0x77};Bitmap bmp={.bitmapInfoHeader={.width=4,.height=3},.bitmapData=(unsigned char*)data};unsigned short page[28];for(unsigned i=0;i<28;i++)page[i]=0xcccc;drawBitmap((char*)(page+8),&bmp,0,0,ALIGN_LEFT);unsigned short want[]={0x31,0x32,0x33,0x34,0x21,0x22,0x23,0x24,0x11,0x12,0x13,0x14};for(unsigned i=0;i<12;i++)assert(page[8+i]==want[i]);for(unsigned i=0;i<8;i++)assert(page[i]==0xcccc&&page[20+i]==0xcccc&&"full-screen original sea bitmap must not write a thirteenth pixel");unsigned short saved[28];memcpy(saved,page,sizeof page);drawBitmap((char*)(page+8),&bmp,-8,0,ALIGN_LEFT);assert(memcmp(saved,page,sizeof page)==0&&"a wholly offscreen sprite cannot read or draw after unsigned-width wraparound");drawBitmap((char*)(page+8),&bmp,4,0,ALIGN_LEFT);for(unsigned i=0;i<8;i++)assert(page[20+i]==0xcccc);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])

    def test_plunder_menu_starts_with_no_phantom_mouse_motion_or_buttons(self):
        project=COMMUNITY/'Lantaros--LCOM/proj/src'
        text=(project/'game.c').read_text();start=text.index('int game_start(');brace=text.index('{',start);level=1;end=brace+1
        while level:
            level+=(text[end]=='{')-(text[end]=='}');end+=1
        function=text[start:end]
        def body(file,signature):
            source=(project/file).read_text();a=source.index(signature);b=source.index('{',a)+1;depth=1;e=b
            while depth:
                depth+=(source[e]=='{')-(source[e]=='}');e+=1
            return source[a:e]
        helpers=body('mouse.c','void convert_packet_struct(')+'\n'+body('game.c','int mouse_update_position(')
        harness=r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include <setjmp.h>
#include "game.h"
#include "i8042.h"
#include "i8254.h"
#include "timer.h"
#include "kbd.h"
#define MOUSE_START_X 30
#define MOUSE_START_Y 30
#define MOUSE_WIDTH 20
static unsigned events,bytes_read,menus;static const unsigned char packets[]={0x28,10,251,0x28,15,251};
static game_t*active;static jmp_buf ready;static Bitmap bitmap;static boat_t boat;
static void*dirty_alloc(size_t n){void*p=malloc(n);assert(p);memset(p,0xa5,n);return p;}
Bitmap*loadBitmap(const char*p){(void)p;return &bitmap;}void deleteBitmap(Bitmap*p){assert(p==&bitmap);}
void drawBitmap(char*b,Bitmap*p,int x,int y,Alignment a){(void)b;(void)p;(void)x;(void)y;(void)a;}
int load_numbers(game_t*g){(void)g;return 0;}int load_enemy_sprites(Bitmap**b){(void)b;return 0;}
boat_t*boat_constructor(unsigned hp,unsigned w,unsigned l,orient_t o,position_t p){(void)hp;(void)w;(void)l;(void)o;(void)p;return &boat;}int load_boat_sprites(boat_t*b){assert(b==&boat);return 0;}
void generate_obstacles(obstacle_t**o,unsigned*n){(void)o;*n=0;}
int timer_subscribe_int(void){assert(active->mouse);assert(active->mouse->delta_x==0&&active->mouse->delta_y==0&&"first timer/menu frame must not consume uninitialized heap deltas");assert(!active->mouse->left_button&&!active->mouse->right_button&&!active->mouse->middle_button);return 0;}
int kbd_subscribe_int(void){return 1;}int mouse_subscribe_int(void){return 12;}int write_command_mouse(unsigned long p,unsigned long c){return 0;}int mouse_packet_ignore(int i){return 0;}
int getHorResolution(void){return 1280;}int getVerResolution(void){return 1024;}
int driver_receive(endpoint_t p,message*m,int*s){assert(events<8);*s=IPC_STATUS_NOTIFY;m->m_source=HARDWARE;m->NOTIFY_ARG=events<6?BIT(12):events==6?BIT(0):BIT(1);events++;return 0;}void timer_int_handler(void){}void game_menu(game_t*g){assert(g->mouse_pos.x==55&&g->mouse_pos.y==40&&"both complete packets before one timer notification must be applied once");menus++;}int game_update(game_t*g){abort();}int getInterruptCounter(void){return 1;}void setInterruptCounter(int n){abort();}void set_enemies_change_orient_flag(game_t*g,char value){abort();}int kbd_int_handler(unsigned long*s){*s=0x81;return 0;}int mouse_int_handler(unsigned char*p){assert(bytes_read<6);*p=packets[bytes_read++];return 0;}void print_mouse_struct(const mouse_t*m){}int kbd_unsubscribe_int(void){return 0;}int clear_in_buf(void){return 0;}int mouse_unsubscribe_int(void){return 0;}
#define malloc dirty_alloc
'''+helpers+'\n'+function+r'''
#undef malloc
int main(void){game_t game={.video_mem_size=64};active=&game;assert(game_start(&game)==0);assert(game.state==MENU&&game.mouse_pos.x==55&&game.mouse_pos.y==40&&menus==1&&bytes_read==6);free(game.mouse);free(game.buffer_video2);free(game.background_buffer);return 0;}
'''
        self.compile_and_run(project,[],harness,source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])

    def test_star_patrol_entry_uses_declared_device_and_void_menu_contracts(self):
        self.compile_and_run(COMMUNITY/'bruno02960--LCOM/proj/src',['spaceinvaders.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "variables.h"
static unsigned subscribed,menus,unsubscribed,startup;
void sef_startup(void){startup++;}int devices_subscriptions(void){subscribed++;return 0;}void print_menu(void){menus++;}int devices_unsubscriptions(void){unsubscribed++;return 0;}
int invaders_original_main(int,char**);
int main(void){char*args[]={"./Space-Invaders","PLAYER",NULL};assert(invaders_original_main(2,args)==0);assert(startup==1&&subscribed==1&&menus==1&&unsubscribed==1);assert(!strcmp(user,"PLAYER"));return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmain=invaders_original_main','-O2','-Werror=implicit-function-declaration'])

    def test_star_patrol_menu_calls_the_declared_void_xpm_renderer(self):
        self.compile_and_run(COMMUNITY/'bruno02960--LCOM/proj/src',['print.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "variables.h"
#include "print.h"
#include "macros.h"
static unsigned titles,copies,clears,scans;
unsigned getHRes(void){return 1024;}unsigned getVRes(void){return 768;}unsigned getBitsPerPixel(void){return 8;}
void clear_buffer(void){clears++;}void copy_to_video_mem(void){copies++;}
void title_creator(unsigned short x,unsigned short y,char*map[]){assert(map);if(titles==0)assert(x==230&&y==220);titles++;}
void buffer_destructor(int w,int h,unsigned short x,unsigned short y){abort();}
unsigned long kbd_scan(void){scans++;return ESC_COD;}void init(void){abort();}
int main(void){menu=NEW_GAME;kbd_code=0;print_menu();assert(titles==4&&copies==1&&clears==1&&scans==1&&kbd_code==ESC_COD);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Werror=implicit-function-declaration'])
    def test_star_patrol_repeated_sprite_draws_release_the_decoded_pixmap(self):
        self.compile_and_run(COMMUNITY/'bruno02960--LCOM/proj/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "xpm.h"
static char framebuffer[32];static unsigned allocated,released;static void*last;
char*getBuffer(void){return framebuffer;}unsigned getHRes(void){return 8;}unsigned getVRes(void){return 4;}
static void*tracked_alloc(size_t n){allocated++;last=malloc(n);return last;}
static void tracked_free(void*p){assert(p==last);released++;free(p);}
#define malloc tracked_alloc
#define free tracked_free
#include "xpm.c"
#undef malloc
#undef free
int main(void){char*map[]={"2 1 2 1",". 0","+ 63",".+"};int w,h;for(unsigned i=0;i<100;i++){object_creator(&w,&h,1,1,map);assert(w==2&&h==1&&framebuffer[9]==0&&framebuffer[10]==63);title_creator(2,2,map);assert(framebuffer[18]==0&&framebuffer[19]==63);assert(allocated==released&&"temporary XPM image must be freed after copying to the framebuffer");}return 0;}
''')

    def test_duelnix_rtc_update_gate_writes_the_register_value(self):
        self.compile_and_run(COMMUNITY/'vosferatu--LCOM/proj/src',['rtc.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "rtc.h"
static unsigned selector,writes;static uint8_t regB;
int(sys_outb)(int port,uint32_t value){assert(value<256);if(port==0x70)selector=value;else{assert(port==0x71&&selector==11);regB=value;writes++;}return 0;}
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){assert(port==0x71&&selector==11);*value=regB;return 0;}
int main(void){regB=0xf6;assert(rtc_enable()==0&&regB==0x76&&writes==1);regB=0x36;assert(rtc_disable()==0&&regB==0xb6&&writes==2);assert(rtc_enable()==0&&regB==0x36&&writes==3);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Werror=int-conversion'])

    def test_duelnix_bitmap_path_allocates_its_nul_terminator(self):
        self.compile_and_run(COMMUNITY/'vosferatu--LCOM/proj/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "bitmap.h"
static unsigned calls,opened;
static void*checked_alloc(size_t n){calls++;if(calls==2)assert(n>=strlen(path_res)+strlen("0small.bmp")+1&&"path concatenation requires storage for its NUL terminator");void*p=malloc(n);assert(p);return p;}
static FILE*checked_open(const char*name,const char*mode){assert(!strcmp(name,"/home/duelnix/res/0small.bmp")&&!strcmp(mode,"rb"));opened++;return NULL;}
#define malloc checked_alloc
#undef fopen
#define fopen checked_open
#include "bitmap.c"
#undef malloc
#undef fopen
int main(void){assert(loadBitmap("0small.bmp")==NULL);assert(calls==2&&opened==1);return 0;}
''')
    def test_duelnix_new_mouse_returns_its_initialized_singleton(self):
        self.compile_and_run(COMMUNITY/'vosferatu--LCOM/proj/src',['mouse.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "mouse.h"
static Bitmap cursor;
unsigned get_hres(void){return 800;}unsigned get_vres(void){return 600;}
Bitmap*loadBitmap(const char*name){assert(!strcmp(name,"cursor.bmp"));return &cursor;}
int(sys_outb)(int port,uint32_t value){return 0;}
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){*value=port==0x64?0:0xfa;return 0;}
int main(void){Mouse*m=newMouse();assert(m&&m==getMouse()&&m==newMouse());assert(m->x==400&&m->y==300&&m->bmp==&cursor);assert(!m->leftButtonPressed&&!m->leftButtonReleased&&!m->readingInfo&&!m->update&&m->draw);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])
    def test_duelnix_entry_uses_the_declared_graphics_pointer_contract(self):
        self.compile_and_run(COMMUNITY/'vosferatu--LCOM/proj/src',['main.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "duelnix.h"
#include "mouse.h"
static Duelnix game;static unsigned initialized,ended,restored;
void sef_startup(void){}void*vg_init(unsigned short mode){assert(mode==0x114);initialized++;return &game;}int vg_exit(void){restored++;return 0;}
Duelnix*initDuelnix(void){game.done=1;return &game;}void endDuelnix(Duelnix*g){assert(g==&game);ended++;}
void updateDuelnix(Duelnix*g){abort();}void drawDuelnix(Duelnix*g){abort();}void drawDate(void){abort();}Mouse*getMouse(void){abort();}void drawMouse(void){abort();}void flip_Display(void){abort();}
int duel_original_main(int,char**);
int main(void){assert(duel_original_main(0,NULL)==0);assert(initialized==1&&ended==1&&restored==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmain=duel_original_main','-O2','-Werror=implicit-function-declaration'])

    def test_fruit_trail_mouse_starts_with_neutral_visible_coordinates(self):
        self.compile_and_run(COMMUNITY/'marisaDaniela--LCOM/project/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "mouse.h"
static void*patterned(size_t n){void*p=malloc(n);memset(p,0xa5,n);return p;}
#define malloc patterned
#include "mouse.c"
#undef malloc
int main(void){Mouse*m=newMouse();assert(m->x>=0&&m->x<1024&&m->y>=0&&m->y<768&&"initial cursor must be inside the original screen");assert(!m->xD&&!m->yD&&!m->xSign&&!m->ySign&&!m->updated&&!m->counter&&!m->leftButtonDown&&!m->rightButtonDown);return 0;}
''')

    def test_fruit_trail_bitmap_uses_the_declared_framebuffer_pointer(self):
        self.compile_and_run(COMMUNITY/'marisaDaniela--LCOM/project/src',['bitmap.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "bitmap.h"
static unsigned char frame[64];
char*getGraphicsBuffer(void){return(char*)frame;}int getHorResolution(void){return 8;}int getVerResolution(void){return 4;}
int main(void){unsigned char image[]={0x34,0x12,0x78,0x56};Bitmap b={0};b.bitmapInfoHeader.width=2;b.bitmapInfoHeader.height=1;b.bitmapData=image;memset(frame,0xa5,sizeof frame);drawBitmap(&b,1,1,ALIGN_LEFT);assert(frame[18]==0x34&&frame[19]==0x12&&frame[20]==0x78&&frame[21]==0x56);assert(frame[17]==0xa5&&frame[22]==0xa5);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Werror=implicit-function-declaration'])
    def test_fruit_trail_graphics_maps_only_initialized_bios_memory(self):
        self.compile_and_run(COMMUNITY/'marisaDaniela--LCOM/project/src',['graphics.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <machine/int86.h>
#include <assert.h>
#include "vbe.h"
#include "graphics.h"
static char pixel;static unsigned maps,permissions;
int vbe_get_mode_info(unsigned short mode,vbe_mode_info_t*info){assert(mode==0x117);memset(info,0,sizeof*info);info->XResolution=1024;info->YResolution=768;info->BitsPerPixel=16;info->PhysBasePtr=0xe0000000u;return 0;}
int lcom_compat_legacy_int86(struct reg86u*r){assert(r->u.w.bx==0x4117);r->u.w.ax=0x004f;return 0;}
int sys_privctl(endpoint_t ep,int request,void*argument){permissions++;return 0;}
void*vm_map_phys(endpoint_t ep,void*base,size_t size){assert(ep==SELF&&(uintptr_t)base==0xe0000000u&&size==1024*768*2&&"map must use initialized BIOS base and framebuffer length");maps++;return &pixel;}
void panic(const char*format,...){abort();}
int main(void){assert(vg_init(0x117)==&pixel);assert(maps==1&&permissions==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])

    def test_fruit_trail_entry_returns_the_game_result(self):
        self.compile_and_run(COMMUNITY/'marisaDaniela--LCOM/project/src',['main.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
time_t lcom_compat_time(time_t*t){return 0;}
static unsigned calls;int game(void){calls++;return 7;}int snake_original_main(int,char**);
int main(void){assert(snake_original_main(0,NULL)==7&&calls==1);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-Dmain=snake_original_main','-O2','-Werror=return-type'])
    def test_fruit_trail_failed_video_initialization_returns_failure(self):
        self.compile_and_run(COMMUNITY/'marisaDaniela--LCOM/project/src',['game.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "game.h"
time_t lcom_compat_time(time_t*t){return 0;}
static unsigned subs,loads;
FILE*lcom_compat_fopen(const char*p,const char*m){return NULL;}
int timer_subscribe_int(void){subs++;return 1;}int kbd_subscribe_int(void){subs++;return 2;}int mouse_subscribe_int(void){subs++;return 4;}
void*lcom_compat_legacy_lm_init(void){return(void*)1;}void*vg_init(unsigned short m){return NULL;}
int timer_unsubscribe_int(void){return 0;}int kbd_unsubscribe_int(void){return 0;}int mouse_unsubscribe_int(void){return 0;}int vg_exit(void){return 0;}
void initDoubleBuffer(void){abort();}Bitmap*loadBitmap(const char*n){loads++;abort();}const char*path(const char*n){abort();}
Snake*initSnake(void){abort();}Fruit*initFruit(void){abort();}Fruit*specialFruit(void){abort();}
void clearBuffer(void){abort();}void bufferToVideoMem(void){abort();}void drawBitmap(Bitmap*b,int x,int y,Alignment a){abort();}
Mouse*getMouse(void){abort();}int mouseInside(Mouse*m,int a,int b,int c,int d){abort();}Point*pointToCoord(Point*p){abort();}
void moveSnake(FILE*f,Snake*s,Fruit*r,Fruit*t){abort();}void updatepositionF(Fruit*f){abort();}void updateDirection(Snake*s,unsigned short k){abort();}void updateMouse(void){abort();}
unsigned short kbd_handler_c(void){abort();}int driver_receive(endpoint_t e,message*m,int*s){abort();}
int main(void){assert(game()==1&&subs==3&&loads==0);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Werror=return-type','-Werror=implicit-function-declaration'])
    def test_fruit_trail_snake_move_keeps_body_storage_bounded_and_owned(self):
        self.compile_and_run(COMMUNITY/'marisaDaniela--LCOM/project/src',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "snake.h"
static unsigned live,old_body_points,saved;static int saved_score;
static void*checked_alloc(size_t n){if(old_body_points){assert(n>=old_body_points*sizeof(Point*)&&"one pointer slot is required for every old body segment");old_body_points=0;}size_t*raw=malloc(sizeof(size_t)+n);*raw=n;live++;return raw+1;}
static void checked_free(void*p){if(p){assert(live);live--;free((size_t*)p-1);}}
Point*getPoint(int x,int y){Point*p=checked_alloc(sizeof* p);p->x=x;p->y=y;return p;}int comparePoints(Point*a,Point*b){return a->x==b->x&&a->y==b->y;}
int eatFruit(Fruit*f){f->fruitPosition->x=24;f->fruitPosition->y=24;return 10;}void updatepositionF(Fruit*f){f->fruitPosition->x=24;f->fruitPosition->y=24;}void writeScore(FILE*f,int score){saved++;saved_score=score;}
#define malloc checked_alloc
#define free checked_free
#include "snake.c"
#undef malloc
#undef free
int main(void){Snake*s=initSnake();Point food={24,24},bonus={23,24};Fruit f={0},b={0};f.fruitPosition=&food;b.fruitPosition=&bonus;for(unsigned i=0;i<4;i++){old_body_points=s->size;moveSnake(NULL,s,&f,&b);}assert(s->snakePosition[2]->x==6&&s->snakePosition[0]->x==4&&s->score==0);food.x=7;food.y=0;old_body_points=s->size;moveSnake(NULL,s,&f,&b);assert(s->size==4&&s->score==10&&s->snakePosition[0]->x==4&&s->snakePosition[3]->x==7);assert(live==6&&"one Snake, one pointer array and four live segment points");bonus.x=8;bonus.y=0;old_body_points=s->size;moveSnake(NULL,s,&f,&b);assert(s->size==5&&s->score==20&&s->snakePosition[4]->x==8&&live==7);s->direction=UP;old_body_points=s->size;moveSnake(NULL,s,&f,&b);assert(saved==1&&saved_score==20&&s->size==3&&s->score==0&&s->direction==RIGHT&&live==5);for(unsigned i=0;i<s->size;i++)checked_free(s->snakePosition[i]);checked_free(s->snakePosition);checked_free(s);assert(live==0);return 0;}
''')
    def test_fruit_trail_collision_saves_the_original_score_and_resets(self):
        self.compile_and_run(COMMUNITY/'marisaDaniela--LCOM/project/src',['snake.c','point.c','utils.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "snake.h"
void moveSnake(FILE*,Snake*,Fruit*,Fruit*);
int eatFruit(Fruit*f){abort();}void updatepositionF(Fruit*f){f->fruitPosition->x=24;f->fruitPosition->y=24;}
int main(void){FILE*f=tmpfile();assert(f);Snake*s=initSnake();Point food={24,24},bonus={23,24};Fruit a={0},b={0};a.fruitPosition=&food;b.fruitPosition=&bonus;s->score=35;s->direction=UP;moveSnake(f,s,&a,&b);assert(s->size==3&&s->direction==RIGHT&&s->score==0);rewind(f);char line[64];assert(fgets(line,sizeof line,f)&&!strcmp(line," score: 35 \n"));fclose(f);for(unsigned i=0;i<s->size;i++)free(s->snakePosition[i]);free(s->snakePosition);free(s);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2','-Werror=implicit-function-declaration'])

    def test_football_mouse_commands_consume_replies_before_stream_packets(self):
        self.compile_and_run(COMMUNITY/'GuilhermeSCP--football-simulator-lcom',['kbc.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "kbc.h"
static unsigned char queue[16],commands[4];static unsigned count,head,writes,reads,selected;
int(sys_outb)(int port,uint32_t value){if(port==0x64){assert(value==0xd4);selected=1;}else{assert(port==0x60&&selected&&writes<4);selected=0;commands[writes++]=value;queue[head+count++]=0xfa;}return 0;}
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){if(port==0x64){*value=count?1:0;return 0;}assert(port==0x60&&count);*value=queue[head++];count--;reads++;return 0;}
int sys_irqsetpolicy(int irq,int policy,int*hook){assert(irq==12&&*hook==2);*hook=11;return 0;}int sys_irqenable(int*hook){assert(*hook==11);return 0;}int sys_irqdisable(int*hook){return 0;}int sys_irqrmpolicy(int*hook){return 0;}
int tickdelay(clock_t ticks){return 0;}uint32_t micros_to_ticks(uint32_t micros){return 1;}
int main(void){assert(mouse_subscribe_int()==4);assert(writes==2&&commands[0]==0xf5&&commands[1]==0xf4);assert(count==0&&reads==2&&"ACK command replies must not enter the movement packet decoder");queue[head]=0x28;queue[head+1]=34;queue[head+2]=220;count=3;assert(mouse_read()==0x28&&mouse_read()==34&&mouse_read()==220&&count==0);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])

    def test_football_empty_mouse_reader_reports_no_byte(self):
        self.compile_and_run(COMMUNITY/'GuilhermeSCP--football-simulator-lcom',['kbc.c'],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include "kbc.h"
static unsigned data_reads;
int lcom_compat_sys_inb_ulong(int port,unsigned long*value){if(port==0x64){*value=0;return 0;}data_reads++;abort();}
int main(void){assert(mouse_read()==(unsigned char)-1&&"empty output buffer is not a mouse byte");assert(data_reads==0);return 0;}
''',source_flags=['-DLCOM_COMPAT_LEGACY=1','-O2'])

    def test_football_integer_text_keeps_the_terminator_inside_its_stack_buffer(self):
        from unittest.mock import patch
        import os
        with patch.dict(os.environ, {'CC':os.environ.get('CC','cc')+' -fsanitize=bounds -fno-sanitize-recover=bounds'}):
            self.compile_and_run(COMMUNITY/'GuilhermeSCP--football-simulator-lcom',[],r'''#define LCOM_COMPAT_LEGACY 1
#include <minix/drivers.h>
#include <assert.h>
#include <limits.h>
#include "video_txt.c"
int main(void){char text[80*25*2];video_mem=text;scr_width=80;scr_lines=25;
const int numbers[]={0,9,99,INT_MAX};const char*expected[]={"0","9","99","2147483647"};
for(unsigned i=0;i<4;i++){memset(text,0xa5,sizeof text);vt_print_int(numbers[i],2,3,0x12);for(unsigned j=0;j<strlen(expected[i]);j++){unsigned pos=(2*80+3+j)*2;assert(text[pos]==expected[i][j]&&text[pos+1]==0x12);}assert((unsigned char)text[(2*80+3+strlen(expected[i]))*2]==0xa5&&"integer text writes only its own digits");}return 0;}
''')


    def test_racemania_rtc_waits_for_stable_uip_clear_before_reading_the_literal_calendar(self):
        self.compile_and_run(COMMUNITY/'JFSLopes--Racemania/proj/src',[],r'''#include <assert.h>
#include "rtc.c"
int sys_irqsetpolicy(int irq,int flags,int*hook){abort();}
int sys_irqrmpolicy(int*hook){abort();}
static uint8_t registers[128],selected;
static unsigned reads,busy_reads;
static bool stable;
int(sys_outb)(int port,uint32_t value){if(port==RTC_ADDR_REG)selected=value;else{assert(port==RTC_DATA_REG);registers[selected]=value;}return 0;}
int(util_sys_inb)(int port,uint8_t*out){assert(port==RTC_DATA_REG);if(selected==RTC_REGISTER_A){assert(++reads<16&&"RTC caller must accept stable UIP-clear samples");*out=reads<=busy_reads?0x80:0;stable=!*out;}else{assert(stable&&"RTC fields must not be read while UIP is set");*out=registers[selected];}return 0;}
static void reset(unsigned busy){memset(registers,0,sizeof registers);reads=0;busy_reads=busy;stable=false;registers[RTC_SECONDS]=0x15;registers[RTC_MINUTES]=0x07;registers[RTC_HOURS]=0x09;registers[RTC_DAY_OF_MONTH]=0x17;registers[RTC_MONTH]=0x10;registers[RTC_YEAR]=0x26;registers[RTC_DAY_OF_WEEK]=0x07;}
int rtc_case(int argc,char**argv){int mode=argc>1?atoi(argv[1]):0;if(mode==0){reset(1);assert(update_in_progress()==0);assert(update_in_progress()==1);puts("UIP_POLARITY_OK");}
else if(mode==1){reset(2);date_time dt;assert(rtc_get_datetime(&dt)==0);assert(reads==3&&dt.seconds==15&&dt.minutes==7&&dt.hours==9&&dt.day==17&&dt.month==10&&dt.year==26&&dt.day_week==7);puts("STABLE_DATETIME_OK");}
else{reset(0);assert(set_alarm(120)==0);assert(registers[RTC_SECONDS_ALARM]==0x15&&registers[RTC_MINUTES_ALARM]==0x09&&registers[RTC_HOURS_ALARM]==0x09);reset(0);registers[RTC_SECONDS]=0x50;registers[RTC_MINUTES]=0x59;registers[RTC_HOURS]=0x23;assert(set_alarm(20)==0);assert(registers[RTC_SECONDS_ALARM]==0x10&&registers[RTC_MINUTES_ALARM]==0&&registers[RTC_HOURS_ALARM]==0);puts("ALARM_MINUTE_AND_MIDNIGHT_CARRY_OK");}return 0;}

int main(void){char *args[]={"rtc","1"};return rtc_case(2,args);}
''')

    def test_racemania_rtc_alarm_carries_minutes_through_midnight(self):
        self.compile_and_run(COMMUNITY/'JFSLopes--Racemania/proj/src',[],r'''#include <assert.h>
#include "rtc.c"
int sys_irqsetpolicy(int irq,int flags,int*hook){abort();}
int sys_irqrmpolicy(int*hook){abort();}
static uint8_t registers[128],selected;
static unsigned reads,busy_reads;
static bool stable;
int(sys_outb)(int port,uint32_t value){if(port==RTC_ADDR_REG)selected=value;else{assert(port==RTC_DATA_REG);registers[selected]=value;}return 0;}
int(util_sys_inb)(int port,uint8_t*out){assert(port==RTC_DATA_REG);if(selected==RTC_REGISTER_A){assert(++reads<16&&"RTC caller must accept stable UIP-clear samples");*out=reads<=busy_reads?0x80:0;stable=!*out;}else{assert(stable&&"RTC fields must not be read while UIP is set");*out=registers[selected];}return 0;}
static void reset(unsigned busy){memset(registers,0,sizeof registers);reads=0;busy_reads=busy;stable=false;registers[RTC_SECONDS]=0x15;registers[RTC_MINUTES]=0x07;registers[RTC_HOURS]=0x09;registers[RTC_DAY_OF_MONTH]=0x17;registers[RTC_MONTH]=0x10;registers[RTC_YEAR]=0x26;registers[RTC_DAY_OF_WEEK]=0x07;}
int rtc_case(int argc,char**argv){int mode=argc>1?atoi(argv[1]):0;if(mode==0){reset(1);assert(update_in_progress()==0);assert(update_in_progress()==1);puts("UIP_POLARITY_OK");}
else if(mode==1){reset(2);date_time dt;assert(rtc_get_datetime(&dt)==0);assert(reads==3&&dt.seconds==15&&dt.minutes==7&&dt.hours==9&&dt.day==17&&dt.month==10&&dt.year==26&&dt.day_week==7);puts("STABLE_DATETIME_OK");}
else{reset(0);assert(set_alarm(120)==0);assert(registers[RTC_SECONDS_ALARM]==0x15&&registers[RTC_MINUTES_ALARM]==0x09&&registers[RTC_HOURS_ALARM]==0x09);reset(0);registers[RTC_SECONDS]=0x50;registers[RTC_MINUTES]=0x59;registers[RTC_HOURS]=0x23;assert(set_alarm(20)==0);assert(registers[RTC_SECONDS_ALARM]==0x10&&registers[RTC_MINUTES_ALARM]==0&&registers[RTC_HOURS_ALARM]==0);puts("ALARM_MINUTE_AND_MIDNIGHT_CARRY_OK");}return 0;}

int main(void){char *args[]={"rtc","2"};return rtc_case(2,args);}
''')

    def test_racemania_visible_stick_center_is_neutral_and_axis_drag_uses_its_center(self):
        self.compile_and_run(COMMUNITY/'JFSLopes--Racemania/proj/src',[],r'''#include <assert.h>
#include "game_loop.c"
struct packet pp;
uint32_t counter;
uint8_t scancodes[2],position;
bool packet_ready,scancode_ready,alarm_out;
int main(void){mouse_sprite cursor={0};cursor.x=816;cursor.y=666;game.mouse=&cursor;game.stick.x=stick_init_x=800;game.stick.y=stick_init_y=650;game.stick.info.width=game.stick.info.height=32;game.stick_margin.info.width=75;game.car.angle=90;state=PLAYING;stickMove=STICK_NO_MOVEMENT;pp.lb=1;control_stick();control_stick();assert(game.car.angle==90&&game.stick.x==800&&game.stick.y==650&&"holding the visible center is neutral, not a135degree turn");cursor.x=846;control_stick();assert(game.car.angle==90&&game.stick.x==821&&game.stick.y==650);cursor.x=816;cursor.y=696;control_stick();assert(game.car.angle==180&&game.stick.x==800&&game.stick.y==671);pp.lb=0;control_stick();control_stick();assert(stickMove==STICK_NO_MOVEMENT&&game.stick.x==800&&game.stick.y==650&&game.car.angle==180);return 0;}
''')

    def test_typo_racer_initial_and_finished_wpm_handle_zero_elapsed_seconds(self):
        import os
        from unittest.mock import patch
        with patch.dict(os.environ,{'CC':os.environ.get('CC','cc')+' -fsanitize=float-cast-overflow,bounds -fno-sanitize-recover=all'}):
            self.compile_and_run(COMMUNITY/'Xouteiro--TypeRacer-in-C/proj/src',[],r'''#include "states_design/design.c"
Game game;
MenuState menuState;
MouseInfo mouse_info;
Sprite *typo_racer,*play_button,*play,*cursor,*quit,*exit_button,*esc,*you_win,*play_again,*play_again_button,*accuracy;
Sprite *letters[26],*numbers[10];
static int elapsed;
int rtc_get_elapsed_time(void){return elapsed;}
int graphics_set_frame_buffer(uint16_t m,uint8_t **p){(void)m;(void)p;return 0;}
int graphics_draw_pixel(uint16_t x,uint16_t y,uint32_t c,uint8_t *p){(void)x;(void)y;(void)c;(void)p;return 0;}
int graphics_draw_rectangle(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint32_t c,uint8_t *p){(void)x;(void)y;(void)w;(void)h;(void)c;(void)p;return 0;}
int main(void){
 uint32_t color=0;Sprite sprite={0};sprite.width=1;sprite.height=1;sprite.colors=&color;
 for(int i=0;i<26;i++)letters[i]=&sprite;
 for(int i=0;i<10;i++)numbers[i]=&sprite;
 esc=you_win=play_again=play_again_button=accuracy=&sprite;
 game.phrase="";game.pos_player=1;game.phrase_size=140;elapsed=0;
 design_draw_game_menu();if(game.wpm!=0)return 10;
 game.pos_player=10;elapsed=2;design_draw_game_menu();if(game.wpm!=60)return 11;
 game.pos_player=1;game.elapsed_time=0;design_draw_finish_menu();if(game.wpm!=0)return 12;
 game.pos_player=10;game.elapsed_time=2;design_draw_finish_menu();if(game.wpm!=60)return 13;
 puts("WPM_ZERO_AND_POSITIVE_TIME_OK");return 0;
}
''')

    def test_typo_racer_state_consumer_uses_the_pinned_mouse_header_directory_case(self):
        import sys
        flags=['-Werror=nonportable-include-path'] if sys.platform=='darwin' else []
        self.compile_and_run(COMMUNITY/'Xouteiro--TypeRacer-in-C/proj/src',['states_design/states.c'],'int main(void){return 0;}\n',source_flags=flags)

    def test_purple_minesweeper_normal_restart_retains_the_medium_board_dispatcher(self):
        self.compile_and_run(COMMUNITY/'RafaelNTeixeira--MinesweeperMINIX/proj/src',[],r'''#include <assert.h>
#include "event_handler/kbc_event_handler.c"
game_state state;static unsigned restarts;
void start_game_20x16(void){restarts++;}
void start_game_16x16(void){abort();}
void start_game_28x20(void){abort();}
void check_keyboard_a(void){abort();}
void check_keyboard_d_16x16(void){abort();}
void check_keyboard_d_20x16(void){abort();}
void check_keyboard_d_28x20(void){abort();}
void check_keyboard_enter_16x16(void){abort();}
void check_keyboard_enter_20x16(void){abort();}
void check_keyboard_enter_28x20(void){abort();}
void check_keyboard_f_16x16(void){abort();}
void check_keyboard_f_20x16(void){abort();}
void check_keyboard_f_28x20(void){abort();}
void check_keyboard_s_16x16(void){abort();}
void check_keyboard_s_20x16(void){abort();}
void check_keyboard_s_28x20(void){abort();}
void check_keyboard_w(void){abort();}
int main(void){uint8_t key[2]={R_MAKE,0};state=MEDIUM;handle_kbd_event(key);assert(restarts==1&&state==MEDIUM&&"Restart must retain the20x16 medium board and matching dispatcher");return 0;}
''',source_flags=['-O2'])

if __name__ == "__main__": unittest.main()
