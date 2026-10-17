#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

/**
 * @brief Function that sets the frequency of the timer.
 * @param timer Timer to set the frequency.
 * @param freq Frequency to set the timer.
 * @return 0 if the frequency was successfully set, 1 otherwise.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Function that subscribes the timer interruptions.
 * @param bit_no Variable to store the bit mask of the timer interruptions.
 * @return 0 if the timer interruptions were successfully subscribed, 1 otherwise.
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Function that unsubscribes the timer interruptions.
 * @return 0 if the timer interruptions were successfully unsubscribed, 1 otherwise.
 */
int (timer_unsubscribe_int)();

/**
 * @brief Function that increments a global variable that represents the number of timer interruptions.
 */
void (timer_int_handler)();

/**
 * @brief Function that gets the configuration of the timer.
 * @param timer Timer to get the configuration.
 * @param st Variable to store the configuration.
 * @return 0 if the configuration was successfully obtained, 1 otherwise.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

#endif
