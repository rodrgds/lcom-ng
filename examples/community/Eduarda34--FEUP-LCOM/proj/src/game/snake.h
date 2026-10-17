#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "controller/video/graphics.h"

// Define the size of each snake segment
#define SEGMENT_SIZE 18
// Define the spacing between segments in pixels
#define SEGMENT_SPACING 2
// Define initial snake length
#define INITIAL_SNAKE_LENGTH 3

#define CELL_SIZE (SEGMENT_SIZE + SEGMENT_SPACING) // Size of each grid cell

typedef struct {
    uint16_t x;
    uint16_t y;
} Point;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// Snake state
extern Point *snake_body;
extern int snake_length;
extern Direction current_direction;

/**
 * @brief Initializes the snake's position and direction based on the screen size
 * 
 * @param screen_width Width of the screen in pixels
 * @param screen_height Height of the screen in pixels
 */
void snake_init(uint16_t screen_width, uint16_t screen_height);

/**
 * @brief Draws the snake's body on the screen
 */
void snake_draw();

/**
 * @brief Updates the position of the snake's segments based on the current direction
 * 
 */
void snake_update();

/**
 * @brief Frees the memory allocated fot the snake's body
 */
void snake_free();

/**
 * @brief Changes the direction the snake will move in the next update
 * 
 * @param dir The new direction to be applied
 */
void snake_change_direction(Direction dir);

/**
 * @brief hecks whether the snake's head has collided with its own body
 * 
 * @return true if collision occurred
 * @return false otherwise
 */
bool snake_check_collision_with_self();

/**
 * @brief Checks whether the snake's head is at the same position as the apple
 * 
 * @param apple The current position of the apple
 * 
 * @return true if the snake ate the apple
 * @return false otherwise
 */
bool snake_check_collision_with_apple(Point apple);

/**
 * @brief Returns the current position of the snake's head
 * 
 * @return Point The coordinates of the snake's head
 */
Point snake_get_head_position();

/**
 * @brief Grows the snake by adding one more segment at the end
 */
void snake_grow();

/**
 * @brief Adjusts the snake's direction based on mouse movement deltas
 * 
 * @param delta_x Horizontal movement from the mouse
 * @param delta_y Vertical movement from the mouse
 */
void snake_handle_mouse_movement(int16_t delta_x, int16_t delta_y);
