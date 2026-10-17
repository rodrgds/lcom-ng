/**
 * @file draw_timer.h
 * @brief Interface for drawing and managing timer sprites.
 *
 * This header provides the structure and functions for creating, drawing, and destroying
 * timer sprites, including digits and colon, for displaying a timer on the screen.
 */
#ifndef __DRAW_TIMER_H
#define __DRAW_TIMER_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "assets/numbers.h"
#include "model/sprite.h"

/** 
 * @brief Holds the sprites used for drawing a timer (digits and colon).
 */
struct draw_timer;
typedef struct draw_timer draw_timer_t;

/**
 * @brief Creates timer sprites with digits and colon at given position.
 * 
 * @param x Initial x-coordinate for sprites.
 * @param y Initial y-coordinate for sprites.
 * @return draw_timer_t* Pointer to created timer sprites struct, or NULL on failure.
 */
draw_timer_t* create_timer(uint16_t x, uint16_t y);

/**
 * @brief Destroys the timer sprites and frees associated resources.
 * 
 * @param ts Pointer to draw_timer_t to destroy.
 */
void destroy_timer(draw_timer_t* ts);

/**
 * @brief Draws the timer (HH:MM:SS) at specified position.
 * 
 * @param ts Pointer to draw_timer_t containing digit and colon sprites.
 * @param hours Hours to display (0-23).
 * @param minutes Minutes to display (0-59).
 * @param seconds Seconds to display (0-59).
 * @param x Starting x-coordinate for drawing.
 * @param y Starting y-coordinate for drawing.
 * @return int 0 on success, non-zero on failure
 */
int draw_timer(draw_timer_t* ts, uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t x, uint16_t y);

/**
 * @brief Draws a single digit sprite at the specified position.
 * 
 * @param ts Pointer to draw_timer_t with digit sprites.
 * @param digit Digit to draw (0-9).
 * @param px X-coordinate to draw the digit.
 * @param py Y-coordinate to draw the digit.
 * @return int 0 on success, non-zero on failure
 */
int draw_digit(draw_timer_t* ts, uint8_t digit, uint16_t px, uint16_t py);

/**
 * @brief Draws the colon sprite at the specified position.
 * 
 * @param ts Pointer to timer_sprites_t with colon sprite.
 * @param px X-coordinate to draw the colon.
 * @param py Y-coordinate to draw the colon.
 * @return int 0 on success, non-zero on failure
 */
int draw_colon(draw_timer_t* ts, uint16_t px, uint16_t py);

#endif // __DRAW_TIMER_H
