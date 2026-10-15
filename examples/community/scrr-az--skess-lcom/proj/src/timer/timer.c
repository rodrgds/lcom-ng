#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer_hook_id = 0;
int counter_timer = 0; //counter for how many times the timer signaled an interruption

/**
 * @brief Subscribes to timer interrupts.
 *
 * @param bit_no pointer to store the IRQ bit mask.
 * @return 0 on success or 1 if pointer is invalid or subscription fails.
 */
int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1; //check if pointer is valid
  *bit_no = BIT(timer_hook_id); //bit_no is now a mask

  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id); //subscribe to the interrupts on IRQ_0 and bit 0
}

/**
 * @brief Unsubscribes from timer interrupts.
 *
 * @return 0 on success.
 */
int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}
