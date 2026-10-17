#ifndef __GRAPHICS_CARD_H
#define __GRAPHICS_CARD_H

#include <lcom/lcf.h>
#include <lcom/video_gr.h>

#include "model/base_frame.h"
#include "model/colors.h"
#include "VBE.h"

/**
 * @brief Maps graphics memory for the specified video mode.
 *
 * @param mode The video mode to map the graphics memory for.
 *
 * @return A pointer to the mapped graphics memory.
 */
void *(map_graphics_memory)(uint16_t mode);

/**
 * @brief Flips the frame buffer to display the newly drawn frame.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (vg_flip_frame)();

/**
 * @brief Copies the base frame buffer to the specified frame buffer.
 *
 * @param frame_buffer The frame buffer to copy the base frame to.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (copy_base_frame)(frame_buffer_t frame_buffer);

/**
 * @brief Draws a pixel at the specified coordinates with the given color.
 *
 * @param x     The x-coordinate of the pixel.
 * @param y     The y-coordinate of the pixel.
 * @param color The color of the pixel.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line starting from the specified coordinates with the given length and color.
 *
 * @param x     The starting x-coordinate of the line.
 * @param y     The y-coordinate of the line.
 * @param len   The length of the line.
 * @param color The color of the line.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle with the specified position, size, and color.
 *
 * @param x      The x-coordinate of the top-left corner of the rectangle.
 * @param y      The y-coordinate of the top-left corner of the rectangle.
 * @param width  The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color  The color of the rectangle.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Initializes the video graphics mode.
 *
 * @param mode The video mode to initialize.
 *
 * @return A pointer to the initialized video graphics mode.
 */
void *(vg_init)(uint16_t mode);

/**
 * @brief Retrieves the red component of a color using the provided parameters.
 *
 * @param y     The y-coordinate.
 * @param first The first color component value.
 * @param step  The color component step value.
 *
 * @return The red component of the color.
 */
uint32_t (red)(unsigned y, uint32_t first, uint8_t step);

/**
 * @brief Retrieves the green component of a color using the provided parameters.
 *
 * @param x     The x-coordinate.
 * @param first The first color component value.
 * @param step  The color component step value.
 *
 * @return The green component of the color.
 */
uint32_t (green)(unsigned x, uint32_t first, uint8_t step);

/**
 * @brief Retrieves the blue component of a color using the provided parameters.
 *
 * @param x     The x-coordinate.
 * @param y     The y-coordinate.
 * @param first The first color component value.
 * @param step  The color component step value.
 *
 * @return The blue component of the color.
 */
uint32_t (blue)(unsigned x, unsigned y, uint32_t first, uint8_t step);

/**
 * @brief Retrieves the red component of a color.
 *
 * @param color The color.
 *
 * @return The red component of the color.
 */
uint32_t (get_red_bits)(uint32_t color);

/**
 * @brief Retrieves the green component of a color.
 *
 * @param color The color.
 *
 * @return The green component of the color.
 */
uint32_t (get_green_bits)(uint32_t color);

/**
 * @brief Retrieves the blue component of a color.
 *
 * @param color The color.
 *
 * @return The blue component of the color.
 */
uint32_t (get_blue_bits)(uint32_t color);

/**
 * @brief Computes the color of a pixel based on a pattern of rectangles.
 *
 * @param first         The first color component value.
 * @param step          The color component step value.
 * @param no_rectangles The number of rectangles.
 * @param row           The row index of the rectangle.
 * @param col           The column index of the rectangle.
 *
 * @return The computed color of the pixel.
 */
uint32_t (index_color)(uint32_t first, uint8_t step, uint8_t no_rectangles, uint8_t row, uint8_t col);

/**
 * @brief Creates a color using the provided RGB values.
 *
 * @param red   The red component value.
 * @param green The green component value.
 * @param blue  The blue component value.
 *
 * @return The created color.
 */
uint32_t (direct_color)(uint32_t red, uint32_t green, uint32_t blue);

/**
 * @brief Retrieves the width of an XPM image.
 *
 * @param xpm The XPM image.
 *
 * @return The width of the XPM image.
 */
uint8_t (get_xpm_width)(xpm_map_t xpm);

/**
 * @brief Retrieves the height of an XPM image.
 *
 * @param xpm The XPM image.
 *
 * @return The height of the XPM image.
 */
uint8_t (get_xpm_height)(xpm_map_t xpm);

/**
 * @brief Retrieves the number of colors in an XPM image.
 *
 * @param xpm The XPM image.
 *
 * @return The number of colors in the XPM image.
 */
uint8_t (get_xpm_colors_no)(xpm_map_t xpm);

/**
 * @brief Retrieves the color symbol array of an XPM image.
 *
 * @param xpm       The XPM image.
 * @param colors_no The number of colors in the XPM image.
 *
 * @return The color symbol array of the XPM image.
 */
color_symbol *(get_xpm_colors)(xpm_map_t xpm, uint8_t colors_no);


#endif /* __GRAPHICS_CARD_H */
