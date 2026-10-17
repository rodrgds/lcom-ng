#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "macros.h"

/** @defgroup Graphics_Card
 * @{
 *
 *
 *
 * Functions related to drawing graphical elements on the screen
 */

/**
 * @param mode desired graphics mode
 * @return Returns 1 if the mode was successfulyy set up, 0 otherwise
 */
int (set_graphics_mode)(uint16_t mode);

/**
 * @brief Draws a pixel on the screen in the specified coordinates
 * @param x x coordinate
 * @param y y coordinate
 * @param color color of the pixel
 * @param addToBackgroundFrame
 * @return Return 1 if an error occurred, 0 otherwise
 */
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, bool addToBackgroundFrame);

/**
 * @brief Map the video memory
 * @param mode Video card selected graphics mode
 * @return Returns 0 if the video memory was successfuly mapped, 1 otherwise
 */
int (map_vram)(uint16_t mode);

/**
 * @brief Maps the VRAM and sets the graphic mode
 * @param mode desired graphics mode
 * @return 1 if the mode has been successfully set, 0 otherwise
 */
int (vgcard_init)(uint16_t mode);

/**
 * @brief Given a xpm image, draws its pixmap to the screen
 * @param x X coordinate
 * @param y Y coordinate
 * @param img xpm image to draw
 * @param addToBackgroundFrame If true, add the image to the frame permanently (mouse purposes)
 */
int(draw_pixmap)(uint16_t x, uint16_t y, xpm_image_t *img, bool addToBackgroundFrame);

/**
 * @brief Given a xpm image, draws its pixmap to the screen, with a set color
 * @param x x coordinate
 * @param y y coordinate
 * @param img xpm image to draw
 * @param color color
 * @param addToBackgroundFrame If true, add the image to the frame permanently (mouse purposes)
 */
int (draw_pixmap_fixed_color)(uint16_t x, uint16_t y, xpm_image_t *img, uint32_t color, bool addToBackgroundFrame);

/**
 * @brief Retrieve the screen size information
 * @param screen_width screen width
 * @param screen_height screen height
 */
void(get_screen_dimensions)(int *screen_width, int *screen_height);

/**
 * @brief Add a pixel to the background frame
 * @param X x position of the pixel
 * @param Y y position of the pixel
 * @param color color of the pixel
 */
void(add_pixel_to_frame)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a line in the screen
 * @param x line start: x coordinate
 * @param y line start: y coordinate
 * @param len line length
 * @param color line color
 * @param addToBackgroundFrame If true, add the line to the frame permanently (mouse purposes)
 * @param horizontal If true, draw a horizontal line. Otherwise draw a vertical line.
 */
int(my_vg_draw_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, bool addToBackgroundFrame, bool horizontal);

/**
 * @brief Draws a rectangle in the screen
 * @param x rectangle top left: x coordinate
 * @param y rectangle top left: y coordinate
 * @param width rectangle width
 * @param height rectangle height
 * @param color rectangle color
 * @param addToBackgroundFrame If true, add the rectangle to the frame permanently (mouse purposes)
 */
int(my_vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, bool add_rectangle_to_frame);

/**
 * @brief Draw the border of a square on the screen
 * @param x square top left: x coordinate
 * @param y square top left: y coordinate
 * @param len square side length
 * @param color square color
 * @param addToBackgroundFrame If true, add the square to the frame permanently (mouse purposes)
 */
void(draw_unfilled_square)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, bool addToBackgroundFrame);

#endif /* __GRAPHICS_H */
