/**
 * @file video.h
 * @brief Video graphics functions and definitions.
 */

#ifndef VIDEO_H
#define VIDEO_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "vbe.h"

extern uint8_t* video_mem;
extern uint8_t* double_buffer;
extern vbe_mode_info_t mode_info;

/**
 * @brief Sets the video mode.
 * 
 * @param submode The VBE submode to set.
 * @return 0 upon success, 1 otherwise.
 */
int (set_video_mode)(uint16_t submode);

/**
 * @brief Retrieves the mode information.
 * 
 * @param mode The mode to get information for.
 * @return 0 upon success, 1 otherwise.
 */
int get_mode_info (uint16_t mode);

/**
 * @brief Calculates the size of the frame buffer.
 * 
 * @return The size of the frame buffer in bytes.
 */
unsigned int (get_frame_buffer_size)();

/**
 * @brief Sets up the frame buffer.
 * 
 * @param mode The mode to set the frame buffer for.
 * @return 0 upon success, 1 otherwise.
 */
int set_frame_buffer(uint16_t mode);

/**
 * @brief Sets the color of a pixel.
 * 
 * @param x The X coordinate of the pixel.
 * @param y The Y coordinate of the pixel.
 * @param color The color to set.
 * @return 0 upon success, 1 otherwise.
 */
int (set_pixel_color) (uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line.
 * 
 * @param x The starting X coordinate of the line.
 * @param y The Y coordinate of the line.
 * @param len The length of the line.
 * @param color The color to set.
 * @return 0 upon success, 1 otherwise.
 */
int (draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a vertical line.
 * 
 * @param x The X coordinate of the line.
 * @param y The starting Y coordinate of the line.
 * @param len The length of the line.
 * @param color The color to set.
 * @return 0 upon success, 1 otherwise.
 */
int (draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle.
 * 
 * @param x The X coordinate of the rectangle.
 * @param y The Y coordinate of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color to set.
 * @return 0 upon success, 1 otherwise.
 */
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws an XPM image.
 * 
 * @param x The X coordinate to start drawing the XPM.
 * @param y The Y coordinate to start drawing the XPM.
 * @param xpm The XPM map to draw.
 * @return 0 upon success, 1 otherwise.
 */
int (draw_xpm)(uint16_t x, uint16_t y, xpm_map_t xpm);

/**
 * @brief Cleans the buffer by setting it to zero.
 * 
 * @return 0 upon success, 1 otherwise.
 */
int (clean_buffer) ();

/**
 * @brief Swaps the buffer to display the contents.
 * 
 * @return 0 upon success, 1 otherwise.
 */
int (swap_buffer) ();

#endif // VIDEO_H
