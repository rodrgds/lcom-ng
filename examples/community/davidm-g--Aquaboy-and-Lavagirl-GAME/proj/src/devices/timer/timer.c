#include "timer.h"
#include <lcom/lcf.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = 0;

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer < 0 || timer > 2)
    return 1;
  if (freq < 19 || freq > TIMER_FREQ)
    return 1;
  uint8_t st;
  if (timer_get_conf(timer, &st))
    return 1;
  uint8_t control_word = 0x00;
  uint8_t mask = 0x0F;
  control_word |= (st & mask);
  uint16_t value = (uint16_t) (TIMER_FREQ / freq);
  control_word |= TIMER_LSB_MSB;

  switch (timer) {
    case 0:
      control_word |= TIMER_SEL0;
      break;
    case 1:
      control_word |= TIMER_SEL1;
      break;
    case 2:
      control_word |= TIMER_SEL2;
      break;
    default:
      break;
  }
  sys_outb(TIMER_CTRL, control_word);
  uint8_t lsb, msb;
  if (util_get_LSB(value, &lsb))
    return 1;
  if (util_get_MSB(value, &msb))
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
  *bit_no = BIT(hook_id);
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int(timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL)
    return 1;
  if (timer < 0 || timer > 2)
    return 1;
  int timer_address;
  uint8_t word;
  switch (timer) {
    case 0:
      timer_address = TIMER_0;
      word = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(0));
      break;
    case 1:
      timer_address = TIMER_1;
      word = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(1));
      break;
    case 2:
      timer_address = TIMER_2;
      word = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(2));
      break;
    default:
      return 1;
  }
  sys_outb(TIMER_CTRL, word);              // Writes Read-back command to Control Register
  int r = util_sys_inb(timer_address, st); // read the configuration of the specified timer and writes it to st

  return r;
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val val;
  int count = 0;
  // put in val the configuration fields stated by field, by masking st.
  switch (field) {
    case tsf_all:
      val.byte = st;
      break;
    case tsf_initial:
      val.in_mode = (TIMER_LSB_MSB & st) >> 4;
      break;
    case tsf_mode:
      count = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;
      if (count == 6) {
        val.count_mode = 2;
      }
      else if (count == 7) {
        val.count_mode = 3;
      }
      else {
        val.count_mode = count;
      }
      break;
    case tsf_base:
      val.bcd = (st & TIMER_BCD);
      break;
    default:
      return 1;
      break;
  }
  timer_print_config(timer, field, val);
  return 0;
}
