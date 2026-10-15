#ifndef _TIMER_H_
#define _TIMER_H_

/**
 * @file timer.h
 * @brief Timer interface functions.
 */


#include <lcom/lcf.h>

/** @defgroup timer timer
 * @{
 */

/**
 * @brief Sets the frequency of a timer.
 *
 * @param timer Timer to configure (0, 1, or 2).
 * @param freq Frequency to set.
 * @return 0 on success, non-zero value on failure.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);


/**
 * @brief Gets the configuration of a timer.
 *
 * @param timer Timer to get the configuration from (0, 1, or 2).
 * @param status Pointer to a variable where the configuration status will be stored.
 * @return 0 on success, non-zero value on failure.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *status);
/**
 * @brief Subscribes to timer interrupts.
 *
 * @param timer_irq Pointer to a variable where the timer IRQ bit mask will be stored.
 * @return 0 on success, non-zero value on failure.
 */
int (timer_subscribe_interrupt)(uint32_t *timer_irq);
/**
 * @brief Unsubscribes from timer interrupts.
 *
 * @return 0 on success, non-zero value on failure.
 */
int (timer_unsubscribe_int)();

/**
 * @brief Timer interrupt handler.
 */
void (timer_int_handler)();

/** @} */ // end of Timer group


#endif
