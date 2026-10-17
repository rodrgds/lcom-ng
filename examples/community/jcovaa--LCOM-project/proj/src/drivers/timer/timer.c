#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "drivers/timer/i8254.h"

int timer_hook_id = 0;
int timer_counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 19) {
    return 1;
  }

  uint8_t control;
  if (timer_get_conf(timer, &control) != 0) {
    return 1;
  }

  control = (control & 0x0F) | TIMER_LSB_MSB;

  uint32_t value = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  util_get_LSB(value, &lsb);
  util_get_MSB(value, &msb);

  uint8_t timer_port;
  switch (timer) {
    case 0:
        control |= TIMER_SEL0;
        timer_port = TIMER_0;
        break;
    case 1:
        control |= TIMER_SEL1;
        timer_port = TIMER_1;
        break;
    case 2:
        control |= TIMER_SEL2;
        timer_port = TIMER_2;
        break;
    default:
        return 1;
  }

  if (sys_outb(TIMER_CTRL, control) != 0) {
    return 1;
  }

  if (sys_outb(timer_port, lsb) != 0) return 1;
  if (sys_outb(timer_port, msb) != 0) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  // mask
  *bit_no = BIT(timer_hook_id);

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0)
    return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id) != 0) {
    return 1;
  }

  return 0;
}

void (timer_int_handler)() {
  timer_counter++;
}


int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2 || timer < 0) {
    return 1;
  }

  // BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1)
  uint8_t command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, command) != 0) {
    return 1;
  }
  if (util_sys_inb(TIMER_0 + timer, st)) {
    return 1;
  }
  return 0;
}
