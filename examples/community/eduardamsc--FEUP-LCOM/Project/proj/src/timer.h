#ifndef __TIMER_H
#define __TIMER_H

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */


/**
 * @brief Subscribes and enables timer 0 interrupts
 * @return Bit order in interrupt mask, 1 upon failure to subscribe
 */
int timer_subscribe_int();

/**
 * @brief Unsubscribes timer
 * @return 1 upon failure to unsubscribe
 */
int timer_unsubscribe_int();

#endif /* __TIMER_H */
