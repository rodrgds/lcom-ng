#pragma once 

#include <stdbool.h>
#include <stdint.h>
#include "snake.h" // For Point and snake_length/snake_body
#include "controller/video/graphics.h" 
#include "pixmap.h"

extern Point apple_position;
extern bool apple_eaten;

/**
 * @brief Generates a random position for the apple that doesn't overlap with the snake's body
 */
void generate_apple_position();

/**
 * @brief Draws the apple at its current position using an XPM image
 */
void draw_apple();

/**
 * @brief Clears the apple from the screen 
 */
void clear_apple();
