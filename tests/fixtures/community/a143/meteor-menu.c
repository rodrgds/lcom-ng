#define main meteor_source_main
#include MAIN_SOURCE
#undef main
#include <lcom/lcf.h>
#include <assert.h>
#include "game/game.h"
#include "devices/mouse.h"
#include "devices/keyboard.h"
static int step, back=-1, presented=-1;
static int scenario;
uint8_t scancode,mouse_counter;
struct packet mouse_packet;
int counter,x=575,y=432;
void menu_interrupts(uint8_t,uint8_t);
void game_update(void){back=current_state;}
int buf_swap(void){presented=back;return 0;}
int vg_draw_xpm(xpm_map_t image,uint16_t a,uint16_t b){assert(image&&a<1152&&b<864);return 0;}
int vg_draw_rectangle(uint16_t a,uint16_t b,uint16_t w,uint16_t h,uint32_t c){return 0;}
void kbc_ih(void){}
void mouse_ih(void){}
int mouse_sync_bytes(void){mouse_counter=3;return 0;}
int mouse_bytes_to_packet(void){return 0;}
void updateMouseLocation(void){}
bool processLeftClick(void){return true;}
int driver_receive(endpoint_t src,message*msg,int*status){
  assert(step<3&&"menu must finish after the ordinary Exit input");
  if(step==0&&scenario==0)assert(presented==STATE_MENU&&"initial menu must paint before waiting for input");
  if(step==1)assert(presented==STATE_INFO&&"Info input must present without a mouse movement");
  if(step==2)assert(presented==STATE_MENU&&"Back input must present without a mouse movement");
  *status=IPC_STATUS_NOTIFY;msg->m_source=HARDWARE;
  if(scenario==2){msg->m_notify.interrupts=1;x=step==1?575:600;y=step==0?530:step==1?780:680;}
  else{msg->m_notify.interrupts=2;scancode=step==0?MAKE_2:step==1?MAKE_1:MAKE_3;}
  step++;return 0;
}
int main(int argc,char**argv){assert(argc==2);scenario=atoi(argv[1]);current_state=STATE_MENU;menu_interrupts(1,2);assert(step==3&&current_state==STATE_EXIT);return 0;}
