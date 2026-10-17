#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#define TIMER0_IRQ 0

/**
 * @defgroup timer
 * @{
 * @brief Functions of the Timer
 */

/**
* @brief Subscribes and enables Timer 0 interrupts.
* @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
* @return Return 0 upon success and non-zero otherwise
*/
int (timer_subscribe_int)(uint8_t *bit_no);

/**
* @brief Unsubscribes Timer 0 interrupts.
* @return Return 0 upon success and non-zero otherwise
*/
int (timer_unsubscribe_int)();

/**
* @brief Timer 0 interrupt handler.
*
* @brief Increments counter
*/
void (timer_int_handler)();

/** @} end of timer */

#endif //TIMER_H
