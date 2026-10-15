#ifndef _VIDEO_H
#define _VIDEO_H

#include <lcom/lcf.h>

/**
 * Sets the video mode.
 * 
 * @param mode Video mode to set.
 * @return 0 on success, non-zero otherwise.
 */
int (video_set_mode)(uint16_t mode);

/**
 * Maps the video memory for the specified mode.
 * 
 * @param mode Video mode to map memory for.
 * @return 0 on success, non-zero otherwise.
 */
int (video_map_memory)(uint16_t mode);

/**
 * Swaps the double buffer to the video memory.
 */
void (video_swap_buffer)();

/**
 * Clears the double buffer.
 */
void (video_clear_buffer)();

/**
 * Gets the vertical resolution of the current video mode.
 * 
 * @return The vertical resolution (number of rows).
 */
unsigned int (get_vertical_resolution)();

/**
 * Gets the horizontal resolution of the current video mode.
 * 
 * @return The horizontal resolution (number of columns).
 */
unsigned int (get_horizontal_resolution)();

/**
 * Gets the number of bits per pixel of the current video mode.
 * 
 * @return The number of bits per pixel.
 */
unsigned int (get_bits_per_pixel)();

/**
 * Draws a pixel at the specified coordinates with the given color.
 * 
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color Color value.
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * Draws a horizontal line starting at (x, y) with the specified length and color.
 * 
 * @param x Starting X coordinate.
 * @param y Y coordinate.
 * @param len Length of the line.
 * @param color Color value.
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * Draws a rectangle at the specified coordinates with the given width, height, and color.
 * 
 * @param x X coordinate of the top-left corner.
 * @param y Y coordinate of the top-left corner.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Color value.
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * Draws an XPM image at the specified coordinates.
 * 
 * @param xpm XPM image to draw.
 * @param x X coordinate of the top-left corner.
 * @param y Y coordinate of the top-left corner.
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * Calculates the direct color value for a rectangle in direct color mode.
 * 
 * @param row Row index of the rectangle.
 * @param col Column index of the rectangle.
 * @param no_rectangles Number of rectangles per row/column.
 * @param first First color value.
 * @param step Color step.
 * @return Calculated color value.
 */
uint32_t (direct_color)(uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first,  uint8_t step);

/**
 * Calculates the indexed color value for a rectangle in indexed color mode.
 * 
 * @param row Row index of the rectangle.
 * @param col Column index of the rectangle.
 * @param no_rectangles Number of rectangles per row/column.
 * @param first First color value.
 * @param step Color step.
 * @return Calculated color value.
 */
uint32_t (indexed_color)(uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first,  uint8_t step);


#endif 
