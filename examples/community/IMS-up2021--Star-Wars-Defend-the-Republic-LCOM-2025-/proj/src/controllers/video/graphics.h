#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include "VBE.h" // Assuming VBE.h contains the definition for vbe_mode_info_t

/**
 * @file graphics.h
 * @brief Functions for graphics mode manipulation, drawing, and buffer management.
 */

/** @defgroup graphics Graphics Library
 * @{
 *
 * Provides functions to initialize VBE graphics modes, draw pixels and pixmaps,
 * manage frame buffers (including support for double buffering), and return to text mode.
 */

/* External Global Variables */

extern vbe_mode_info_t mode_info;   /**< @brief Global structure holding VBE mode information for the current graphics mode. */
extern uint8_t* frame_buffer;       /**< @brief Pointer to the secondary buffer used for drawing operations (for double buffering). */
                                    /**< Content from this buffer is copied to video RAM by vg_swap_buffers(). */
extern const uint8_t xpm_source_bpp;/**< @brief Bits per pixel of the source XPM images being handled or expected by drawing functions. */


/**
 * @brief Initializes the specified VBE graphics submode.
 *
 * This function sets the video controller to the graphics mode identified by `submode`.
 * It retrieves mode information, maps the video RAM, and initializes the drawing buffer.
 *
 * @param submode The VBE mode number to set (e.g., VBE_768p_INDEXED, VBE_600p_DC).
 * @return int 0 upon success, non-zero otherwise.
 */
int (set_graphic_mode)(uint16_t submode);

/**
 * @brief Restores the system to the default text mode.
 *
 * This function is typically called to clean up graphics settings before program termination,
 * returning the display to standard text mode.
 *
 * @return int 0 upon success, non-zero otherwise.
 */
int (set_text_mode)();

/**
 * @brief Swaps the drawing buffer with the display buffer (implements double buffering).
 *
 * Copies the contents of the secondary buffer (pointed to by `frame_buffer`, where drawing occurs)
 * to the primary frame buffer in video RAM, making the drawn content visible.
 *
 * @return int 0 upon success, non-zero otherwise.
 */
int (vg_swap_buffers)();

/**
 * @brief Sets up the frame buffer(s) for the specified graphics mode.
 *
 * This function is typically called internally by `set_graphic_mode`.
 * It handles the mapping of the primary video RAM and the allocation of the
 * secondary drawing buffer (`frame_buffer`) based on the mode's characteristics.
 *
 * @param mode The VBE graphics mode for which to set up the frame buffer.
 * @return int 0 upon success, non-zero otherwise.
 */
int (set_frame_buffer)(uint16_t mode);

/**
 * @brief Draws a single pixel at the specified coordinates with the given color.
 *
 * The pixel is drawn on the secondary drawing buffer (pointed to by `frame_buffer`).
 * The color format (e.g., indexed, direct RGB) depends on the current graphics mode.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel. The interpretation of this value depends on the video mode's color depth and memory model.
 * @return int 0 upon success, non-zero otherwise (e.g., if coordinates are out of bounds).
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a pixmap, scaling it to fit the target dimensions on the screen.
 *
 * This function takes raw pixmap data and renders it at the specified screen coordinates,
 * applying scaling to match the `screen_target_width` and `screen_target_height`.
 * Drawing occurs on the secondary buffer.
 *
 * @param pixmap_data Pointer to the raw pixel data of the source image.
 * @param original_width Width of the source pixmap in pixels.
 * @param original_height Height of the source pixmap in pixels.
 * @param src_bytes_per_pixel Bytes per pixel of the source pixmap data.
 * @param screen_target_x X-coordinate on the screen where the top-left of the scaled pixmap will be drawn.
 * @param screen_target_y Y-coordinate on the screen where the top-left of the scaled pixmap will be drawn.
 * @param screen_target_width The desired width of the pixmap on the screen after scaling.
 * @param screen_target_height The desired height of the pixmap on the screen after scaling.
 * @return int 0 upon success, non-zero otherwise.
 */
int (vg_draw_scaled_pixmap)(uint8_t *pixmap_data, uint16_t original_width, uint16_t original_height, uint8_t  src_bytes_per_pixel,
    uint16_t screen_target_x, uint16_t screen_target_y, uint16_t screen_target_width, uint16_t screen_target_height);

/**@}*/

#endif /* __GRAPHICS_H */
