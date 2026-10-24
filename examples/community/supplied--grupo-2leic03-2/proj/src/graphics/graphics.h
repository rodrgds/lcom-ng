#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Initializes a VBE graphics mode and allocates the software back buffer.
 *
 * The selected mode is requested with linear framebuffer enabled. Drawing
 * operations write to RAM first; the physical VRAM is updated later with
 * graphics_swap_buffers().
 *
 * @param mode VBE mode number, for example 0x105.
 * @return 0 on success, non-zero otherwise.
 */
int graphics_init(uint16_t mode);

/**
 * @brief Frees graphics resources and returns to text mode.
 *
 * This releases the module back buffer. Higher-level resources such as sprites
 * must be destroyed by their own modules before or around this call.
 *
 * @return 0 on success, non-zero otherwise.
 */
int graphics_exit(void);

/**
 * @brief Fills the entire software back buffer with one color.
 *
 * @param color Color encoded for the current VBE mode.
 */
void graphics_clear_screen(uint32_t color);

/**
 * @brief Copies a full frame-sized buffer into the software back buffer.
 *
 * The source buffer must use the same pitch/scanline layout as the active VBE
 * mode.
 *
 * @param buffer Source buffer with graphics_get_frame_size() bytes.
 */
void graphics_copy_buffer_to_back(const uint8_t *buffer);

/**
 * @brief Copies a full frame-sized buffer into the back buffer with vertical wrap.
 *
 * Useful for scrolling backgrounds without redrawing the whole background
 * pixel by pixel every frame.
 *
 * @param buffer Source buffer with graphics_get_frame_size() bytes.
 * @param y_offset Source row used as the first visible screen row.
 */
void graphics_copy_buffer_to_back_scrolled(const uint8_t *buffer, uint16_t y_offset);

/**
 * @brief Copies the software back buffer into physical VRAM.
 *
 * This is the visible frame swap used for double buffering.
 */
void graphics_swap_buffers(void);

/**
 * @brief Draws one pixel into the software back buffer.
 *
 * @param x Pixel x coordinate.
 * @param y Pixel y coordinate.
 * @param color Color encoded for the current VBE mode.
 * @return 0 on success, non-zero otherwise.
 */
int graphics_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a filled rectangle into the software back buffer.
 *
 * Rectangles are clipped to the screen when they exceed the right or bottom
 * edges. Coordinates are unsigned, so callers should clip negative positions
 * before calling this function.
 *
 * @param x Top-left x coordinate.
 * @param y Top-left y coordinate.
 * @param width Rectangle width in pixels.
 * @param height Rectangle height in pixels.
 * @param color Color encoded for the current VBE mode.
 * @return 0 on success, non-zero otherwise.
 */
int graphics_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a filled rectangle with signed coordinates and full clipping.
 *
 * This is safer for objects that may be partially outside the screen because
 * negative coordinates are clipped before calling graphics_draw_rectangle().
 *
 * @param x Top-left x coordinate. May be negative.
 * @param y Top-left y coordinate. May be negative.
 * @param width Rectangle width in pixels.
 * @param height Rectangle height in pixels.
 * @param color Color encoded for the current VBE mode.
 * @return 0 on success, non-zero otherwise.
 */
int graphics_draw_rectangle_clipped(int x, int y, int width, int height, uint32_t color);

/** @return Active horizontal resolution in pixels. */
uint16_t graphics_get_width(void);

/** @return Active vertical resolution in pixels. */
uint16_t graphics_get_height(void);

/** @return Number of bytes used by one pixel in the active mode. */
uint8_t graphics_get_bytes_per_pixel(void);

/** @return Number of bytes between the start of two consecutive scanlines. */
uint16_t graphics_get_bytes_per_scanline(void);

/** @return Total frame buffer size in bytes, including scanline padding. */
size_t graphics_get_frame_size(void);

#endif
