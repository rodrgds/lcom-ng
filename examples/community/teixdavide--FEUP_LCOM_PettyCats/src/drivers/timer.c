#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

#define TIMER(n) (0x40 + n)


int hook_id_timer = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if (timer < 0 || timer > 2) return 1;

  uint8_t current_configuration;

  timer_get_conf(timer, &current_configuration);

  uint8_t lsb4_current_configuration = (BIT(3) | BIT(2) | BIT(1) | BIT(0)) & current_configuration; //stores 4 LSBs of current configuration (so as to not change them)
  
  uint8_t control_word;

  switch (timer) { //timer selection
    case 0:
      control_word = TIMER_SEL0;
      break;
    
    case 1:
      control_word = TIMER_SEL1;
      break;

    case 2:
      control_word = TIMER_SEL2;
      break;
  }

  control_word = control_word | TIMER_LSB_MSB | lsb4_current_configuration; //final composition of control word

  if (sys_outb(TIMER_CTRL, control_word) != 0) return 1;
  uint16_t base = (TIMER_FREQ/freq);

  uint8_t base_lsb;
  uint8_t base_msb;

  if (util_get_LSB(base,&base_lsb) == 1) return 1;
  if (util_get_MSB(base,&base_msb) == 1) return 1;

  if (sys_outb(TIMER(timer), base_lsb) != 0) return 1;
  if (sys_outb(TIMER(timer), base_msb) != 0) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {

  if (bit_no == NULL) return 1;

  *bit_no = BIT(hook_id_timer);

  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer);
}

int (timer_unsubscribe_int)() {

  return sys_irqrmpolicy(&hook_id_timer);
}

void (timer_int_handler)() {

}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  if (timer < 0 || timer > 2 || st == NULL) {
    return 1;
  }

  uint8_t command = BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1);

  if (sys_outb(TIMER_CTRL, command) != 0) return 1;

  if (util_sys_inb(TIMER(timer), st) != 0) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  if (timer < 0 || timer > 2) return 1;

  union timer_status_field_val value;

  switch(field) {
    case tsf_all:
      value.byte = st;
      break;
    case tsf_initial:{
      uint8_t bits = (st & (BIT(4) | BIT(5))) >> 4;
      value.in_mode = bits;
      break;
    }
    case tsf_mode:{
        uint8_t bits = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;
        if (bits >= 6) {
          value.count_mode = bits - 4;
        } else {
          value.count_mode = bits;
        }
        break;
      }
    case tsf_base:
      value.bcd = st & BIT(0);
      break;
    default:
      break;
  }

  return timer_print_config(timer, field, value);
}
