#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hookid;
extern int global_counter;
extern int hunger_counter;
extern int to_sleep_counter;
extern int hygiene_counter;
extern int love_counter;
extern int play_counter;
extern int minute_counter;
extern int petting_counter;



int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint16_t init = TIMER_FREQ/freq;
  uint8_t lsb; uint8_t msb;
  if (util_get_MSB(init, &msb)) return 1;
  if (util_get_LSB(init, &lsb)) return 1;

  uint8_t st;
  if (timer_get_conf(timer, &st)) return 1;
  uint8_t op_mode = (st & (BIT(0) | BIT(1) | BIT(2) | BIT(3)));

  uint8_t control_w = TIMER_SEL(timer) | TIMER_LSB_MSB | op_mode;
  if (sys_outb(TIMER_CTRL, control_w)) return 1;
  if (sys_outb(TIMER(timer), lsb)) return 1;
  if (sys_outb(TIMER(timer), msb)) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  hookid = 0;
  *bit_no = hookid;

  if (sys_irqsetpolicy(TIMER0_IRQ,  IRQ_REENABLE , &hookid)) return -1;

  return 0;
}

int (timer_unsubscribe_int)() {
  
  return sys_irqrmpolicy(&hookid);

}

void (timer_int_handler)() {
  global_counter++;
  hunger_counter++;
  to_sleep_counter++;
  hygiene_counter++;
  love_counter++;
  play_counter++;
  minute_counter++;
  petting_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint32_t rb_cmd;
  rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  
  if (util_sys_inb(TIMER(timer), st)) return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

  union timer_status_field_val val;
  switch(field){
    case tsf_all: 
      val.byte = st; 
      break;
    case tsf_initial: 
      if (st & TIMER_MSB && st & TIMER_LSB) val.in_mode = MSB_after_LSB;
      else if (st & TIMER_LSB) val.in_mode = LSB_only;
      else if (st & TIMER_MSB) val.in_mode = MSB_only;
      else val.in_mode = INVAL_val;
      break;
    case tsf_mode:
      val.count_mode = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;
      break;
    case tsf_base:
      val.bcd = st & BIT(0);
      break;
  }

  if (timer_print_config(timer, field, val)) return 1;

  return 0;
}
