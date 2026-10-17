/**
 * This module contains all functions to manage timer
 * Jorge Vale: 50%
 * Telmo Barros: 50%
 * Relative Weight: 4%
 */
#ifndef __TIMER_H
#define __TIMER_H

/** @defgroup Timer Timer
 * @{
 * Functions to manage timer
 */

/**
 * @brief Subscribes the timer interrupts
 *
 * @return Return timer ID to verify the interrupts and -1 upon failure
 */
int timer_subscribe_int(void );

/**
 * @brief Unsubscribes the timer interrupts
 *
 * @return Return 0 upon success and 1 upon failure
 */
int timer_unsubscribe_int();

/**
 * @brief Sets the frequency of the timer to a desired value
 *
 * @param timer Timer select (value from 1 to 3 that corresponds the three timers)
 * @param freq Frequency value to set
 *
 * @return Return 0 upon success and 1 upon failure
 */
int timer_set_square(unsigned long timer, unsigned long freq);

/**
 * @brief Gets timer configuration
 *
 * @param timer Timer select (value from 1 to 3 that corresponds the three timers)
 * @param st Value to change with the configuration of the timer
 *
 * @return Return 0 upon success and 1 upon failure
 */
int timer_get_conf(unsigned long timer, unsigned char *st);

/**@}*/


#endif /* __TIMER_H */
