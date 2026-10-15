#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "timer.h"

static uint32_t timer_counter = 0;
static int timer_hook_id = 0;

#include "i8254.h"

uint32_t get_timer_counter() {
  return timer_counter;
}

void reset_timer_counter() { timer_counter = 0; }

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer > 2 || freq < 19 || freq > TIMER_FREQ) {
    return 1;
  }

  uint8_t st;
  if (timer_get_conf(timer, &st) != 0) {
    return 1;
  }


  uint16_t div = (uint16_t)(TIMER_FREQ / freq);
  if (div == 0) return 1;

  // Make control word
  uint8_t control = 0;
  switch (timer) {
    case 0:
      control = TIMER_SEL0;
      break;
    case 1:
      control = TIMER_SEL1;
      break;
    case 2:
      control = TIMER_SEL2;
      break;
    default:
      return 1;
  }
  control |= TIMER_LSB_MSB; 
  control |= (st & 0x0F);  

  // Write control word to the control register
  if (sys_outb(TIMER_CTRL, control) != 0) {
    return 1;
  }

  uint8_t lsb, msb;
  if (util_get_LSB(div, &lsb) != 0 || util_get_MSB(div, &msb) != 0) {
    return 1;
  }

  uint8_t port;
  switch (timer) {
    case 0:
      port = TIMER_0;
      break;
    case 1:
      port = TIMER_1;
      break;
    case 2:
      port = TIMER_2;
      break;
    default:
      return 1;
  }

  // Update the new frequency
  if (sys_outb(port, lsb) != 0) return 1;
  if (sys_outb(port, msb) != 0) return 1;

  return 0; 
}

// Subscribes (activates) timer interrupts
int (timer_subscribe_int)(uint8_t *bit_no) {
  timer_hook_id = 0;
  if (bit_no == NULL) return 1;
  timer_hook_id = 0;
  *bit_no = timer_hook_id;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0) {
    return 1;
  }

  return 0;
}

// Unsubscribes (deactivates) timer interrupts
int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id) != 0) {
    return 1;
  }

  return 0;
}

// Increases the amount of times that the handler was called, i.e. the amount of timer interrupts
void (timer_int_handler)() {
  timer_counter++;
}


int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2) {
    return 1;
  }

  // Construct the Read-Back Command
  uint8_t rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, rb_cmd) != 0) {
    return 1;
  }

  // Determine the appropriate port based on the timer number
  uint8_t port;
  switch (timer) {
    case 0:
      port = TIMER_0;
      break;
    case 1:
      port = TIMER_1;
      break;
    case 2:
      port = TIMER_2;
      break;
    default:
      return 1;
  }

  // Read the status byte from the appropriate timer port
  if (util_sys_inb(port, st) != 0) {
    return 1;
  }

  return 0;
}


int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val conf;
  switch (field) {
    case tsf_all:
      conf.byte = st;
      break;
    case tsf_initial: {
      uint8_t init = (st & (BIT(5) | BIT(4))) >> 4;
      switch (init) {
        case 1:
          conf.in_mode = LSB_only;
          break;
        case 2:
          conf.in_mode = MSB_only;
          break;
        case 3:
          conf.in_mode = MSB_after_LSB;
          break;
        default:
          conf.in_mode = INVAL_val;
      }
      break;
    }
    case tsf_mode: {
      uint8_t mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
      if (mode > 5) mode &= 0x03; // modes 6 and 7 are same as 2 and 3
      conf.count_mode = mode;
      break;
    }
    case tsf_base:
      conf.bcd = (st & TIMER_BCD) ? true : false;
      break;
    default:
      return 1;
  }
  return timer_print_config(timer, field, conf);
}
