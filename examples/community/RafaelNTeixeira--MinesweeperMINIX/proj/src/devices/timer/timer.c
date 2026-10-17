#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

#include "i8254.h"

static int hook_id = TIMER0_IRQ;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq < 19 || freq > TIMER_FREQ) return 1;

  uint16_t div = TIMER_FREQ / freq;
  uint8_t lsb = 0;
  uint8_t msb = 0;
  uint8_t cw = 0;

  if (util_get_LSB(div, &lsb) != 0) return 1;  // return (util_get_LSB(div, &lsb))

  if (util_get_MSB(div, &msb) != 0) return 1; // return (util_get_LSB(div, &lsb))

  if (timer_get_conf(timer, &cw) != 0) return 1; // return (util_get_conf(div, &lsb))

  cw = cw & 0x0F;
  cw = cw | TIMER_LSB_MSB;

  if (timer == 1)
    cw = cw | TIMER_SEL1;
  if (timer == 2)
    cw = cw | TIMER_SEL2;

  if (sys_outb(TIMER_CTRL, cw) != 0) return 1;

  if (sys_outb(TIMER_0 + timer, lsb) != 0) return 1;

  if (sys_outb(TIMER_0 + timer, msb) != 0) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  hook_id = TIMER0_IRQ;
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
  if(timer < 0 || timer > 2) return 1;
  if(st == NULL) return 1;

  uint32_t read_back_comm = (TIMER_RB_CMD | BIT(5) | BIT(timer + 1));
  sys_outb(TIMER_CTRL, read_back_comm);

  return util_sys_inb(TIMER_0 + timer, st);
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val value;
  
  switch (field) {
    case tsf_all: 
      value.byte = st;
      return timer_print_config(timer, field, value);

    case tsf_initial:
      value.in_mode = (st & (BIT(5) | BIT(4))) >> 4;
      return timer_print_config(timer, field, value);

    case tsf_mode: 
      if (((st & BIT(3))>>3) && ((st & BIT(2))>>2) && (!((st & BIT(1))>>1))) {
        value.in_mode = 2;
      }
      else if (((st & BIT(3))>>3) && ((st & BIT(2))>>2) && (((st & BIT(1))>>1))) {
        value.in_mode = 3;
      }
      else value.in_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
      return timer_print_config(timer, field, value);

    case tsf_base: 
      value.bcd = st & BIT(0);
      return timer_print_config(timer, field, value);
  }
}
