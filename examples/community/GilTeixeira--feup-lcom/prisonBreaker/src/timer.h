#ifndef __TIMER_H
#define __TIMER_H

#include "i8254.h"

#define DEBUG 0

#define IRQ_SET_ERROR 2
#define IRQ_ENAB_ERROR 3
#define IRQ_DISAB_ERROR 4
#define IRQ_REM_ERROR 5
#define TIMER_CONF_ERROR 6
#define SYS_OUT_ERROR 7
#define SYS_IN_ERROR 8
#define TIMER_SUB_ERROR 9
#define TIMER_UNSUB_ERROR 10
#define TIMER_GET_CONF_ERROR 11
#define TIMER_DISPLAY_CONF_ERROR 12
#define FREQ_NEG_ERROR 13
#define FREQ_OVERFLOW_ERROR 14
#define INPUT_NEG_ERROR 15

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @struct Timer
 * @brief This structure holds time passed in form of seconds and ticks of the Timer
 */

typedef struct {
	long ticks;
	long counter;
} Timer;

/**
 * @brief Initializes the Timer
 *
 * @return The new Timer
 */
Timer* initTimer();

/**
 * @brief Processes an interrupt
 *
 * @param timer The Timer to be handled
 */
void timerHandler(Timer* timer);

/**
 * @brief Free the Timer and all the resources used by it
 *
 * @param timer The Timer to be freed
 */
void stopTimer(Timer* timer);

/**
 * @brief Reset The Timer Settings
 *
 * @param timer The Timer to be reset
 */
void resetTimer(Timer* timer);

/**
 * @brief print error message when DEBUG is activated (= 1)
 *
 * @param msg The message to print
 */
void printErrors(char * msg);

/**
 * @brief Changes the operating frequency of a timer
 * 
 * Must not change the 3 LSBs (mode and BCD/binary) of the timer's control word.
 * @param timer Timer to configure. (Ranges from 0 to 2)
 * @param freq Timer operating frequency
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_set_frequency(unsigned char timer, unsigned long freq);

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int(void);

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
 * @brief Reads the input timer configuration via read-back command
 *
 * @param timer Timer whose config to read (Ranges from 0 to 2)
 * @param st    Address of memory position to be filled with the timer config
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_get_conf(unsigned char timer, unsigned char *st);


/** @} end of timer */

#endif /* __TIMER_H */
