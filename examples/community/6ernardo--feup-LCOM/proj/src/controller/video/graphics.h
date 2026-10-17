#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include "VBE.h"

/** @defgroup graphics graphics
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

vbe_mode_info_t mode_info;

/**
 * @brief Sets the graphic mode with the specified submode
 * @param submode Submode of the graphic mode
 * @return 0 if it is successful, not 0 if otherwise
 */

int set_graphic_mode( uint16_t submode);

/**
 * @brief  Sets the display mode to text mode.
 * @return 0 if it is successful, not 0 if otherwise
 */

int set_text_mode();

/**
 * @brief Sets up the frame buffer for the specified video mode.
 * @param mode Video mode to set up the frame buffer for.
 * @param frame_buffer Pointer to the frame buffer that will be allocated and populated.
 * @return 0 if the frame buffer is set up successfully, non-zero value otherwise.
 */


int set_frame_buffer(uint16_t mode, uint8_t **frame_buffer);

/** @brief Paints pixel in certain position of certain color
 * @param x X-coordinate of the pixel
 * @param y Y-coordinate of the pixel
 * @param color Color of the pixel
 * @param frame_buffer Frame buffer to paint the pixel in
 * @return Returns 0 upon success, 1 with failure
 */

int paint_pixel(uint16_t x, uint16_t y, uint32_t color, uint8_t *frame_buffer);

/**
 * @brief Draws a horizontal line on the frame buffer
 * @param x X-coordinate of the starting point of the line
 * @param y Y-coordinate of the starting point of the line
 * @param len Length of the line in pixels
 * @param color Color of the line to be drawn
 * @param frame_buffer Pointer to the frame buffer
 * @return 0 if the line is drawn successfully, non-zero value otherwise
 */


int draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t *frame_buffer);
/** @brief Draws rectangle starting in position (x,y) with width and height specified in parameters.
 * @param x Horizontal position of the beginning of the rectangle
 * @param y Vertical position of the beginning of the rectangle
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color to fill the rectangle
 * @param frame_buffer Pointer to the frame buffer
 */
int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *frame_buffer);

/**
 * @brief Inverts the color of a pixel in the frame buffer.
 * @param x X-coordinate of the pixel.
 * @param y Y-coordinate of the pixel.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 if the pixel color is inverted successfully, non-zero value otherwise.
 *
 * This function inverts the color of a pixel at the specified coordinates (x, y) in the frame buffer.
 * The color of the pixel is then inverted by applying bitwise negation to each color component (R, G, B) of the 32-bit color.
 */

int invert_pixel_color(uint16_t x, uint16_t y, uint8_t *frame_buffer);

/**
 * @brief Converts a pixel to grayscale in the frame buffer.
 * @param x X-coordinate of the pixel.
 * @param y Y-coordinate of the pixel.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 if the pixel is converted to grayscale successfully, non-zero value otherwise.
 *
 * This function converts the color of a pixel at the specified coordinates (x, y) in the frame buffer to grayscale.
 * The color components (red, green, blue) of the pixel are extracted from the 32-bit color value.
 * The average value of the color components is calculated to obtain the grayscale value.
 * A grayscale color is created by setting the red, green, and blue components with the same grayscale value.
 * The original color in the pixel is then replaced with the grayscale color.
 */


int convert_to_gray(uint16_t x, uint16_t y, uint8_t *frame_buffer);

#endif
