#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

static int hook_id = 2;
int counter = 0;
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  if(freq > TIMER_FREQ || freq<19){
    return 1;
  }

  uint8_t st;
  if(timer_get_conf(timer,&st) != 0) {
    printf("Error getting config\n");
    return 1;
  }

  st = st & 0x0F;
  st = st | TIMER_LSB_MSB;
  if(timer == 0){
    st = (st|TIMER_SEL0);
  }
  else if(timer == 1){
    st = (st|TIMER_SEL1);
  }
  else if(timer == 2){
    st = (st|TIMER_SEL2);
  }

  uint16_t timer_freq = TIMER_FREQ / freq;
  
  uint8_t msb,lsb;
  if(util_get_LSB(timer_freq,&lsb) != 0) {
    printf("Error getting LSB\n");
    return 1;
  }

  if(util_get_MSB(timer_freq,&msb) != 0) {
    printf("Error getting MSB\n");
    return 1;
  }

  return sys_outb(TIMER_CTRL,st)||sys_outb(TIMER_0+timer,lsb)||sys_outb(TIMER_0+timer,msb);
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id;
  if(sys_irqsetpolicy(0,IRQ_REENABLE,&hook_id) != 0) {
    printf("Error subscribe timer\n");
    return 1;
  }

  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id) != 0) {
    printf("Error unsubscribe timer\n");
    return 1;
  }
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t readBCMD = (TIMER_RB_CMD | TIMER_RB_SEL(timer)|TIMER_RB_COUNT_);
  if(sys_outb(TIMER_CTRL,(u32_t)readBCMD) != 0) {
    printf("Error sending control word\n");
    return 1;
  }

  if(util_sys_inb(TIMER_0+timer,st) != 0) {
    printf("Error reading from timer port\n");
    return 1;
  }

  return 0;
}
enum timer_init init_mode_func(uint8_t *st){

  *st = *st >> 4;
  *st &= 0x03;

  if(*st == 0x01){
    return LSB_only;
  }
  else if(*st == 0x02){
    return MSB_only;
  }
  else if(*st == 0x03){
    return MSB_after_LSB;
  }
  else{
    return INVAL_val;
  }
}
uint8_t count_mode_func(uint8_t st){

  if(st == 0x06){
    return 2;
  }
  else if(st ==0x07 ){
    return 3;
  }
  else{
    return st;
  }

}
int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val test;

  if(field == tsf_all){
    test.byte = st;
  }
  else if(field == tsf_initial){

    test.in_mode = init_mode_func(&st);
  }
  else if(field == tsf_mode){
    st = st >> 1;
    st &= 0x07;
    test.count_mode = count_mode_func(st);
  }
  else if(field == tsf_base){
    test.bcd = ((st & BIT(0)) == 1);
  }
  
  if(timer_print_config(timer,field,test) != 0) {
    printf("timer_print_config failed \n");
    return 1;
  }

  return 0;
}
