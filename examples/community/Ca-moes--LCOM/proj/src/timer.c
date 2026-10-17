#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

uint32_t TIMER_COUNTER = 0;

int TIMER_HOOK_ID = TIMER0_IRQ; 
/*before calling sys_irqsetpolicy the variable represents a identifier associated with a specific diver - timer 0 in this case*/
/*after calling sys_irqsetpolicy, hook_id is used to specifiy the  interrupt notification to operate on*/

int(timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = TIMER_HOOK_ID;

  //subscribe a notification on every interrupt in the IRQ (interrupt request line) of Timer 0
  //IRQ_REENABLE indicates that the interrupt is going to be enabled by the generic interrupt handler 
  if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&TIMER_HOOK_ID) == OK){
    return 0;
  }
  else
    return 1;
}

int(timer_unsubscribe_int)() {
  //unsubscribe a previous subscription of a specific interrupt notification (the one associated with the HOOK_ID)
  if(sys_irqrmpolicy(&TIMER_HOOK_ID) == OK){
    return 0;
  }
  else
    return 1;
}

void(timer_int_handler)() {
    TIMER_COUNTER ++;
}
