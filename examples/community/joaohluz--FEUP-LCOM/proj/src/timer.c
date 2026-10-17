#include <lcom/lcf.h>

#include <stdint.h>

#include "i8254.h"

int globalCounter = 0;
int globalHookIdTemp;

int (timer_subscribe_int)(uint8_t *bit_no) {

  int temp_hook = TEMP_HOOK;
  
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &temp_hook) != OK)
  	return 1;

  globalHookIdTemp = temp_hook;

 *bit_no = TEMP_HOOK;

  return 0;
}
//
int (timer_unsubscribe_int)() {
  
  if(sys_irqrmpolicy(&globalHookIdTemp) != OK)
  	return 1;

  return 0;
}

void (timer_int_handler)() {
  ++globalCounter;
}
