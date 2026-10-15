/**
 * @file cursor_input
 * @brief This file has the implementation of the functions related to the cursor input.
 */

#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdbool.h>
#include <lcom/lcf.h>
#include "../cursor/cursor.h"

/**
 * @brief Struct that represents the state of the cursor.
 *
 * This struct contains the state of the cursor, including the state of left, right, 
 * and middle clicks, as well as the cursor's position on the x and y axes.
 */
typedef struct {

    bool leftClick;
    bool rightClick;
    bool middleClick;
    int x;
    int y;
    
} CursorInput;

/**
 * @brief Reads the current cursor input.
 * 
 * @param packet A pointer to the packet structure to be filled with the mouse data.
 *
 * This function reads the current cursor input by processing the mouse packet data. 
 * It calculates the cursor's position and reads the state of the left, right, and middle clicks. 
 * The calculated information is stored and returned in a CursorInput structure.
 * 
 * @return Returns the current cursor input as a CursorInput structure.
 */
CursorInput (read_cursor_input)(struct packet *packet);

#endif
