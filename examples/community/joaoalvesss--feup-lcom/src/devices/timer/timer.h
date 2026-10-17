/**
 * @file timer.h
 * @brief file that has functions to allow configurating the timers
 * 
 */

#ifndef _TIMER_
#define _TIMER_

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

/**
 * @brief set the frequency of a timer
 * @param timer that we want to configurate 
 * @param freq frequency that we want to set in the timer
 * @return 0 if success otherwise 1
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);


/**
 * @brief function to subscribe timer interruptions 
 * @return 0 if success otherwise 1
 */
int (timer_subscribe_interrupts)();


/**
 * @brief function to unsubscribe timer interruptions
 * @return 0 if success otherwise 1
 */
int (timer_unsubscribe_interrupts)();

/**
 * @brief function to get the current timer configuration
 * @param timer which timer to get the configuration
 * @param st timer status
 * @return 0 if success otherwise 1
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);


#endif /* _TIMER_ */
