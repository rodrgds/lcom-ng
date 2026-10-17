/**
 * @file video_gr.h
 * @brief Interface for handling video graphics operations.
 *
 * This header provides functions for initializing video modes, drawing pixels, lines, rectangles,
 * patterns, and XPM images, as well as managing double buffering and retrieving video mode information.
 */
#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <lcom/xpm.h>

/** @defgroup video video
 *
 * @brief Functions for using the video graphics
 * 
 * @{
 *
 */

/**
 * @brief Color mode used by the video graphics
 */
typedef enum color_mode {
  DIRECT,  /**< Direct color mode */
  INDEXED, /**< Indexed color mode */
  OTHER    /**< Other color mode */
} color_mode;

/**
 * @brief Initializes the video graphics in the specified mode.
 *
 * Maps video memory and allocates a back buffer for double buffering.
 *
 * @param mode The VBE mode to set.
 * @return Pointer to the mapped VRAM if successful, NULL otherwise.
 */
void* (vg_init)(uint16_t mode);

/**
 * @brief Draws a horizontal line on the back buffer.
 *
 * @param x Starting x coordinate.
 * @param y Y coordinate.
 * @param len Length of the line.
 * @param color Color to draw with.
 * @return 0 on success and 1 if failed.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a pixel on the back buffer.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color Color of the pixel.
 * @return 0 on success and 1 if failed.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a filled rectangle on the back buffer.
 *
 * @param x Top-left x coordinate.
 * @param y Top-left y coordinate.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Color to fill.
 * @return 0 on success and 1 if failed.
 */
int(vg_draw_rectangle)(uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws multiple rectangles (pattern) - function not fully documented.
 *
 * @param no_rectangles Number of rectangles.
 * @param first Starting color.
 * @param step Color step increment.
 * @return 0 on success and 1 if failed.
 */
int (vg_draw_rectangles)(uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief Fills the entire back buffer with a color.
 *
 * @param color Color to fill with.
 * @return 0 on success and 1 if failed.
 */
int (vg_fill)(uint32_t color);

/**
 * @brief Copies the back buffer content to VRAM.
 */
void (vg_copy_buffer)();

/**
 * @brief Draws an XPM image on the back buffer.
 *
 * @param img The XPM image.
 * @param x Top-left x coordinate.
 * @param y Top-left y coordinate.
 * @param flip Whether to flip the image horizontally.
 * @return 0 on success and 1 if failed.
 */
int (draw_xpm)(xpm_image_t img, uint16_t x, uint16_t y, bool flip);

/**
 * @brief Returns the horizontal resolution.
 *
 * @return Horizontal resolution in pixels.
 */
unsigned int get_hres();

/**
 * @brief Returns the vertical resolution.
 *
 * @return Vertical resolution in pixels.
 */
unsigned int get_vres();

/**
 * @brief Returns the bytes per pixel.
 *
 * @return Number of bytes per pixel.
 */
unsigned int get_bytes_per_pixel();

/**
 * @brief Returns a pointer to the back buffer.
 *
 * @return Pointer to the back buffer.
 */
uint8_t* get_back_buffer();

/**
 * @brief Returns the size of the VRAM and back buffer.
 *
 * @return Size in bytes.
 */
size_t get_vram_size();

/** @}  */

#endif // __VIDEO_GR_H
