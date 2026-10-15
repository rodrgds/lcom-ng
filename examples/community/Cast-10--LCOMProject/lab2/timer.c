#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id_timer = 0;
uint32_t counter = 0;

void lazy_link(){}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if (timer > 2 || freq == 0) return 1;

  uint8_t st;
  if (timer_get_conf(timer, &st) != 0) return 1;

  st &= 0x0F;
  st |= (timer << 6) | TIMER_LSB_MSB;

  if (sys_outb(TIMER_CTRL, st) != 0) return 1;

  uint16_t divider = TIMER_FREQ / freq;
  uint8_t lsb, msb;

  if (util_get_LSB(divider, &lsb) != 0) return 1;
  if (util_get_MSB(divider, &msb) != 0) return 1;

  int port = TIMER_0 + timer;

  if (sys_outb(port, lsb) != 0) return 1;
  if (sys_outb(port, msb) != 0) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id_timer;

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) != OK) return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_timer) != OK) return 1;

  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2 || st == NULL) return 1;

  uint8_t rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, rb_cmd) != 0) return 1;
  if (util_sys_inb(TIMER_0 + timer, st) != 0) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                         enum timer_status_field field) {

  union timer_status_field_val conf_val;

  switch (field) {
    case tsf_all:
      conf_val.byte = st;
      break;

    case tsf_initial:
      conf_val.in_mode = (st >> 4) & 0x03;
      break;

    case tsf_mode: {
      uint8_t mode = (st >> 1) & 0x07;
      if (mode > 5) mode -= 4;
      conf_val.count_mode = mode;
      break;
    }

    case tsf_base:
      conf_val.bcd = st & TIMER_BCD;
      break;

    default:
      return 1;
  }

  return timer_print_config(timer, field, conf_val);
}
