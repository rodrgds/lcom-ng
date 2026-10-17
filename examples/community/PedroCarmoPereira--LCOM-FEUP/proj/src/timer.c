#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = 1;
int counter = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(hook_id);
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
  return 0;
}

int (timer_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_id);
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t (timer), uint8_t *(st)) {

  uint8_t cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  uint32_t tmp_st;

  sys_outb(TIMER_CTRL, cmd);

  switch(timer){
    case 0:
      sys_inb(TIMER_0, &tmp_st);
      break;
    case 1:
      sys_inb(TIMER_1, &tmp_st);
      break;
    case 2:
      sys_inb(TIMER_2, &tmp_st);
      break;
    default:
      tmp_st = 0;
      break;
  }
  util_get_LSB(tmp_st, st);
  return 0;

}

int (timer_display_conf)(uint8_t (timer), uint8_t (st),
                        enum timer_status_field (field)) {

 union timer_status_field_val conf;
  switch(field){
      case all:
        conf.byte = st;
        break;
      case initial:
        st &= BIT(5) | BIT(4);
        st >>= 4;
        conf.in_mode = st;
        break;
      case mode:
        st >>= 1;
        st &= BIT(2) | BIT(1) | BIT(0);
        if(st >= 6) st -= 4;
        conf.count_mode = st;
        break;
      case base:
        st &= BIT(0);
        conf.bcd = st;
      default:
        break;
  }

  timer_print_config(timer, field, conf);
  return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

	uint8_t st; //= malloc(sizeof(uint8_t *));
	timer_get_conf(timer, &st);
	(st) &= 0xF;
  (st) |= TIMER_LSB_MSB;
  uint8_t timer_tbc = 0;

  switch(timer){
    case 0:
    (st) |= TIMER_SEL0;
    timer_tbc = TIMER_0;
    break;
    case 1:
    (st) |= TIMER_SEL1;
    timer_tbc = TIMER_1;
    break;
    case 2:
    (st) |= TIMER_SEL2;
    timer_tbc = TIMER_2;
    break;
    default:
    break;
  }
    sys_outb(TIMER_CTRL, (st));
    uint8_t new_freq;
    util_get_LSB(TIMER_FREQ/freq, &new_freq);
    sys_outb(timer_tbc, new_freq);
    util_get_MSB(TIMER_FREQ/freq, &new_freq);
    sys_outb(timer_tbc, new_freq);

  return 1;
}
