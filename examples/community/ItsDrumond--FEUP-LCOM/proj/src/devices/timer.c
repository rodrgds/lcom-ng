#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

uint32_t timer_cnt = 0;
int timer_hook_id;

int (timer_subscribe_int)(uint8_t *bit_no) {
  timer_hook_id = *bit_no;
  return sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&timer_hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
  timer_cnt++;
}

