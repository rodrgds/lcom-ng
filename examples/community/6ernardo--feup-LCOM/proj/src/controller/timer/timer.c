#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "timer.h"

unsigned int timer_counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  uint8_t st;
  if(timer_get_conf(timer,&st)!=0){
    return 1;
  }
  if (freq<19 || freq> TIMER_FREQ ){
    return 1;
  }
  uint16_t new_freq = TIMER_FREQ/freq;
  st= (st&0x0F);
  st= (st|TIMER_LSB_MSB);
  if (timer==1){
    st=(st|TIMER_SEL1);
  }
  if (timer==2){
    st=(st|TIMER_SEL2);
  }
  uint8_t new_freqlsb;
  uint8_t new_freqmsb;
  if(util_get_LSB(new_freq,&new_freqlsb)){
    return 1;
  }
   if(util_get_MSB(new_freq,&new_freqmsb)){
    return 1;
  }
  if(sys_outb(TIMER_CTRL,st)){
    return 1;
  }
  if(sys_outb(TIMER_0+timer,new_freqlsb)){
    return 1;
  }
  if(sys_outb(TIMER_0+timer,new_freqmsb)){
    return 1;
  }
  return 0;
}
int hook_id=0;
int (timer_subscribe_int)(uint8_t *bit_no) {
    if (bit_no==NULL){
      return 1;
    }
    *bit_no=BIT(hook_id);
    
    if (sys_irqsetpolicy(TIMER0_IRQ , IRQ_REENABLE, &hook_id)){return 1;}
    return 0;
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

void (timer_int_handler)() {
  timer_counter ++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t readback_command= (TIMER_RB_CMD |TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if (sys_outb(TIMER_CTRL, readback_command )!=0){
    printf("sys_out readback_command failed");
    return 1;
  }
  if(util_sys_inb(TIMER_0+timer, st)){
    return 1;
  }
  return 0;
  /* To be implemented by the students */
  
  
}
