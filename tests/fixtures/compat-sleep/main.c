#include <lcom/lcf.h>
static uint8_t *frame;
static void fill(uint8_t red,uint8_t green,uint8_t blue) {
  for(unsigned pixel=0;pixel<800*600;pixel++) {
    frame[pixel*3]=blue;frame[pixel*3+1]=green;frame[pixel*3+2]=red;
  }
}
int compat_sleep_frame(unsigned route) {
  if(!frame)frame=vg_init(0x115);
  if(!frame)return 1;
  if(route==2) { fill(255,0,0);if(sleep(2))return 2; }
  else if(route==1) { fill(0,255,0);if(usleep(500000))return 3; }
  else { fill(0,255,255);if(tickdelay(30))return 4; }
  /* The next mapped frame must not replace the frame already presented
     during the blocking call until a later presentation boundary. */
  fill(0,0,255);
  return 0;
}
int compat_sleep_finish(void) { frame=NULL;return lcf_cleanup(); }
#ifndef LCOM_COMPAT_SLEEP_NATIVE_FIXTURE
int main(int argc,char **argv) { return lcf_start(argc,argv); }
int proj_main_loop(int argc,char **argv) {
  if(compat_sleep_frame(2)||compat_sleep_frame(1)||compat_sleep_frame(0))return 1;
  int hook=0;if(sys_irqsetpolicy(0,IRQ_REENABLE,&hook))return 2;
  message msg;int status;
  for(;;)if(driver_receive(ANY,&msg,&status))return 3;
}
#endif
