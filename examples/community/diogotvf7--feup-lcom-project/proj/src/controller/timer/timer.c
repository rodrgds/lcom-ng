#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "timer.h"

#include <stdint.h>

#include "i8254.h"

static int hook_id = 0;
static int counter = 0;

int (get_counter)() {
  return counter;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st, lsb, msb;
  if (timer_get_conf(timer, &st) != OK)
    return 1;
  st &= 0xF; // reset MSBits
  switch (timer) {
    case 0:
      st = TIMER_SEL0 | TIMER_LSB_MSB | st;
      break;
    case 1:
      st = TIMER_SEL1 | TIMER_LSB_MSB | st;
      break;
    case 2:
      st = TIMER_SEL2 | TIMER_LSB_MSB | st;
      break;
  }
  if (sys_outb(TIMER_CTRL, st) != OK)
    return 1; 
  uint16_t new_freq = TIMER_FREQ / freq;
  util_get_LSB(new_freq, &lsb);
  util_get_MSB(new_freq, &msb);
  sys_outb(0x40 + timer, lsb);
  sys_outb(0x40 + timer, msb);
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK)
    return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id) != OK)
    return 1;
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint32_t command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, command) != OK)
    return EXIT_FAILURE;
  if (util_sys_inb(0x40 + timer, st))
    return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val field_val;
  switch (field) {
    case tsf_all:
      field_val.byte = st;
      break;
    case tsf_initial:
      switch ((st & TIMER_LSB_MSB) >> 4) {
        case 0:
          field_val.in_mode = INVAL_val;
          break;
        case 1:
          field_val.in_mode = LSB_only;
          break;
        case 2:
          field_val.in_mode = MSB_only;
          break;
        case 3:
          field_val.in_mode = MSB_after_LSB;
          break;
      }
      break;
    case tsf_mode:
      field_val.count_mode = (st & (TIMER_SQR_WAVE | BIT(3))) >> 1;
      if (field_val.count_mode == 6  || field_val.count_mode == 7) 
        field_val.count_mode -= 4;
      break;
    case tsf_base:
      field_val.bcd = st & TIMER_BCD;
      break;
  }

  if (timer_print_config(timer, field, field_val) != 0) 
    return EXIT_FAILURE;
  return 0;
}
