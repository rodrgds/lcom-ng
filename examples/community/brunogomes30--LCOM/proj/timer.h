#ifndef TIMER_H
#define TIMER_H
  
/** @defgroup timer 
 * @{
 *
 * Functions related to timer 
 */

/**
 * @brief Set timer frequency
 * 
 */
int(timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribe to timer interrupts
 * 
 */
int(timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribe from timer interrupts
 * 
 */
int(timer_unsubscribe_int)();

/**
 * @brief Get configuration of the given timer
 * 
 */

int(timer_get_conf)(uint8_t timer, uint8_t *st);

#endif

