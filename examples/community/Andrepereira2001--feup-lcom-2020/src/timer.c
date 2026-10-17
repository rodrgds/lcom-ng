#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer_hook_id;

/**
 * @brief subscribe interrupts of the timer
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return int 0 up on sucess
 */
int (timer_subscribe_int)(uint8_t *bit_no) {
  timer_hook_id = TIMER0_IRQ;
  *bit_no = timer_hook_id;
  if( sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&timer_hook_id) != 0){
    printf("sys_irqsetpolicy error!\n");
    return 1;
  }
  return 0;
}

/**
 * @brief unscribe interrupts of the timer
 * 
 * @return int o up on sucess
 */
int (timer_unsubscribe_int)() {
  if( sys_irqrmpolicy(&timer_hook_id) != 0){
    printf("sys_irqrmpolicy error!\n");
    return 1;
  } 
  return 0;
}
