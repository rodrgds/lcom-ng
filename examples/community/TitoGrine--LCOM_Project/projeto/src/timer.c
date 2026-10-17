#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id_timer = 1;
int g_counter = 0;

/**
 * @brief Subscribes to the timer interrupts.
 * 
 * @param bit_no
 */
int (timer_subscribe_int)(uint8_t *(bit_no)) {
  
  *bit_no = hook_id_timer;
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer);

  return 0;
}

/**
 * @brief Unsubscribes the timers interrupts.
 * 
 */
int (timer_unsubscribe_int)() {
  
  sys_irqrmpolicy(&hook_id_timer);

  return 0;
}

/**
 * @brief Timer interrupt handler.
 * 
 */
void (timer_int_handler)() {
  g_counter ++;
}
