#include <lcom/lcf.h>
/** @defgroup timer timer.h
 * @{
 *
 * This file contains the functions related to the timer functioning.
 * 
 */

/**
 * @brief Subscribes timer interrupts
 * 
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes timer interrupts
 * 
 */
int (timer_unsubscribe_int)();

/**
 * @brief Handles timer interrupts
 * 
 */
void (timer_ih)();

/**
 * @brief Uses timer interrupts to count the time
 * 
 */
void update_game_elapsed_time();

/**
 * @brief Resets the timer count
 * 
 */
void reset_game_elapsed_time();
/**@}*/
