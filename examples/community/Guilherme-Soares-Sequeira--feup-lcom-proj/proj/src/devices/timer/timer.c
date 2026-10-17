#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

#include "i8254.h"
#include "timer.h"

unsigned long counter; /**< @brief value that is increased everytime a timer interrupt occurs */

int32_t timer_global_hook_id; /**< @brief hook id used to unsubscribe interrupts from the timer */

uint16_t(to_bcd)(uint16_t val); /**< @brief converts an unsigned 16 bit value to bcd */

unsigned long(get_timer_get_counter)() {
  return counter;
}

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st, IV_LSB, IV_MSB, write_cmd;
  uint16_t IV;
  uint32_t min_frequency;

  timer_get_conf(timer, &st);

  write_cmd = st & (TIMER_ST_MODE | TIMER_ST_BCD); // set correct mode and BCD
  write_cmd |= TIMER_LSB_MSB;                      // activate writing LSB and MSB
  write_cmd |= (timer << 6);                       // set correct timer

  sys_outb(TIMER_CTRL, write_cmd);

  min_frequency = (st & TIMER_BCD) ? TIMER_MIN_FREQ_BCD : TIMER_MIN_FREQ_BIN;

  if (freq < min_frequency)
    return 1;

  if (st & TIMER_BCD) // encode IV in BCD
    IV = to_bcd((uint16_t) (TIMER_FREQ / freq));
  else // encode IV in binary
    IV = (uint16_t) (TIMER_FREQ / freq);

  util_get_LSB(IV, &IV_LSB);
  util_get_MSB(IV, &IV_MSB);

  sys_outb(TIMER_0 + timer, IV_LSB);
  sys_outb(TIMER_0 + timer, IV_MSB);

  return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  int hook_id = TIMER0_IRQ;

  *bit_no = (uint8_t) hook_id;

  int res = sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);

  timer_global_hook_id = hook_id;

  return res;
}

int(timer_unsubscribe_int)() {
  return sys_irqrmpolicy((int *) &timer_global_hook_id);
}

void(timer_int_handler)() {
  counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {
  sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_);

  return util_sys_inb(TIMER_0 + timer, st);
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val val;

  switch (field) {
    case tsf_all:
      val.byte = st;
      break;
    case tsf_initial:
      val.in_mode = (st & TIMER_ST_INI_RW) >> 4;
      break;
    case tsf_mode:
      val.count_mode = (st & TIMER_ST_MODE) >> 1;
      break;
    case tsf_base:
      val.bcd = st & TIMER_ST_BCD;
      break;
  }

  timer_print_config(timer, field, val);

  return 0;
}
