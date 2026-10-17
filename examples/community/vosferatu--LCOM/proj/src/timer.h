#ifndef __TIMER_H
#define __TIMER_H

/** @defgroup timer timer
 * @{
 * 	timer functions
 */



#define BITMASK_001 		0x01 /**< @brief bitmask 00000001 */
#define BITMASK_LSB_MSB 	0x30 /**< @brief bitmask for most/least significant bit */
#define BITMASK_MODE 		0x0E  /**< @brief bitmask for the timer mode */
#define BITMASK_MODE_0  	0x00 /**< @brief bitmask for timer mode 0 */
#define BITMASK_MODE_1  	0x02 /**< @brief bitmask for timer mode 1 */
#define BITMASK_MODE_4  	0x08 /**< @brief bitmask for timer mode 4 */
#define BITMASK_MODE_5  	0x0A /**< @brief bitmask for timer mode 5 */
#define BITMASK_4LSB    	0X0F /**< @brief bitmask to get the 4 least significant bits */

/*! Represents the timer */
typedef struct{
	int ticked;
	unsigned long counter;
} Timer;

/**
 * @brief creates a new timer; allocates it
 * @return a pointer to a timer type
 */
Timer* newTimer();

/**
 * @brief frees a timer from memory
 * @param timer a pointer to a timer type
 */
void deleteTimer(Timer* timer);

/**
 * @brief resets ticks to 0
 * @param timer a pointer to a timer type
 */
void resetTimerTickedFlag(Timer* timer);

/**
 * @brief counts timer interrupts; activates a flag to refresh screen
 * @param timer a pointer to a timer type
 */
void timerHandler(Timer* timer);

/**
 * @brief function to add a second to time, if a second has passed
 * @param time pointer to variable location
 */
void timed_scan_handler(unsigned short *time);

/**
 * @brief resets global variable used by the timer
 */
void reset_counter_handler();

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
void timer_int_handler();

/**
 * @brief Tests Timer 0 interrupt handling
 *
 * Subscribes Timer 0 interrupts and prints a message once
 *  per second for the specified time interval
 *
 * @param time Length of time interval while interrupts are subscribed
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_test_int(unsigned long time);

/**
 * @brief Reads the input timer configuration via read-back command
 *
 * @param timer Timer whose config to read (Ranges from 0 to 2)
 * @param st    Address of memory position to be filled with the timer config
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_get_conf(unsigned long timer, unsigned char *st);

/**@}*/

#endif /* __TIMER_H */
