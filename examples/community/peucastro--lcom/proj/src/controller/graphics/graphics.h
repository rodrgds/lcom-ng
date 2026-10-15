#ifndef __PROJ_GRAPHICS_H
#define __PROJ_GRAPHICS_H

#include <stdint.h>

#include "controller/graphics/vbe.h"

/** @defgroup graphics graphics
 * @{
 *
 * Functions for using the PC's video card in Graphics Mode.
 */

/**
 * @brief Extracts the Red component from a 32 bit color.
 *
 * @param color a 32 bit color
 * @return The Red component of the color
 */
uint32_t(extract_red)(uint32_t color);

/**
 * @brief Extracts the Green component from a 32 bit color.
 *
 * @param color a 32 bit color
 * @return The Green component of the color
 */
uint32_t(extract_green)(uint32_t color);

/**
 * @brief Extracts the Blue component from a 32 bit color.
 *
 * @param color a 32 bit color
 * @return The Blue component of the color
 */
uint32_t(extract_blue)(uint32_t color);

/**
 * @brief Draws a pixel in the screen.
 *
 * @param x Horizontal axis coordinate
 * @param y Vertial axis coordinate
 * @param color Color to set the pixel
 * @return Return 0 upon success, non-zero otherwise
 */
int(graphics_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line on the screen.
 *
 * @param x Horizontal axis coordinate of the initial point
 * @param y Vertical axis coordinate of the initial point
 * @param len Line's length in pixels
 * @param color Color to draw the line's pixels
 * @return Return 0 upon success, non-zero otherwise
 */
int(graphics_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a filled rectangle on the screen.
 *
 * @param x Horizontal axis coordinate of the initial point
 * @param y Vertical axis coordinate of the initial point
 * @param width Rectangle's width in pixels
 * @param height Rectangle's height in pixels
 * @param color Color to set the rectangle's pixels
 * @return Return 0 upon success, non-zero otherwise
 */
int(graphics_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a matrix of colored rectangles on the screen.
 *
 * @param mode Video mode to set
 * @param no_rectangles Number of rectangles per row and column
 * @param first Color of the first rectangle
 * @param step Step value to calculate the color of subsequent rectangles
 * @return Return 0 upon success, non-zero otherwise
 */
int(graphics_draw_matrix)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief Draws a pixmap (XPM image) on the screen at the specified coordinates.
 *
 * @param xpm Pixmap to be drawn
 * @param x Horizontal axis coordinate of the upper left corner of the pixmap
 * @param y Vertical axis coordinate of the upper left corner of the pixmap
 * @return Return 0 upon success, non-zero otherwise
 */
int(graphics_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Clears the entire screen.
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int(graphics_clear_screen)(void);

/**@}*/

#endif /* __PROJ_GRAPHICS_H */
