#ifndef __TIMER_H
#define __TIMER_H
#include "i8254.h"
/** @defgroup Timer Timer

 * @{
 *
 * Functions for the i8254 timers, mainly used to set time limits in the game
 */
static int hook_id_timer = 0; //!< bit order for timer0 to be used in interupt mask

/**
 * @brief Configures a timer to generate a square wave
 *		  Used to generate frequencies used for sound emulation
 * @param timer Timer to configure. (Ranges from 0 to 2)
 * @param freq Frequency of the square wave tso generate
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_set_square(unsigned long timer, unsigned long freq);

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int();

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe_int();

#endif /* __TIMER_H */
