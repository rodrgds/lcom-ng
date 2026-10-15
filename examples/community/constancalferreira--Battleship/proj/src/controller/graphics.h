#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include <lcom/vbe.h>
#include "keyboard.h"
#include <stdint.h>

/**
 * @brief Sets the video graphics mode.
 * 
 * @param mode VBE mode to set (e.g., 0x115)
 * @return 0 on success, non-zero otherwise
 */
int (set_graphics_mode)(uint16_t mode);

/**
 * @brief Restores the default text mode (0x03).
 * 
 * @return 0 on success, non-zero otherwise
 */
int (set_text_mode)();

/**
 * @brief Initializes and maps the frame buffer to memory.
 * 
 * @param mode VBE mode (e.g., 0x115)
 * @return 0 on success, non-zero otherwise
 */
int (set_frame_buffer)(uint16_t mode);

/**
 * @brief Normalizes a 24-bit color into the current graphics mode format.
 * 
 * @param color Input RGB color
 * @param new_color Pointer to store the converted color
 * @return 0 on success, non-zero otherwise
 */
int (normalize_color)(uint32_t color, uint32_t *new_color);

/**
 * @brief Draws a pixel at a given position with a specific color in the given buffer.
 * 
 * @param x X-coordinate
 * @param y Y-coordinate
 * @param color Pixel color
 * @param buffer Pointer to the video buffer
 * @return 0 on success, non-zero otherwise
 */
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *buffer);

/**
 * @brief Draws a horizontal line starting at (x, y) with the given length and color.
 * 
 * @param x Starting X-coordinate
 * @param y Y-coordinate
 * @param len Length of the line
 * @param color Line color
 * @param buffer Target video buffer
 * @return 0 on success, non-zero otherwise
 */
int (draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t *buffer);

/**
 * @brief Draws a filled rectangle at (x, y) with specified width, height, and color.
 * 
 * @param x X-coordinate of top-left corner
 * @param y Y-coordinate of top-left corner
 * @param width Rectangle width
 * @param height Rectangle height
 * @param color Fill color
 * @param buffer Target video buffer
 * @return 0 on success, non-zero otherwise
 */
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *buffer);

/**
 * @brief Fills the entire screen buffer with the specified color.
 * 
 * @param color Fill color
 * @param buffer Target video buffer
 * @return 0 on success, non-zero otherwise
 */
int (fill_screen)(uint32_t color, uint8_t *buffer);

/**
 * @brief Swaps the display buffers (double buffering).
 * 
 * @return 0 on success, non-zero otherwise
 */
int (swap_buffers)();

/**
 * @brief Frees memory allocated for a buffer.
 *
 * @param buffer Pointer to the buffer to free
 * @return 0 on success, non-zero otherwise
 */
int (free_buffer)(uint8_t *buffer);


#endif
