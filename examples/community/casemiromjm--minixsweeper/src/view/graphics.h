/**
 * @file graphics.h
 * @brief Header file for graphics handling functions and global variables.
 */

#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include "../VBE.h"

/// Default screen width in pixels
#define SCREEN_WIDTH 800

/// Default screen height in pixels
#define SCREEN_HEIGHT 600

/**
 * @brief Stores information about the current VBE graphics mode.
 */
vbe_mode_info_t mode_info;

/**
 * @brief Pointer to the mapped video memory (frame buffer).
 */
uint8_t* frame_buffer;

/**
 * @brief Sets the video card to the specified VBE graphics mode.
 * @param submode VBE mode identifier.
 * @return 0 on success, non-zero otherwise.
 */
int (set_graphic_mode)(uint16_t submode);

/**
 * @brief Switches the video card to text mode (0x03).
 * @return 0 on success, non-zero otherwise.
 */
int (set_text_mode)();

/**
 * @brief Maps the frame buffer into the process' virtual address space.
 * @param mode Video mode to set and retrieve info from.
 * @return 0 on success, non-zero otherwise.
 */
int (set_frame_buffer)(uint16_t mode);

/**
 * @brief Draws a pixel to the image buffer.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color Pixel color.
 * @return 0 on success, 1 on failure.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line on the image buffer.
 * @param x Starting X coordinate.
 * @param y Y coordinate.
 * @param len Length of the line.
 * @param color Line color.
 * @return 0 on success, 1 on failure.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a filled rectangle on the image buffer.
 * @param x Top-left X coordinate.
 * @param y Top-left Y coordinate.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Fill color.
 * @return 0 on success, 1 on failure.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Prints an XPM image at a given location.
 * @param xpm XPM image data.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return 0 on success, non-zero on failure.
 */
int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Normalizes a color to the current graphics mode.
 * @param color Input color.
 * @param new_color Pointer to output normalized color.
 * @return 0 always.
 */
int (normalize_color)(uint32_t color, uint32_t *new_color);

/**
 * @brief Creates a direct color value from RGB components.
 * @param R Red component.
 * @param G Green component.
 * @param B Blue component.
 * @return Packed RGB color value.
 */
uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B);

/**
 * @brief Computes color value for indexed color mode.
 * @param col Column index.
 * @param row Row index.
 * @param step Color increment.
 * @param first First color value.
 * @param n Number of unique colors per row.
 * @return Computed indexed color.
 */
uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);

/**
 * @brief Computes red component for pattern color.
 */
uint32_t (Red)(unsigned j, uint8_t step, uint32_t first);

/**
 * @brief Computes green component for pattern color.
 */
uint32_t (Green)(unsigned i, uint8_t step, uint32_t first);

/**
 * @brief Computes blue component for pattern color.
 */
uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first);

/**
 * @brief Extracts red component from a 32-bit packed color.
 */
uint32_t (R)(uint32_t first);

/**
 * @brief Extracts green component from a 32-bit packed color.
 */
uint32_t (G)(uint32_t first);

/**
 * @brief Extracts blue component from a 32-bit packed color.
 */
uint32_t (B)(uint32_t first);

/**
 * @brief Returns a pointer to the image (off-screen) buffer.
 * @return Pointer to the image buffer.
 */
uint8_t *get_buffer();

/**
 * @brief Allocates memory for the image buffer.
 */
void allocateImgBuffer();

/**
 * @brief Copies the image buffer to the video memory.
 */
void copyBufferToVideo();

/**
 * @brief Clears the contents of the image buffer (sets to black/zero).
 */
void clearImgBuffer();

/**
 * @brief Frees memory allocated to the image buffer.
 */
void freeImgBuffer();

#endif /* __GRAPHICS_H */

