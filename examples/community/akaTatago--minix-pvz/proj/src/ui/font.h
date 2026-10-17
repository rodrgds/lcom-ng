/**
 * @file font.h
 * @brief Bitmap font rendering.
 */

#ifndef __FONT_H
#define __FONT_H

#include <stdint.h>

/**
 * @brief Draws a single character on the screen.
 * * @param c The character to draw.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @param color The color to draw the character in (hex format).
 */
void font_draw_char(char c, int x, int y, uint32_t color);

/**
 * @brief Draws a string on the screen.
 * * @param str The string to draw.
 * @param x The starting x-coordinate.
 * @param y The starting y-coordinate.
 * @param color The color of the text (hex format).
 */
void font_draw_string(const char *str, int x, int y, uint32_t color);

/**
 * @brief Draws an integer number on the screen.
 * * @param num The number to draw.
 * @param x The starting x-coordinate.
 * @param y The starting y-coordinate.
 * @param color The color of the number (hex format).
 */
void font_draw_number(int num, int x, int y, uint32_t color);

#endif
