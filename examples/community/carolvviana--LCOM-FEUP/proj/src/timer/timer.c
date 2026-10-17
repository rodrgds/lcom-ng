#include <lcom/lcf.h>

#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int hook_id_tmr = 0;

void (timer_int_handler)() {
  counter++;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id_tmr) != 0) return 1;
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(hook_id_tmr);
  if(sys_irqsetpolicy(TMR0_IRQ, IRQ_REENABLE, &hook_id_tmr)== 0) return 0;

  return 1;
}
