/**
 * @file cursor
 * @brief This file has the implementation of the functions related to the cursor.
 */

#ifndef _CURSOR_H_
#define _CURSOR_H_

/**
 * @brief Struct that represents a cursor.
 *
 * This struct contains the current and previous position of the cursor,
 * as well as its width and height.
 */
typedef struct {
    int x;
    int y;
    int old_x;
    int old_y;
    int width;
    int height;
} Cursor;

/**
 * @brief The cursor.
 *
 * This variable contains the cursor.
 */
Cursor* cursor;

/**
 * @brief Creates a new cursor.
 * 
 * @param x The initial x-coordinate of the cursor.
 * @param y The initial y-coordinate of the cursor.
 *
 * This function initializes a new cursor at the given coordinates with a predefined width and height.
 * 
 * @return void.
 */
void (cursor_create)(int x, int y);

/**
 * @brief Destroys the cursor.
 *
 * This function frees the memory allocated for the cursor.
 * 
 * @return void.
 */
void (cursor_destroy)();

/**
 * @brief Moves the cursor by the specified amount.
 * 
 * @param x The amount to move the cursor along the x-axis.
 * @param y The amount to move the cursor along the y-axis.
 *
 * This function moves the cursor by the specified amounts along the x and y axes. 
 * The function checks if the new position is within the screen resolution before moving the cursor.
 * 
 * @return void.
 */
void (cursor_move)(int x, int y);

#endif 
