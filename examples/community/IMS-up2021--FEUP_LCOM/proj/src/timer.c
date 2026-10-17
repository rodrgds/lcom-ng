#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

#include "timer.h"
#include "i8254.h"

static int timer_hook_id = 0;
int timer_counter = 0;

#define READ_FOUR_LSB 0x0F

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer > 2 || freq < 19) {
    return 1; // Invalid timer or frequency too high
  }

  // Read-back command to configure the timer
  uint32_t rb_comm = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, rb_comm) != OK) {
    return 1; // Failed to issue read-back command
  }

  uint8_t conf = 0;
  if (timer_get_conf(timer, &conf) != OK) {
    return 1; // Failed to get timer configuration
  }

  // Preserve the four LSBs and set the mode to LSB followed by MSB
  conf &= READ_FOUR_LSB;
  conf |= TIMER_LSB_MSB;

  // Select the timer
  switch (timer) {
    case 0: conf |= TIMER_SEL0; break;
    case 1: conf |= TIMER_SEL1; break;
    case 2: conf |= TIMER_SEL2; break;
    default: return 1; // Invalid timer
  }

  if (sys_outb(TIMER_CTRL, conf) != OK) {
    return 1; // Failed to write configuration to control register
  }

  uint16_t new_clk = TIMER_FREQ / freq;
  uint8_t lsb = 0, msb = 0;

  if (util_get_LSB(new_clk, &lsb) != OK || util_get_MSB(new_clk, &msb) != OK) {
    return 1; // Failed to split frequency into LSB and MSB
  }

  switch (timer) {
    case 0:
      if (sys_outb(TIMER_0, lsb) != OK || sys_outb(TIMER_0, msb) != OK) {
        return 1; // Failed to set timer 0 frequency
      }
      break;
    case 1:
      if (sys_outb(TIMER_1, lsb) != OK || sys_outb(TIMER_1, msb) != OK) {
        return 1; // Failed to set timer 1 frequency
      }
      break;
    case 2:
      if (sys_outb(TIMER_2, lsb) != OK || sys_outb(TIMER_2, msb) != OK) {
        return 1; // Failed to set timer 2 frequency
      }
      break;
    default:
      return 1; // Invalid timer (should never reach here)
  }

  return 0; // Success
}

void (timer_int_handler)() {
  timer_counter++;
}

int (timer_subscribe_int) (uint8_t* bit_no) {
  *bit_no = timer_hook_id;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK) {
    return 1; // Failed to subscribe timer interrupt
  }
  return 0; // Success
}

int (timer_unsubscribe_int) () {
  if (sys_irqrmpolicy(&timer_hook_id) != OK) {
    return 1; // Failed to unsubscribe timer interrupt
  }
  return 0; // Success
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (!st || timer > 2) {
    return 1; // Invalid pointer or timer number
  }

  uint32_t timer_port;
  switch (timer) {
    case 0: timer_port = TIMER_0; break;
    case 1: timer_port = TIMER_1; break;
    case 2: timer_port = TIMER_2; break;
    default: return 1; // Invalid timer (should never reach here)
  }

  if (util_sys_inb(timer_port, st) != OK) {
    return 1; // Failed to read timer configuration
  }

  return 0; // Success
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val val;

  switch (field) {
    case tsf_all:
      val.byte = st;
      break;
    case tsf_initial:
      val.in_mode = (st & (BIT(5) | BIT(6))) >> 4;
      break;
    case tsf_mode:
      val.count_mode = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;
      break;
    case tsf_base:
      val.bcd = st & BIT(0);
      break;
    default:
      return 1; // Invalid field
  }

  if (timer_print_config(timer, field, val) != OK) {
    return 1; // Failed to print timer configuration
  }

  return 0; // Success
}
