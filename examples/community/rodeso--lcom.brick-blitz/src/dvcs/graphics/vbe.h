/**
 * @file vbe.h
 * @brief Header file for VBE functions.
 */

#pragma once
 
#include <stdint.h>
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Structure containing VBE mode information.
 */
vbe_mode_info_t vmi_p;

/**
 * @brief Sets the display mode to the specified mode.
 *
 * @param mode The mode to set.
 * @return Return 0 upon success and non-zero otherwise.
 */
int (vbe_set_display_mode)(uint16_t mode);

/**
 * @brief Maps video RAM to the address space for the specified mode.
 *
 * @param mode The mode to map.
 * @return Return 0 upon success and non-zero otherwise.
 */
int (vbe_mapping_videoRAM_to_address_space)(uint16_t mode);

/**
 * @brief Draws a pixel at the specified coordinates with the specified color.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * @return Return 0 upon success and non-zero otherwise.
 */
int (vbe_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);  

/**
 * @brief Draws a line starting from the specified coordinates with the specified length and color.
 *
 * @param x The x-coordinate of the starting point of the line.
 * @param y The y-coordinate of the starting point of the line.
 * @param len The length of the line.
 * @param color The color of the line.
 * @return Return 0 upon success and non-zero otherwise.
 */
int (vbe_draw_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle starting from the specified coordinates with the specified width, height, and color.
 *
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 * @return Return 0 upon success and non-zero otherwise.
 */
int (vbe_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a pattern of rectangles with the specified mode, number of rectangles, first color, and step.
 *
 * @param mode The mode to use.
 * @param no_rectangles The number of rectangles to draw.
 * @param first The first color of the pattern.
 * @param step The step between colors.
 * @return Return 0 upon success and non-zero otherwise.
 */
int (vbe_draw_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief Draws an XPM image at the specified coordinates.
 *
 * @param xpm The XPM image to draw.
 * @param x The x-coordinate of the top-left corner of the image.
 * @param y The y-coordinate of the top-left corner of the image.
 * @return Return 0 upon success and non-zero otherwise.
 */
int (vbe_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Moves an XPM image from the initial coordinates to the final coordinates with the specified speed and frame rate.
 *
 * @param xpm The XPM image to move.
 * @param xi The initial x-coordinate of the image.
 * @param yi The initial y-coordinate of the image.
 * @param xf The final x-coordinate of the image.
 * @param yf The final y-coordinate of the image.
 * @param speed The speed of the movement.
 * @param fr_rate The frame rate of the movement.
 * @return Return 0 upon success and non-zero otherwise.
 */
int (vbe_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate);
