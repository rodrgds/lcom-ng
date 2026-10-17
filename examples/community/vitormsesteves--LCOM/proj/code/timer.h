#ifndef __TIMER_H
#define __TIMER_H

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

typedef struct {
	int counter; // specifies a counter
	int ticked; // specifies if a timer interrupt is ticked
} Timer;

/**
 * @brief Creates a new Timer
 *
 * @return Non NULL pointer to the timer
 */
Timer* newTimer();
/**
 * @brief Reset ticked flag
 *
 * @param timer Pointer to the timer
 */
void resetTimer(Timer* timer);

/**
 * @brief Delete timer
 *
 * @param timer Pointer to the timer
 */
void deleteTimer(Timer* timer);


/**
 * @brief Configures a timer to generate a square wave
 * 
 * Does not change the LSB (BCD/binary) of the timer's control word.
 * 
 * @param timer Timer to configure. (Ranges from 0 to 2)
 * @param freq Frequency of the square wave to generate
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_set_square(unsigned long timer, unsigned long freq);

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int(void );

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe_int();

/**
 * @brief Timer 0 interrupt handler
 *
 * Increments counter
 */
void timer_handler(Timer* timer);



#endif /* __TIMER_H */
