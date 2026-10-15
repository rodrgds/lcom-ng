#ifndef VIDEO_H
#define VIDEO_H

#include <lcom/lcf.h>

/**
 * @brief Initializes the video card in the specified mode.
 * @details This function sets up the video card to operate in the specified graphics mode.
 * @param mode The video mode to set.
 * @return 0 on success, 1 otherwise.
 */
int(vg_initi)(uint16_t mode);

/**
 * @brief Draws a pixel on the screen.
 * @details This function draws a pixel at the specified (x, y) coordinates with the given color.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * @return 0 on success, 1 otherwise.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint8_t* color);

/**
 * @brief Draws a horizontal line on the screen.
 * @details This function draws a horizontal line starting from the specified (x, y) coordinates 
 * with the given length and color.
 * @param x The x-coordinate of the starting point of the line.
 * @param y The y-coordinate of the starting point of the line.
 * @param len The length of the line.
 * @param color The color of the line.
 * @return 0 on success, 1 otherwise.
 */
int (vg_draw_line)(uint16_t x, uint16_t y, uint16_t len, uint8_t* color);

/**
 * @brief Draws a rectangle on the screen.
 * @details This function draws a rectangle starting from the specified (x, y) coordinates with the 
 * given width, height, and color.
 * @param x The x-coordinate of the starting point of the rectangle.
 * @param y The y-coordinate of the starting point of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 * @return 0 on success, 1 otherwise.
 */
int (vg_draw_rect)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t* color);

/**
 * @brief Copies the buffer to the video memory.
 * @details This function implements double buffering by copying the content of the buffer to the 
 * video memory.
 */
void (double_buffer)();

/**
 * @brief Draws an XPM image on the screen.
 * @details This function draws an XPM image at the specified (x, y) coordinates.
 * @param used_colors The color palette used in the XPM image.
 * @param img The XPM image to be drawn.
 * @param x The x-coordinate of the starting point of the image.
 * @param y The y-coordinate of the starting point of the image.
 * @return 0 on success, 1 otherwise.
 */
int(draw_xpm)(uint8_t *used_colors, xpm_image_t img, uint16_t x, uint16_t y);

/**
 * @brief Draws a sprite on the screen.
 * @details This function draws a sprite represented by an XPM image at the specified (x, y) 
 * coordinates.
 * @param img The XPM image representing the sprite.
 * @param sprite The sprite data to be drawn.
 * @param x The x-coordinate of the starting point of the sprite.
 * @param y The y-coordinate of the starting point of the sprite.
 * @return 0 on success, 1 otherwise.
 */
int(draw_sprite)(xpm_image_t img, uint8_t* sprite, uint16_t x, uint16_t y);

#endif
