#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "macro_timer.h"

static int timer_hook_id = TIMER0_IRQ;
int timer_counter = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = timer_hook_id;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK) return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
  timer_counter++;
}
