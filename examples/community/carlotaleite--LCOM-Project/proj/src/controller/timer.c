#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer_hook_id = 0;
int counter = 0;

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer < 0 | timer > 2)
    return 1;

  uint8_t ctrl_word;
  if (timer_get_conf(timer, &ctrl_word))
    return 1;
  ctrl_word = (ctrl_word & 0x0F) | TIMER_LSB_MSB;

  uint32_t counter_value = TIMER_FREQ / freq;
  if (counter_value > 0xFFFF) {
    return 1; // counter_value is out of range for the i8254 timer
  }
  uint8_t lsb, msb;
  if (util_get_LSB(counter_value, &lsb))
    return 1;
  if (util_get_MSB(counter_value, &msb))
    return 1;

  switch (timer) {
    case 0:
      ctrl_word |= TIMER_SEL0;
      break;
    case 1:
      ctrl_word |= TIMER_SEL1;
      break;
    case 2:
      ctrl_word |= TIMER_SEL2;
      break;
    default:
      return 1;
  }

  if (sys_outb(TIMER_CTRL, ctrl_word))
    return 1;
  if (sys_outb(TIMER_0 + timer, lsb))
    return 1;
  if (sys_outb(TIMER_0 + timer, msb))
    return 1;

  return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(timer_hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id))
    return 1;
  return 0;
}

int(timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id))
    return 0;
  return 0;
}

void(timer_int_handler)() {
  counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer < 0 || timer > 2)
    return 1;

  uint8_t readBack = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, readBack))
    return 1;
  if (util_sys_inb(TIMER_0 + timer, st))
    return 1;
  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val tsfv;

  switch (field) {
    case tsf_all:
      tsfv.byte = st;
      break;
    case tsf_initial:
      switch ((st >> 4) & 0x3) {
        case 0x1:
          tsfv.in_mode = LSB_only;
          break;
        case 0x2:
          tsfv.in_mode = MSB_only;
          break;
        case 0x3:
          tsfv.in_mode = MSB_after_LSB;
          break;
        default:
          tsfv.in_mode = INVAL_val;
          break;
      }
      break;
    case tsf_mode:
      tsfv.count_mode = (st >> 1) & 0x7;
      if (tsfv.count_mode == 0x6)
        tsfv.count_mode = 0x2;
      if (tsfv.count_mode == 0x7)
        tsfv.count_mode = 0x3;
      break;
    case tsf_base:
      tsfv.bcd = st & TIMER_BCD;
      break;
    default:
      return 1;
      break;
  }
  if (timer_print_config(timer, field, tsfv))
    return 1;
  return 0;
}
