#ifndef TIMER_H
#define TIMER_H


#include <stdint.h>
/**
 * @brief Subscribes and enables Timer 0 interrupts.
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * 
 * @return 0 upon success, one otherwise.
*/
int(timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Timer 0 interrupts.
 * 
 * @return 0 upon success, one otherwise.
*/
int(timer_unsubscribe_int)();

/**
 * @brief Timer 0 interrupt handler.
 * 
 * Sums the elapsed time.
*/
void(timer_int_handler)();

/**
 * @brief Gets the elapsed time.
 * 
 * @return The elapsed time.
*/
int get_elapsed();

/**
 * @brief Gets the configuration of a timer.
 * 
 * @param timer Timer to get the configuration of.
 * @param st Address of memory to be initialized with the configuration.
 * 
 * @return 0 upon success, one otherwise.
*/
int(timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Sets the frequency of a timer.
 * 
 * @param timer Timer to set the frequency of.
 * @param freq Frequency to set.
 * 
 * @return 0 upon success, one otherwise.
*/
int(timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Resets the elapsed time.
*/
void reset_elapsed();

#endif
