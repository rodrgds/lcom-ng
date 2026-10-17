#include <lcom/lcf.h>
#include <stdint.h>
#include "timer.h"
#include "i8042.h"


int timer_hook_id;
int timer_interrupt_counter = 0;


int (timer_subscribe_int)(uint8_t *bit_no) {

  timer_hook_id = TIMER0_IRQ;
  *bit_no = timer_hook_id;  

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) == OK) {
    return 0;
  }

  else {
    printf("sys_irqsetpolicy() failed\n");
    return 1;    
  }

}
int (timer_unsubscribe_int)() {

  if (sys_irqrmpolicy(&timer_hook_id) != OK) {
    printf("sys_irqrmpolicy() failed\n");
    return 1;
  }

  return 0;
}

void (timer_int_handler)() {
 timer_interrupt_counter++;
}
