/**
 * @file mouseView.c
 * @brief Mouse view implementation
 * 
 * This file contains the implementation of mouse view functions
 * for rendering the mouse cursor on the screen.
 */

#include "mouseView.h"

/**
 * @brief Draws the mouse cursor on the screen
 * 
 * This function renders the mouse cursor sprite at its current position
 * using the graphics system.
 * 
 * @param m Mouse object containing position and sprite data
 * @return 0 on success, non-zero on error
 */
int drawMouse(Mouse m) {
    if (draw_Sprite(m.sprite, m.pos.x, m.pos.y) != 0) return 1;
    return 0;
}
