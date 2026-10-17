#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"



int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if(timer<0 || timer>2)return 1;
  if(freq<18||freq>TIMER_FREQ)return 1;
  uint8_t timer_opmode_countmode;   //to get the four lsb of the status
  if(timer_get_conf(timer,&timer_opmode_countmode))return 1;;
  timer_opmode_countmode=timer_opmode_countmode<<4;
  timer_opmode_countmode=timer_opmode_countmode>>4;

  uint16_t init_freq;
  init_freq=(uint16_t)(TIMER_FREQ/freq);
  
  uint8_t lsb,msb;
  if(util_get_MSB(init_freq,&msb))return 1;
  if(util_get_LSB(init_freq,&lsb))return 1;

  uint8_t timersel='\0';
  switch (timer)
  {
  case 0:
    timersel=TIMER_SEL0;
    break;
  case 1:
    timersel=TIMER_SEL1;
    break;
  case 2:
    timersel=TIMER_SEL2;
  default:
    return 1;
    break;
  }


  uint8_t controlword =timersel| TIMER_LSB_MSB | timer_opmode_countmode;
  
  if(sys_outb(TIMER_CTRL,controlword))return 1;
  if(sys_outb(TIMER_0+timer,lsb))return 1;
  if(sys_outb(TIMER_0+timer,msb))return 1;
  return 0;


}




uint32_t counter=0;
int hook_id =TIMER0_IRQ;


int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id;
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE,&hook_id))return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

void (timer_int_handler)() {
  counter++;
}





int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t readbackcmd;
  readbackcmd=TIMER_RB_CMD|TIMER_RB_COUNT_|TIMER_RB_SEL(timer);
  if(sys_outb(TIMER_CTRL, readbackcmd))return 1;
  if(util_sys_inb( TIMER_0+timer, st)) return 1; 
  return 0;
  
}


int (timer_display_conf)(uint8_t timer, uint8_t st,enum timer_status_field field) {
  union timer_status_field_val fvalue;
  uint8_t init_mode;
  
  if (timer<0||timer>2) return 1;
  
  switch (field)
  {
  case tsf_all:
    fvalue.byte=st;
    break;
  
  case tsf_initial:
    //init_mode holds only the initialization mode bits of st 

    init_mode=st<<2;
    init_mode=init_mode>>6;
    init_mode=init_mode<<4;
   
    if(init_mode == TIMER_LSB_MSB)
      fvalue.in_mode=MSB_after_LSB;

    else if(init_mode == TIMER_LSB)
      fvalue.in_mode=LSB_only;

    else if (init_mode == TIMER_MSB)
      fvalue.in_mode=MSB_only;
    
    else fvalue.in_mode=INVAL_val;
    
    break;
 
  case tsf_mode: 
    
    fvalue.count_mode = (st & 0x0E) >> 1;
    if(fvalue.count_mode==6)fvalue.count_mode=2;
    if(fvalue.count_mode==7)fvalue.count_mode=3;
    if(fvalue.count_mode<0||fvalue.count_mode>5)return 1;
    break;
  case tsf_base:
    fvalue.bcd=((st & 0x01) ==TIMER_BCD);
    break;
      
  default:
    return 1;
    break;
  }
  

  if(timer_print_config( timer,  field, fvalue))return 1;
   
  return 0;
}
