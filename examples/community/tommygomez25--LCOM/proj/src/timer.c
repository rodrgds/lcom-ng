#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
uint32_t COUNTER = 0;
int hook_id = TIMER0_IRQ;
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer < 0 || timer > 2 || freq < 19)  {
    return 1;
  }
  uint8_t st;
  timer_get_conf(timer, &st);
  uint8_t preserve = st << 4;
  preserve = preserve >> 4;
  uint32_t controlWord = TIMER_LSB_MSB | preserve;
  if(timer == 0){
    controlWord = TIMER_SEL0 | controlWord;
  }
  else if(timer == 1){
    controlWord = TIMER_SEL1 | controlWord;
  }
  else if(timer == 2){
    controlWord = TIMER_SEL2 | controlWord;
  }
  uint32_t div = TIMER_FREQ / freq;
  uint8_t msb = (uint8_t) (div >> 8);
  uint8_t lsb = (uint8_t) div;

  if (sys_outb(TIMER_CTRL, controlWord) != OK) {
    return 1;
  }
  if (sys_outb(TIMER_0 + timer, lsb) != OK) {
    return 1;
  }
  if (sys_outb(TIMER_0 + timer, msb) != OK) {
    return 1;
  }
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id;
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
  return 0;
}

int (timer_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_id);
  return 0;
}

void (timer_int_handler)() {
 COUNTER++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
     *st = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
     if (timer == 0) {
       sys_outb(TIMER_CTRL, *st);
       util_sys_inb(TIMER_0, st);
      return 0;
    }
    else if (timer == 1) {
      sys_outb(TIMER_CTRL, *st);
      util_sys_inb(TIMER_1, st);
      return 0;
    }
   else if (timer == 2) {
      sys_outb(TIMER_CTRL, *st);
      util_sys_inb(TIMER_2, st);
      return 0;
   }
   else {
      return 1;
   }
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val val1;
  if (field == tsf_all) {
    val1.byte = st;
  }
  else if (field == tsf_initial) {
    if ((st & TIMER_LSB_MSB) == TIMER_LSB_MSB) {
      val1.in_mode = MSB_after_LSB;
    }
    else if ((st & TIMER_LSB) == TIMER_LSB) {
      val1.in_mode = LSB_only;
    }
    else if ((st & TIMER_MSB) == TIMER_MSB) {
      val1.in_mode = MSB_only;
    }
    else {
      val1.in_mode = INVAL_val;
    }
  }
  else if (field == tsf_mode) {
    uint8_t mode = (BIT(3) | BIT(2) | BIT(1)) & st;
    mode = mode >> 1;
    if (mode == 6) {
      mode = 2;
    }
    if (mode == 7) {
      mode = 3;
    }
    val1.count_mode = mode;
  }
  else if (field == tsf_base) {
    val1.bcd = BIT(0) & st;
  }
  else {
    return 1;
  }
  timer_print_config(timer, field, val1);
  return 0;
}
