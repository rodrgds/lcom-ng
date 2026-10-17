#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include <lcom/lcf.h>

/**
 * @brief Struct that represents the mode information.
*/
vbe_mode_info_t mode_info;

/**
 * @brief Function that sets the graphics mode.
 * @param mode Mode to set.
 * @return 0 if the graphics mode was successfully set, 1 otherwise.
*/
int (set_graphics_mode)(uint16_t mode);

/**
 * @brief Function that sets the text mode.
 * @return 0 if the text mode was successfully set, 1 otherwise.
*/
int (set_text_mode)();

/**
 * @brief Function that sets the frame buffer according to the submode and the buffer size.
 * @param mode Submode to use.
 * @return 0 if the frame buffer was successfully set, 1 otherwise.
*/
int (set_frame_buffer)(uint16_t mode, uint8_t **frame_buffer);

/**
 * @brief Function that draws a pixel on the screen.
 * @param x X coordinate of the pixel.
 * @param y Y coordinate of the pixel.
 * @param color Color of the pixel.
 * @param frame_buffer Frame buffer to draw the pixel.
 * @return 0 if the pixel was successfully drawn, 1 otherwise.
*/
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *frame_buffer);

/**
 * @brief Function that draws a vertical line on the screen.
 * @param x Initial X coordinate of the line.
 * @param y Initial Y coordinate of the line.
 * @param len Length of the line.
 * @param color Color of the line.
 * @param frame_buffer Frame buffer to draw the line.
 * @return 0 if the line was successfully drawn, 1 otherwise.
*/
int (draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t *frame_buffer);

#endif
