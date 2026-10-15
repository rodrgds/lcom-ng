#include "timer.h"

int hook_id_timer = 0;
int timer_counter = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id_timer);

  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer);
}

int (timer_unsubscribe_int)() {
  if (hook_id_timer < 0)
    return 1;
  return sys_irqrmpolicy(&hook_id_timer);
}

void (timer_int_handler)() {
  timer_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (!st || timer < 0 || timer > 2)
    return 1;

  uint8_t cmd = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT;
  int retv = sys_outb(TIMER_CTRL, cmd);
  if (retv)
    return 1;

  return util_sys_inb(TIMER_0 + timer, st);
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (timer < 0 || timer > 2)
    return 1;

  union timer_status_field_val conf;
  uint8_t mode = 0;

  switch (field) {
    case tsf_all:
      conf.byte = st;
      break;
    case tsf_initial:
      mode = (st & (BIT(4) | BIT(5))) >> 4;
      switch (mode) {
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
          break;
      }
      break;
    case tsf_mode:
      mode = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;
      if (mode == 6 || mode == 7)
        conf.count_mode = mode - 4;
      else
        conf.count_mode = mode;
      break;
    case tsf_base:
      conf.bcd = st & BIT(0);
      break;
    default:
      return 1;
  }

  return timer_print_config(timer, field, conf);
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st, msb, lsb;

  if (timer < 0 || timer > 2)
    return 1;

  if (freq > TIMER_FREQ)
    return 1;

  if (timer_get_conf(timer, &st))
    return 1;

  st = (st & 0x0F) | TIMER_LSB_MSB;
  util_get_LSB(TIMER_FREQ / freq, &lsb);
  util_get_MSB(TIMER_FREQ / freq, &msb);

  switch (timer) {
    case 0:
      st |= TIMER_SEL0;
      break;
    case 1:
      st |= TIMER_SEL1;
      break;
    case 2:
      st |= TIMER_SEL2;
      break;
  }

  return sys_outb(TIMER_CTRL, st) || sys_outb(TIMER_0 + timer, lsb) || sys_outb(TIMER_0 + timer, msb);
}
