#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "src/include/Macros.h"
#include "src/include/controllers/timer/timer.h"

static int hookId = 0;
uint8_t timerCounter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st;
  CHECK_RETURN(timer_get_conf(timer,&st));
  st = (st&0x0F)|TIMER_LSB_MSB;
  uint16_t fullBytes = TIMER_FREQ/freq;
  uint8_t mSB,lSB;
  CHECK_RETURN(util_get_LSB(fullBytes,&lSB));
  CHECK_RETURN(util_get_MSB(fullBytes,&mSB));
  uint8_t timerRegister = 0x40 + timer;
  st |= timer << 6;
  CHECK_RETURN(sys_outb(TIMER_CTRL, st));
  CHECK_RETURN(sys_outb(timerRegister, lSB));
  CHECK_RETURN(sys_outb(timerRegister, mSB));
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(hookId);
  CHECK_RETURN(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookId));
  return 0;
}

int (timer_unsubscribe_int)() {
  CHECK_RETURN(sys_irqrmpolicy(&hookId));
  return 0;
}

void (timer_int_handler)() {
  if((++timerCounter)%60 == 0) timerCounter = 0;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint32_t rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_;
  rb_cmd |= TIMER_RB_SEL(timer);
  CHECK_RETURN(sys_outb(TIMER_CTRL, rb_cmd));
  CHECK_RETURN(util_sys_inb(TIMER_0+timer, st));
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val values;
  switch (field){
  case tsf_all:
    values.byte = st;
    break;
  case tsf_initial:
    values.in_mode = (st >> 4) & 3;
    break;
  case tsf_mode:
    st = (st >> 1) & 7;
    if(st == 6) st = 2;
    else if(st == 7) st = 3;
    values.count_mode = st;
    break;
  case tsf_base:
    values.bcd = st & 1;
    break;
  default:
    return 1;  
  }
  CHECK_RETURN(timer_print_config(timer, field, values));
  return 0;
}
