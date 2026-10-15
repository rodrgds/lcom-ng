#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

uint32_t time_counter = 0;
int timer_hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  // Timer can't be more than 1193182 Hz (CPU Frequency) or less than 19 Hz (Leading to overflow)
  if (freq > TIMER_FREQ || freq < 19) return 1;

  // Timer can only be 0, 1 or 2
  if (timer < 0 || timer > 2) return 1;

  // Get timer configuration
  uint8_t config;
  if (timer_get_conf(timer, &config) != 0) return 1;  

  config = (config & 0x0F) | TIMER_LSB_MSB;

  // Calculate the new value of the timer
  uint16_t frequency = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  util_get_LSB(frequency, &lsb);
  util_get_MSB(frequency, &msb);

  uint8_t select;
  switch (timer) {
    case 0:
      config = config | TIMER_SEL0;
      select = TIMER_0;
      break;
    case 1:
      config = config | TIMER_SEL1;
      select = TIMER_1;
      break;
    case 2:
      config = config | TIMER_SEL2;
      select = TIMER_2;
      break;
    default:
      return 1;
  }

  if (sys_outb(TIMER_CTRL, config) != 0) return 1;

  if (sys_outb(select, lsb) != 0) return 1;
  if (sys_outb(select, msb) != 0) return 1;

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
  time_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer < 0 || timer > 2) return 1;

  uint8_t rb_command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  
  if (sys_outb(TIMER_CTRL, rb_command) != OK) return 1;
  if (util_sys_inb(TIMER_0 + timer, st) != OK) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val config;
  switch (field){
    
    case tsf_all:
      config.byte = st;
      break;
    case tsf_initial:
      config.in_mode = (st & TIMER_LSB_MSB) >> 4;
      break;

    case tsf_mode:
      config.count_mode = (st & (BIT(3) | TIMER_SQR_WAVE)) >> 1;
      break;

    case tsf_base:
      config.bcd = st & TIMER_BCD;
      break;

    default:
      return 1;
  }

  if (timer_print_config(timer, field, config) != 0) return 1;
  return 0;
}
