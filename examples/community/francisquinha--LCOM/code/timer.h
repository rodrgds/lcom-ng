#ifndef __TIMER_H
#define __TIMER_H

/** @file timer.h
 * @brief file containing the function declarations to handle the timer
 */

/** @defgroup timer timer
 * @{
 *
 * @brief functions for using the i8254 timers
 */

/**@{
 *
 * @brief Timer info
 *
 * Timer info structure
 */
typedef struct {
	unsigned short hours;		/**< @brief Timer hours since start */
	unsigned short minutes;		/**< @brief Timer minutes since last hour */
	unsigned short seconds;		/**< @brief Timer seconds since last minute */
	unsigned short counter;		/**< @brief Timer counter since last second */
} Timer;
/** @} */

/**
 * @brief creates a new Timer struct
 * @return pointer to the created Timer struct
 */
Timer *create_timer();

/**
 * @brief Timer 0 interrupt handler
 *
 * Increments counter and when it makes sense, it also increments the seconds, minutes and/or hours
 *
 * @param timer timer used to handle the interruption
 */
void timer_int_handler(Timer *timer);

/**
 * @brief Timer reset function
 *
 * Resets all timer parameters to 0
 *
 * @param timer timer to reset
 */
void reset_timer(Timer *timer);

/**
 * @brief Timer penalize function
 *
 * Increments the number of minutes (and if necessary hours) of the timer
 *
 * @param timer timer to penalize
 * @param minutes number of minutes that will be incremented
 */
void penalize(Timer *timer, unsigned short minutes);

/** @} */

#endif /* __TIMER_H */
