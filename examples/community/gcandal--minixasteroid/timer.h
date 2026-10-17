#ifndef __TIMER_H
#define __TIMER_H

static int timerhook=2;

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Return 0 upon success
 */
int timer_subscribe_int(void );

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success
 */
int timer_unsubscribe_int();

#endif /* __TIMER_H */
