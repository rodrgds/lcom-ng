#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

extern int counter;
static int timer_hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  if(timer<0 || timer>2) return 1;

  if(freq < 19 || freq > TIMER_FREQ) return 1;

  uint16_t frequency = (uint16_t)(TIMER_FREQ / freq);

  uint8_t lsb, msb;

  if(util_get_MSB(frequency, &msb)) return 1;
  if(util_get_LSB(frequency, &lsb)) return 1;

  uint8_t st;

  if(timer_get_conf(timer, &st) != 0) return 1;

  uint8_t control_word;

  st &= 0x0f;   //Do not change 4 least-significant bits

  if (timer == 0){
    control_word = TIMER_LSB_MSB | st | TIMER_SEL0;
  }
  else if (timer == 1){
    control_word = TIMER_LSB_MSB | st | TIMER_SEL1;
  }
  else if (timer == 2){
    control_word = TIMER_LSB_MSB | st | TIMER_SEL2;
  }
  else return 1;

  if(sys_outb(TIMER_CTRL, control_word) != 0) return 1;
  
  if(timer==0){
    if(sys_outb(TIMER_0, lsb) != 0) return 1;
    if(sys_outb(TIMER_0, msb) != 0) return 1;
  }
  else if(timer==1){
    if(sys_outb(TIMER_1, lsb) != 0) return 1;
    if(sys_outb(TIMER_1, msb) != 0) return 1;
  }
  else if(timer==2){
    if(sys_outb(TIMER_2, lsb) != 0) return 1;
    if(sys_outb(TIMER_2, msb) != 0) return 1;
  }
  else return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {

  if(bit_no == NULL) return 1;

  *bit_no = BIT(timer_hook_id);

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0) return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  if(timer<0 || timer>2) return 1;

  uint8_t read_back_command = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));

  //printf("READBACK_COMM: %02x\n", read_back_command);

  if(sys_outb(TIMER_CTRL, read_back_command) != 0) return 1;
  
  if(timer==0){
    if(util_sys_inb(TIMER_0, st) != 0) return 1;
    //printf("Timer0\n");
  }
  else if(timer==1){
    if(util_sys_inb(TIMER_1, st) != 0) return 1;
    //printf("Timer1\n");
  }
  else if(timer==2){
    if(util_sys_inb(TIMER_2, st) != 0) return 1;
    //printf("Timer2\n");
  }

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  
  union timer_status_field_val conf;
  //printf("DEBUF ST: %02x\n", st);
  
  if(field==tsf_all){
      conf.byte = st;
  }
  else if(field==tsf_initial){
    st = (st >> 4) & 0x03;

    if(st == 1) conf.in_mode = LSB_only;
    else if(st == 2) conf.in_mode = MSB_only;
    else if(st == 3) conf.in_mode = MSB_after_LSB;
    else conf.in_mode = INVAL_val;
  }
  else if(field==tsf_mode){
    st = (st >> 1) & 0x07;

    if(st == 6) conf.count_mode = 2;
    else if(st == 7) conf.count_mode = 3;
    else if(st >= 0 && st <=5) conf.count_mode = st;
    else return 1;
  }
  else if(field==tsf_base){
    conf.bcd = st & TIMER_BCD;
  }
  else return 1;

  if(timer_print_config(timer, field, conf) != 0) return 1;

  return 0;
}

