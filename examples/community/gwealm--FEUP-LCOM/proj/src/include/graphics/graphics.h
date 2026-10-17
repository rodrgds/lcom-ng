#pragma once

#include <lcom/lcf.h>
#include "errors.h"

#include "vector/vector.h"

#include "game/model/cell_background.h"

#define GRAPHICS_ALFA_WIDTH 64
#define GRAPHICS_ALFA_HEIGHT 64

#define GRAPHICS_NUM_WIDTH 32
#define GRAPHICS_NUM_HEIGHT 32

/**
 * @brief Handles operations to inicialize the graphics for usage
 *
 * @return 0 if the operation was successful, non-zero otherwise 
 */
int (graphics_init)();

/**
 * @brief Handles operations to terminate the graphics usage
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (graphics_cleanup)();

/**
 * @brief Draws a character on the VBE
 * 
 * @param x The top-leftest pixel's x position
 * @param y the top-leftest pixel's y position
 * @param ch The character to draw (in lower-case)
 * @return 0 if the operation was successful, non-zero otherwise
 */
void (graphics_draw_char)(uint16_t x, uint16_t y, char ch);

/**
 * @brief Draws a string of characters on the VBE
 * 
 * @param x The top-leftest pixel's x position
 * @param y the top-leftest pixel's y position
 * @param str The string to draw (in lower-case)
 * @return 0 if the operation was successful, non-zero otherwise
 */
void (graphics_draw_text)(uint16_t x, uint16_t y, char str[]);

/**
 * @brief Draws small text
 * 
 * @param x x coordinate
 * @param y y coordinate
 * @param str text to draw
 */
void (graphics_draw_small_text)(uint16_t x, uint16_t y, char str[]);

/**
 * @brief Draws a string of numbers on the VBE
 * 
 * @param x The top-leftest pixel's x position
 * @param y the top-leftest pixel's y position
 * @param str The string to draw (digits only)
 * @return 0 if the operation was successful, non-zero otherwise
 */
void (graphics_draw_numbers)(uint16_t x, uint16_t y, char str[]);

/**
 * @brief Draws a board cell on the VBE
 * 
 * @param x The top-leftest pixel's x position
 * @param y the top-leftest pixel's y position
 * @param background The cell's background 
 * @return 0 if the operation was successful, non-zero otherwise
 */
void (graphics_draw_square)(uint16_t x, uint16_t y, cell_background background);

/**
 * @brief Draws the whole game to the VBE
 * 
 */
// void (graphics_draw_game)();

/**
 * @brief Draws the content displayed onthe screen to the VBE
 * 
 */
void (graphics_draw_screen)();
