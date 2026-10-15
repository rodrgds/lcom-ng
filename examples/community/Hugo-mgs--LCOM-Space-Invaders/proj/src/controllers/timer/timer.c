#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int timer_hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 19) return 1;
  if (timer < 0 || timer > 2) return 1;
  uint8_t old_conf, new_conf;
  timer_get_conf(timer, &old_conf);

  new_conf = (old_conf & 0x0F) | TIMER_LSB_MSB | (TIMER_SEL0 + timer);

  uint16_t initial_value = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  util_get_LSB(initial_value, &lsb);
  util_get_MSB(initial_value, &msb);

  if (sys_outb(TIMER_CTRL, new_conf) != OK) return 1;

  if (sys_outb(TIMER_0 + timer, lsb) != OK) return 1;
  if (sys_outb(TIMER_0 + timer, msb) != OK) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(timer_hook_id);
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer < 0 || timer > 2) return 1;

  uint8_t command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, command) != OK) return 1;
  if (util_sys_inb(TIMER_0 + timer, st) != OK) return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  if (timer < 0 || timer > 2) return 1;

  union timer_status_field_val conf;
  switch(field) {
    case tsf_all:
      conf.byte = st;
      break;
    case tsf_initial:
      conf.in_mode = st >> 4;
      break;
    case tsf_mode:
      conf.count_mode = (st & 0x0E) >> 1;
      break;
    case tsf_base:
      conf.bcd = (st & 0x01);
      break;
    default:
      return 1;
  }
  if (timer_print_config(timer, field, conf) != OK) return 1;
  return 0;
}
