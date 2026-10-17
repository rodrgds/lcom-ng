#ifndef NUMBERS_VIEW_H
#define NUMBERS_VIEW_H

#include <lcom/lcf.h>

#define DIGIT_WIDTH 25
#define DIGIT_HEIGHT 28
#define DIGIT_SPACING 0

/**
 * @brief Draws an unsigned integer on screen using digit XPMs.
 *
 * Converts the given number to its decimal representation and
 * renders each digit at the specified position, using the
 * predefined digit width, height, and spacing.
 *
 * @param number The unsigned integer to draw
 * @param x      X coordinate of the first digit (top-left corner)
 * @param y      Y coordinate of the first digit (top-left corner)
 */
void draw_number(uint32_t number, int x, int y);

#endif
