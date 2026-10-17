#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id=0;
uint8_t count=0;
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(timer>2||timer<0 ||freq> TIMER_FREQ||freq<19){
    return 1;
  }
  uint8_t MSB,LSB,CW;
  if(timer_get_conf(timer,&CW))return 1;
  CW=(CW&0xf)|TIMER_LSB_MSB;
  uint16_t div= TIMER_FREQ/freq;
  util_get_LSB(div,&LSB);
  util_get_MSB(div,&MSB);
  uint8_t selectedTimerAddress;
  switch(timer){
  case(0):
    CW|=TIMER_SEL0;
    selectedTimerAddress=TIMER_0;
    break;
  case(1):
    CW|=TIMER_SEL1;
    selectedTimerAddress=TIMER_1;
    break;
  case(2):
    CW|=TIMER_SEL2;
    selectedTimerAddress=TIMER_2;
    break;
  }
  return (sys_outb(selectedTimerAddress,MSB)|sys_outb(selectedTimerAddress,LSB)|sys_outb(TIMER_CTRL,CW));
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  hook_id=*bit_no;
  if(bit_no==NULL) return 1;
  if(sys_irqsetpolicy( TIMER0_IRQ,IRQ_REENABLE, &hook_id))return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

void (timer_int_handler)() {
  count++;
}



int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if(timer>2||timer<0 ||st==NULL){
    return 1;
  }
  uint8_t RBC=(TIMER_RB_CMD|TIMER_RB_COUNT_|TIMER_RB_SEL(timer));
  return (sys_outb(TIMER_CTRL,RBC)|util_sys_inb(TIMER_0 + timer,st));
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val conf;
  switch(field){
  case(tsf_all):
    conf.byte=st;
    break;
  case(tsf_initial):
    st=(st>>4)&(0x03);
    if(st==3){
      conf.in_mode=MSB_after_LSB;
    }
    if(st==2){
      conf.in_mode=MSB_only;
    }
    if(st==1){
      conf.in_mode=LSB_only;
    }
    else{conf.in_mode=INVAL_val;} 
    break;
    case(tsf_mode):
      st=(st>>1)&0x07;
      conf.count_mode=st;
    break;
    case(tsf_base):
    st=st&0x1;
    conf.bcd=st;
    break;
    default:
    return 1;
  }
  
  
  if(timer_print_config(timer,field,conf))return 1; 

  return 0;
}



