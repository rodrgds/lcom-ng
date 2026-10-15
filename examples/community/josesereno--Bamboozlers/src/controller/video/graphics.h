#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

/**
 * @file graphics.h
 * @brief Header file containing graphics interface functions.
 */

#include <lcom/lcf.h>
#include "VBE.h"

/** @defgroup graphics graphics
 * @{
 */
vbe_mode_info_t mode_info;
uint8_t *main_frame_buffer;
uint8_t *drawing_frame_buffer;
uint16_t xRes;
uint16_t yRes;
unsigned int bytes_per_pixel;
unsigned int frame_size;



/**
 * @brief Sets the graphic mode.
 *
 * @param mode Video mode to set.
 * @return 0 on success, non-zero value on failure.
 */
int(set_graphic_mode)(uint16_t mode);

/**
 * @brief Sets the text mode.
 *
 * @return 0 on success, non-zero value on failure.
 */
int(set_text_mode)();

/**
 * @brief Sets the main buffer for drawing operations.
 *
 * @param mode Video mode to set.
 * @return 0 on success, non-zero value on failure.
 */
int(set_main_buffer)(uint16_t mode);

/**
 * @brief Sets the drawing buffer for subsequent drawing operations.
 */
void (set_drawing_buffer)();

/**
 * @brief Swaps the drawing buffer with the main buffer, updating the screen.
 */
void (swap_buffers)();

/**
 * @brief Clears the drawing buffer by setting all pixels to black.
 */
void (clear_drawing_buffer)();

/**
 * @brief Frees the memory allocated for the drawing buffer.
 */
void (free_drawing_buffer)();

/**
 * @brief Draws a pixel at the specified coordinates with the given color.
 *
 * @param x X coordinate of the pixel.
 * @param y Y coordinate of the pixel.
 * @param color Color of the pixel.
 * @return 0 on success, non-zero value on failure.
 */
int(vg_draw_pixel)(int x, int y, uint32_t color);

/**
 * @brief Draws a horizontal line starting at the specified coordinates with the given length and color.
 *
 * @param x X coordinate of the starting point.
 * @param y Y coordinate of the starting point.
 * @param len Length of the line.
 * @param color Color of the line.
 * @return 0 on success, non-zero value on failure.
 */
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle starting at the specified coordinates with the given width, height, and color.
 *
 * @param x X coordinate of the starting point.
 * @param y Y coordinate of the starting point.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Color of the rectangle.
 * @return 0 on success, non-zero value on failure.
 */
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/** @} */ // end of Graphics group

#endif
