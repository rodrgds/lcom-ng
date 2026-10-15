/**
 * @file video.h
 * @brief Interface for video graphics operations and drawing primitives.
 *
 * Provides functions for setting video modes, drawing pixels, lines, rectangles, sprites, and text.
 */

#ifndef _LCOM_VIDEO_H_
#define _LCOM_VIDEO_H_

#include "VBE_interface.h"
#include <lcom/lcf.h>
#include "utils/sprite.h"

/**
 * @brief VBE mode information structure for the current graphics mode.
 */
vbe_mode_info_t mode_info;

/**
 * @brief Enables the specified VBE graphics mode and initializes video memory.
 *
 * @param res The VBE mode to set (resolution and color depth).
 * @return 0 on success, non-zero otherwise.
 */
int enable_graphic_mode(uint16_t res);

/**
 * @brief Maps the video memory (frame buffer) for the specified VBE graphics mode into the process's address space.
 *
 * This function fills the global mode_info structure with information about the requested mode, calculates the frame buffer's physical address and size, requests permission to map this memory, and then maps it so the process can draw directly to video memory.
 *
 * @param mode The VBE mode to use (resolution and color depth).
 * @param frame_buffer Pointer to a variable that will be set to the mapped frame buffer address on success.
 * @return 0 on success, non-zero otherwise.
 */
int get_frame_buffer(uint16_t mode, uint8_t** frame_buffer);

/**
 * @brief Draws a single pixel at (x, y) with the specified color in the given frame buffer.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color Color value.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero otherwise.
 */
int video_draw_pixel(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer);

/**
 * @brief Draws a horizontal line starting at (x, y) with the specified length and color.
 *
 * @param x X coordinate of the start.
 * @param y Y coordinate.
 * @param len Length of the line in pixels.
 * @param color Color value.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero otherwise.
 */
int video_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t* frame_buffer);

/**
 * @brief Draws a filled rectangle at (x, y) with the specified width, height, and color.
 *
 * @param x X coordinate of the top-left corner.
 * @param y Y coordinate of the top-left corner.
 * @param width Rectangle width in pixels.
 * @param height Rectangle height in pixels.
 * @param color Color value.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero otherwise.
 */
int video_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t* frame_buffer);

/**
 * @brief Draws an XPM sprite at (x, y) in the given frame buffer.
 *
 * @param sprite The Sprite structure containing the XPM image.
 * @param x X coordinate of the top-left corner.
 * @param y Y coordinate of the top-left corner.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero otherwise.
 */
int video_draw_xpm(struct Sprite sprite, uint16_t x, uint16_t y, uint8_t* frame_buffer);

/**
 * @brief Draws a single character at (x, y) with the specified color and scale.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param c Character to draw.
 * @param color Color value.
 * @param frame_buffer Pointer to the frame buffer.
 * @param scale Scale factor for the character size.
 * @return 0 on success, non-zero otherwise.
 */
int video_draw_char(uint16_t x, uint16_t y, char c, uint32_t color, uint8_t* frame_buffer, uint8_t scale);

/**
 * @brief Draws a string of text at (x, y) with the specified color and scale.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color Color value.
 * @param text Null-terminated string to draw.
 * @param frame_buffer Pointer to the frame buffer.
 * @param scale Scale factor for the text size.
 * @return 0 on success, non-zero otherwise.
 */
int video_draw_text(uint16_t x, uint16_t y, uint32_t color, char* text, uint8_t* frame_buffer, uint8_t scale);

/**
 * @brief Draws a string of text centered horizontally at the given y coordinate.
 *
 * @param y Y coordinate.
 * @param color Color value.
 * @param text Null-terminated string to draw.
 * @param frame_buffer Pointer to the frame buffer.
 * @param scale Scale factor for the text size.
 * @return 0 on success, non-zero otherwise.
 */
int video_draw_centered_text(uint16_t y, uint32_t color, char* text, uint8_t* frame_buffer, uint8_t scale);


#endif /* _LCOM_VIDEO_H_ */
