#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

uint32_t timer_interrupt_count = 0; // More descriptive name
int timer_hook_id = 0;             // More descriptive name

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 19) {
    return 1; // Indicate an error
  }

  uint8_t control_word;
  if (timer_get_conf(timer, &control_word) != 0) {
    return 1; // Indicate an error
  }

  control_word = (control_word & 0x0F) | TIMER_LSB_MSB;

  uint32_t initial_value = TIMER_FREQ / freq;
  uint8_t msb, lsb;
  if (util_get_MSB(initial_value, &msb) != 0 || util_get_LSB(initial_value, &lsb) != 0) {
    return 1; // Indicate an error
  }

  uint8_t selectTimer;
  switch (timer) {
    case 0:
      control_word |= TIMER_SEL0;
      selectTimer = TIMER_0;
      break;
    case 1:
      control_word |= TIMER_SEL1;
      selectTimer = TIMER_1;
      break;
    case 2:
      control_word |= TIMER_SEL2;
      selectTimer = TIMER_2;
      break;
  }

  if (sys_outb(TIMER_CTRL, control_word) != 0) return 1;

  if (sys_outb(selectTimer,lsb) != 0) return 1;
  if (sys_outb(selectTimer, msb) != 0) return 1;
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    return 1; // Indicate an error
  }

  *bit_no = BIT(timer_hook_id);

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0) {
    return 1; // Indicate an error
  }

  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id) != 0) {
    return 1; // Indicate an error
  }
  return 0;
}

void (timer_int_handler)() {
  timer_interrupt_count++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2) {
    return 1; // Indicate an error
  }
  uint8_t read_back_command = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if (sys_outb(TIMER_CTRL, read_back_command) != 0) {
    return 1; // Indicate an error
  }
  if (util_sys_inb(TIMER_0 + timer, st) != 0) {
    return 1; // Indicate an error
  }
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val data;

  switch (field) {
    case tsf_all:
      data.byte = st;
      break;

    case tsf_initial:
      st = (st >> 4) & 0x03; // Extract bits 4 and 5
      if (st == 1) {
        data.in_mode = LSB_only;
      } else if (st == 2) {
        data.in_mode = MSB_only;
      } else if (st == 3) {
        data.in_mode = MSB_after_LSB;
      } else {
        data.in_mode = INVAL_val;
      }
      break;

    case tsf_mode:
      st = (st >> 1) & 0x07; // Extract bits 1, 2, and 3
      if (st == 6) {
        data.count_mode = 2;
      } else if (st == 7) {
        data.count_mode = 3;
      } else {
        data.count_mode = st;
      }
      break;

    case tsf_base:
      data.bcd = st & TIMER_BCD; // Check bit 0
      break;

    default:
      return 1; // Invalid field
  }

  if (timer_print_config(timer, field, data) != 0) {
    return 1; // Indicate an error
  }
  return 0;
}
