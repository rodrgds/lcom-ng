/**
 * @file cursor.h
 * @brief Header file containing function prototypes for cursor manipulation.
 */

#ifndef CURSOR_H
#define CURSOR_H

#include <lcom/lcf.h>

/**
 * @brief Draws the cursor sprite at the specified coordinates.
 * @details This function draws the cursor sprite at the given (x, y) coordinates on the screen.
 * @param x The x-coordinate of the upper-left corner of the cursor sprite.
 * @param y The y-coordinate of the upper-left corner of the cursor sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_cursor(uint16_t x, uint16_t y);

/**
 * @brief Deletes the cursor sprite at the specified coordinates.
 * @details This function deletes the cursor sprite at the given (x, y) coordinates on the screen 
 * by drawing a black sprite over it.
 * @param x The x-coordinate of the upper-left corner of the cursor sprite.
 * @param y The y-coordinate of the upper-left corner of the cursor sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int delete_cursor(uint16_t x, uint16_t y);

/**
 * @brief Draws the target sprite at the specified coordinates.
 * @details This function draws the target sprite at the given (x, y) coordinates on the screen.
 * @param x The x-coordinate of the upper-left corner of the target sprite.
 * @param y The y-coordinate of the upper-left corner of the target sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_target(uint16_t x, uint16_t y);

/**
 * @brief Deletes the target sprite at the specified coordinates.
 * @details This function deletes the target sprite at the given (x, y) coordinates on the screen 
 * by drawing a black sprite over it.
 * @param x The x-coordinate of the upper-left corner of the target sprite.
 * @param y The y-coordinate of the upper-left corner of the target sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int delete_target(uint16_t x, uint16_t y);


#endif
