/**
 * @file timer
 * @brief Functions for using the i8254 timers
 */

#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

/** @defgroup Timer Timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @brief Set the timer frenquency
 *
 * @param timer The timer to change the frenquency
 * @param freq The value of the frequency to be set
 * @return Return 0 upon success and 1 otherwise
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes timer interrupt
 *
 * @param bit_no Bit number of timer IRQ
 * @return Return 0 upon success and 1 otherwise
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes timer interrupt
 * @return Return 0 upon success and 1 otherwise
 */
int (timer_unsubscribe_int)();

/**
 * @brief Timer interrupt handler
 */
void (timer_ih)();

/**
 * @brief Reads the timer configuration
 *
 * @param timer The timer to read the configuration
 * @param st The configuration obtained from the timer
 * @return Return 0 upon success and 1 otherwise
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays the timer configuration
 *
 * @param timer The timer to display the configuration
 * @param st The configuration from the timer
 * @param field The field of the configuration to display
 * @return Return 0 upon success and 1 otherwise
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

/**@}*/

#endif
