/**
 * @file graphics.h
 * @brief Contains functions for setting graphic modes, managing buffers, and drawing.
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <lcom/lcf.h>
#include <stdint.h>


/**
 * @brief Sets the graphic mode.
 *
 * @param mode The VBE mode to set.
 * @return 0 on success, 1 on failure.
 */
int (set_graphic_mode)(uint16_t submode);


/**
 * @brief Sets up the video buffer for a specified mode.
 *
 * @param mode The VBE mode to set up.
 * @return 0 on success, 1 on failure.
 */
int (set_buffer)(uint16_t mode);


/**
 * @brief Draws a pixel at the specified coordinates with the specified color.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * @return 0 on success, 1 on failure.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);


/**
 * @brief Draws an XPM image at the specified coordinates.
 *
 * @param xmap The XPM image to draw.
 * @param x The x-coordinate of the top-left corner of the image.
 * @param y The y-coordinate of the top-left corner of the image.
 * @return 0 on success, 1 on failure.
 */
int (draw_xpm)(xpm_map_t xmap, uint16_t x, uint16_t y);


/**
 * @brief Normalizes the color according to the current bits per pixel setting.
 *
 * @param color The original color.
 * @param normalized_color The normalized color.
 */
void (fix_color)(uint32_t color, uint32_t *new_color);


/**
 * @brief Allocates a write buffer.
 *
 * @return 0 on success, 1 on failure.
 */
int(allocate_write_buffer)();


/**
 * @brief Switches the write buffer with the video buffer.
 */
void(switch_buffers)();


#endif
