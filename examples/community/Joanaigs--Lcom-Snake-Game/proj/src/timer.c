#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"


int hook_id=0;
int n_interrupts = 0;

int (timer_subscribe)(uint8_t *bit_no, int hook) {
    hook_id=hook;
    /* To be implemented by the students */
     *bit_no=BIT(hook_id);
    return sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

