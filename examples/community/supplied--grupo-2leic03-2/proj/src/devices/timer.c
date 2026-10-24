#include <lcom/lcf.h>
#include <stdint.h>

#include "timer.h"

#define TIMER0_IRQ 0

static int timer_hook_id = TIMER0_IRQ;

int project_timer_subscribe(uint32_t *irq_set) {
  if (irq_set == NULL) {
    return 1;
  }

  timer_hook_id = TIMER0_IRQ;
  *irq_set = BIT(timer_hook_id);

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK) {
    return 1;
  }

  return 0;
}

int project_timer_unsubscribe(void) {
  if (sys_irqrmpolicy(&timer_hook_id) != OK) {
    return 1;
  }

  return 0;
}

