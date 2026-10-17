#include <minix/drivers.h>
int fixture_driver_receive(int,message *,int *);
#define driver_receive fixture_driver_receive
#define main original_game_main
#include ASTEROID_SOURCE
#undef main
#include <assert.h>
static Sprite pool[130];static int pool_count;
Sprite *create_sprite(char *pic[],int x,int y){Sprite *s=&pool[pool_count++];s->x=x;s->y=y;s->width=1;s->height=1;return s;}
void draw_sprite(Sprite *s){}
int draw_battleship(Sprite *s){return 0;}
int draw_asteroid(Sprite *s){return 0;}
void erase_sprite(Sprite *s,unsigned long background){}
void vg_buffertomem(void){}void vg_memtobuffer(void){}
int vg_fill(unsigned long color){return 0;}int vg_fill_buffer(unsigned long color){return 0;}
void *vg_init(unsigned long mode){return pool;}int vg_exit(void){return 0;}
int mouse_subscribe_int(void){return 0;}int mouse_unsubscribe_int(void){return 0;}
int rtc_subscribe_int(void){return 0;}int rtc_unsubscribe_int(void){return 0;}
int timer_subscribe_int(void){return 0;}int timer_unsubscribe_int(void){return 0;}
int kbd_subscribe_int(void){return 0;}int kbd_unsubscribe_int(void){return 0;}
int kbd_int_handler(const unsigned int shots){return 0;}
int rtc_int_handler(unsigned short readtime,unsigned short *timesalarm,int delta){return 0;}
int mouse_int_handler(int*x,int*y,int*px,int*py,int h,int v){return 0;}
int driver_receive(int source,message *msg,int *status){*status=IPC_STATUS_NOTIFY;msg->m_source=HARDWARE;msg->NOTIFY_ARG=finished?4:32;return 0;}
int main(int argc,char **argv){
 if(argc>1 && strcmp(argv[1],"timer")==0){
  for(int i=0;i<NUM_ASTEROIDS;i++)asteroids[i]=create_sprite(NULL,0,100);
  for(int i=0;i<4;i++)shotsprt[i]=create_sprite(NULL,0,100);
  intcounter=1;shots=4;changed=0;timer_int_handler();assert(intcounter==1);
 }else{assert(original_game_main(0,NULL)==0);assert(pool_count>NUM_ASTEROIDS);}
 return 0;
}
