#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

// All the following functions were created during the labs

// Global Variables for Timer
int counter = 0;
int hook_id_timer = 1;

int(timer_subscribe_int)(uint8_t *bit_no) {
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer);
  return 0;
}

int(timer_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_id_timer);
  return 0;
}

void(timer_int_handler)() {
  counter++;
}
