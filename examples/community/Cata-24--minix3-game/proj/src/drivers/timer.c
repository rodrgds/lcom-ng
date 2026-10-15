#include <lcom/lcf.h>
#include "timer.h"
#include "../helper/i8254.h"

int timer_hook_id = 0;

int (timer_int_sub)(uint8_t *no_bit) {
    if (no_bit == NULL) return 1;
    *no_bit = BIT(timer_hook_id);
    return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);
}

int (timer_int_unsub)() {
    return sys_irqrmpolicy(&timer_hook_id);
}

int (timer_set_freq)(uint32_t frequency) {
    if (frequency < 19 || frequency > TIMER_FREQ) return 1;
  uint32_t FREQ = TIMER_FREQ / frequency;
  uint8_t msb, lsb;
  if (util_get_LSB(FREQ, &lsb) != 0) return 1;
  if (util_get_MSB(FREQ, &msb) != 0) return 1;

  uint8_t command;
  if (timer_get_config(&command) != 0) return 1;

  command = (command & 0xF) | TIMER_LSB_MSB | TIMER_SEL0;

  if (sys_outb(TIMER_CTRL, command) != 0) return 1;

  if (sys_outb(TIMER_0, lsb) != 0) return 1;
  if (sys_outb(TIMER_0, msb) != 0) return 1;

  return 0;
}

int (timer_get_config)(uint8_t *byte) {
  uint8_t command = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(0));
  if (sys_outb(0x43, command) != 0) return 1;
  return util_sys_inb(0x40, byte);
}
