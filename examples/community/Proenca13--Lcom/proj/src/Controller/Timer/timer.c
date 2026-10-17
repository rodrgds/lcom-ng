#include <lcom/lcf.h>
#include "timer.h"

#include <stdint.h>

#include "i8254.h"

int hook_id = 0;
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(timer > 2 || timer < 0 || freq < 19 || freq > TIMER_FREQ)return 1;
  uint8_t cntword;
  if(timer_get_conf(timer ,&cntword)!=0)return 1;
  cntword = cntword & 0x0F;
  cntword = cntword | TIMER_LSB_MSB;
  switch (timer)
  {
  case 0:
    cntword = cntword | TIMER_SEL0;
    break;
  case 1:
    cntword = cntword | TIMER_SEL1;
    break;
  case 2:
    cntword = cntword | TIMER_SEL2;
    break;  
  default:
    return 1;
    break;
  }
  uint8_t LSB,MSB;
  uint16_t val = TIMER_FREQ / freq;
  if(util_get_LSB(val,&LSB)!=0)return 1;
  if(util_get_MSB(val,&MSB)!=0)return 1;
  if(sys_outb(TIMER_CTRL,cntword)!=0)return 1;
  if(sys_outb(TIMER_0 +timer, LSB)!=0)return 1;
  if(sys_outb(TIMER_0 + timer , MSB)!=0)return 1;
  return 0;
}

int timer_subscribe_interrupts() {
  return sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id); 
}



int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if(st == NULL)return 1;
  uint8_t readbackcommand;
  readbackcommand = TIMER_RB_COUNT_| TIMER_RB_CMD | TIMER_RB_SEL(timer);
  if(sys_outb(TIMER_CTRL,readbackcommand)!=0)return 1;
  if(util_sys_inb(TIMER_0+timer,st)!=0)return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,enum timer_status_field field) {
  union timer_status_field_val data;
  switch (field)
  {
  case tsf_all:
    data.byte = st;
    break;
  case tsf_initial:
    st = st >> 4;
    st = st & 3;
    if(st == 1)data.in_mode =LSB_only ;
    if(st == 2)data.in_mode =MSB_only ;
    if(st == 3)data.in_mode = MSB_after_LSB;
    else data.in_mode = INVAL_val;
    break;
  case tsf_mode:
    st = st >> 1;
    st = st & 0x07;
    if(st == 6)data.count_mode = 2;
    if(st == 7)data.count_mode = 3;
    else data.count_mode = st;
    break; 
  case tsf_base:
    data.bcd = st & BIT(1);
    break;  
  default:
    return 1;
    break;
  }
  if(timer_print_config(timer,field, data)!=0)return 1; 
  return 0;
}
