#ifndef __COUNT_DOWN_H
#define __COUNT_DOWN_H

#include <lcom/lcf.h>
#include "../drivers/graphics/graphics.h"

/**
 * @brief Starts a counter.
 * 
 * @param time The time of the counter.
 * @return int Returns 1 if the time is lower than 0, 0 otherwise.
 */
int start_counter(int time);

/**
 * @brief Decrements the counter by one.
 * 
 * @return int Returns 1 if the time has ended, 0 otherwise.
 */
int decrement_counter();

/**
 * @brief Sets the position of the timer on the screen.
 * 
 * @param x The horizontal position of the timer on the screen.
 * @param y The vertical position of the timer on the screen.
 */
void set_timer_pos(int x, int y);

/**
 * @brief Draws the timer on the screen.
 * 
 * @return int Returns 1 if an error occurs while drawing the numbers, 0 otherwise.
 */
int draw_timer();

/**
 * @brief Clears the timer from the screen.
 * 
 * @return int Returns 1 if an error occurs while clearing the numbers, 0 otherwise.
 */
int clear_timer();

#endif
