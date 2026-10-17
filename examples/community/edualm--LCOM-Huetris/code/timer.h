#ifndef __TIMER_H
#define __TIMER_H

/** @defgroup timer timer
 * @{
 * Functions for controlling the timer.
 */

/**
 * @brief Handler for the timer interrupts.
 */

void timer_int_handler();

/**
 * @brief Subscribes the timer interrupts.
 *
 * @return 0 on success, undefined if otherwise.
 */

int timer_subscribe();

/**
 * @brief Unsubscribes the timer interrupts.
 *
 * @return 0 on success, undefined if otherwise.
 */

int timer_unsubscribe();

/**@}*/

#endif /* __TIMER_H */
