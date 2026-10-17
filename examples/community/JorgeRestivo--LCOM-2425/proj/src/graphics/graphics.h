/**
 * @file graphics.h
 * @brief Graphics mode management, drawing primitives, and sprite rendering.
 */

#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include <lcom/xpm.h>
#include "keyboard.h"

/** @brief VBE mode information. */
vbe_mode_info_t mode_info;
/** @brief Pointer to the frame buffer. */
extern uint8_t *frame_buffer;
/** @brief Pointer to the double buffer. */
extern uint8_t *double_buffer;
/** @brief Size of the frame buffer. */
extern unsigned int frame_size;

/**
 * @brief Swaps the double buffer to the frame buffer.
 */
void swap_buffer();

void (vg_clear_screen)();

/**
 * @brief Initializes graphics mode and frame buffer.
 * @param mode VBE mode to set.
 * @return Pointer to the frame buffer, or NULL on failure.
 */
void* (vg_initialize)(uint16_t mode);

/**
 * @brief Draws the background image to the buffer.
 * @param bg Pointer to the background xpm image.
 * @return 0 on success, 1 on error.
 */
int(draw_background)(xpm_image_t *bg);

/**
 * @brief Loads an XPM sprite into an xpm_image_t.
 * @param xpm XPM map.
 * @param img Pointer to xpm_image_t to fill.
 * @return 0 on success, 1 on error.
 */
int(load_xpm)(xpm_map_t xpm, xpm_image_t *img);

/**
 * @brief Draws a sprite at the given position.
 * @param sprite Pointer to sprite image.
 * @param x X coordinate.
 * @param y Y coordinate.
 */
void(draw_sprite)(xpm_image_t *sprite, uint16_t x, uint16_t y);

/**
 * @brief Sets the graphics mode.
 * @param submode VBE mode.
 * @return 0 on success, non-zero on error.
 */
int(set_graphic_mode)(uint16_t submode);

/**
 * @brief Sets the text mode.
 * @return 0 on success, non-zero on error.
 */
int(set_text_mode)();

/**
 * @brief Sets up the frame buffer for the given mode.
 * @param mode VBE mode.
 * @return 0 on success, 1 on error.
 */
int(set_frame_buffer)(uint16_t mode);

/**
 * @brief Draws a pixel at (x, y) with the given color.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color Color value.
 * @return 0 on success, 1 on error.
 */
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line.
 * @param x Starting X coordinate.
 * @param y Y coordinate.
 * @param len Length of the line.
 * @param color Color value.
 * @return 0 on success, 1 on error.
 */
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param color Color value.
 * @return 0 on success, 1 on error.
 */
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Prints an XPM image at (x, y).
 * @param xpm XPM map.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return 0 on success, 1 on error.
 */
int(print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Normalizes a color value for the current mode.
 * @param color Input color.
 * @param new_color Output normalized color.
 * @return 0 on success.
 */
int(normalize_color)(uint32_t color, uint32_t *new_color);

/**
 * @brief Computes direct color mode value.
 */
uint32_t(direct_mode)(uint32_t R, uint32_t G, uint32_t B);

/**
 * @brief Computes indexed color mode value.
 */
uint32_t(indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);

/**
 * @brief Computes red component for color.
 */
uint32_t(Red)(unsigned j, uint8_t step, uint32_t first);

/**
 * @brief Computes green component for color.
 */
uint32_t(Green)(unsigned i, uint8_t step, uint32_t first);

/**
 * @brief Computes blue component for color.
 */
uint32_t(Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first);

/**
 * @brief Extracts red component from color.
 */
uint32_t(R)(uint32_t first);

/**
 * @brief Extracts green component from color.
 */
uint32_t(G)(uint32_t first);

/**
 * @brief Extracts blue component from color.
 */
uint32_t(B)(uint32_t first);

#endif
