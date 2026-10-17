#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include "snake.h"
#include "controller/video/graphics.h"
#include "../pixmap.h"
#include "game.h"

#define TONGUE_DURATION 10

extern bool show_tongue;
extern int tongue_timer;
extern bool tongue_button_held;

/**
 * @brief Initializes the tongue state and resets all related variables
 */
void tongue_init(void);

/**
 * @brief Resets the tongue state and hides it
 */
void tongue_reset(void);

/**
 * @brief Updates the tongue's visibility
 * 
 */
void tongue_update(void);

/**
 * @brief Draws the tongue in front of the snake's head depending on its direction
 * 
 * @param screen_width Width of the screen
 * @param screen_height Height of the screen
 */
void tongue_draw(uint16_t screen_width, uint16_t screen_height);

/**
 * @brief Updates the tongue's state based on mouse button input
 * 
 * @param left_button_pressed True if the mouse button is currently pressed
 */
void tongue_handle_mouse(bool left_button_pressed);

/**
 * @brief Calculates the position of the snake's tongue based on the current direction
 * 
 * @param head_pos The current position of the snake's head
 * @param dir The direction the snake is currently moving
 * @param width The width of the game screen
 * @param height The height of the game screen
 * 
 * @return Point representing the position of the tongue
 */
Point tongue_direction(Point head_pos, Direction dir, uint16_t width, uint16_t height);
