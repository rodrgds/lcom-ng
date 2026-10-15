#ifndef TIMER_COUNT_H
#define TIMER_COUNT_H

#include <lcom/lcf.h>

/** 
 * @file timer_count.h
 * @brief Timer count management
 * 
 * This module provides functions to manage the timer count in the game.
 * It allows getting the current count value and resetting it to zero.
 */


/**
 * @brief Returns the current count value
 * 
 * @return The current count value
 */
unsigned int timer_get_count();

/**
 * @brief Sets the count value to 0
 */
void timer_reset_count();

#endif
