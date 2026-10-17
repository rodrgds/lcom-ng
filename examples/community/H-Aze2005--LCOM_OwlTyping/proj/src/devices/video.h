/**
 * @file video.h
 * @brief Video device interface and graphics operations.
 *
 * Declares functions for initializing video mode, drawing images and pixels,
 * managing buffers, and handling background rendering.
 */

#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <lcom/lcf.h>
#include <machine/int86.h>

/**
 * @brief Initializes the video mode.
 * @param mode Video mode to set.
 * @return 0 on success, non-zero otherwise.
 */
int (video_init)(uint16_t mode);

/**
 * @brief Draws an XPM image to a buffer at the specified position.
 * @param xpm The XPM image.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param buffer The buffer to draw to.
 * @return 0 on success, non-zero otherwise.
 */
int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y, void *buffer);

/**
 * @brief Fills a pixel at (x, y) with the specified color in a buffer.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color Pixel color.
 * @param buffer The buffer to draw to.
 * @return 0 on success, non-zero otherwise.
 */
int (fill_pixel)(uint16_t x, uint16_t y, uint32_t color, void *buffer);

/**
 * @brief Fills the entire screen with the specified color.
 * @param color Color to fill.
 * @return 0 on success, non-zero otherwise.
 */
int (fill_screen)(uint32_t color);

/**
 * @brief Draws an XPM image to the screen at the specified position.
 * @param xpm The XPM image.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return 0 on success, non-zero otherwise.
 */
int (draw_image)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Draws an XPM image to both the screen and background buffer.
 * @param xpm The XPM image.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return 0 on success, non-zero otherwise.
 */
int draw_image_to_both(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Loads an XPM image as the background.
 * @param xpm The XPM image.
 * @return 0 on success, non-zero otherwise.
 */
int (load_background_xpm)(xpm_map_t xpm);

/**
 * @brief Draws the contents of the video buffer to the screen.
 */
void (draw_buffer)();

/**
 * @brief Draws the background image to the screen.
 */
void (draw_background)();

/**
 * @brief Redraws a rectangular region of the background.
 * @param x X coordinate of the region.
 * @param y Y coordinate of the region.
 * @param width Width of the region.
 * @param height Height of the region.
 * @return 0 on success, non-zero otherwise.
 */
int (redraw_background)(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Draws a rectangle from the background at the specified region.
 * @param x X coordinate of the rectangle.
 * @param y Y coordinate of the rectangle.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @return 0 on success, non-zero otherwise.
 */
int (draw_rectangle_background)(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

#endif
