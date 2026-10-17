#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

static int hook_id = TIMER0_IRQ;

uint32_t counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  // freq out of bounds
  if (freq < 19 || freq > (1 << 16)) return 1;

  // get current conf first
  uint8_t tmp;
  if (timer_get_conf(timer, &tmp) != 0) return -1;

  // assemble and send ctrl word
  tmp = (tmp & 0xF) | TIMER_LSB_MSB | TIMER_SEL(timer);
  if (sys_outb(TIMER_CTRL, tmp) != OK) return -2;

  uint8_t lsb, msb;
  uint16_t val = TIMER_FREQ / freq;
  util_get_LSB(val, &lsb);
  util_get_MSB(val, &msb);
 
  // update timer frequency
  if (sys_outb(TIMER(timer), lsb) != OK) return -2;
  if (sys_outb(TIMER(timer), msb) != OK) return -2; 
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = hook_id;
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint32_t out = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;
  // send rb cmd
  if (sys_outb(TIMER_CTRL, out) != OK) {
    return -1;
  }
  // get rb response
  if (util_sys_inb(TIMER(timer), st) != OK) {
    return -2;
  }
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val conf;

  switch (field) {
    case tsf_all:
      conf.byte = st;
      break;
    case tsf_base:
      conf.bcd = st & BIT(0);
      break;
    case tsf_mode:
      st = (st >> 1) & 0x7;
      // to account for the X bits
      switch (st) {
        case 0x6:
          conf.count_mode = 2;
          break;
        case 0x7:
          conf.count_mode = 3;
          break;
        default:
          conf.count_mode = st;
      }
      break;
    case tsf_initial: {
      st = (st >> 4) & 0x3;
      // parse init mode
      switch (st) {
        case 0x1:
          conf.in_mode = LSB_only;
          break;
        case 0x2:
          conf.in_mode = MSB_only;
          break;
        case 0x3:
          conf.in_mode = MSB_after_LSB;
          break;
        default:
          conf.in_mode = INVAL_val;
      }
    }
  }
  
  return timer_print_config(timer, field, conf);
}
