#ifndef VIDEO_H
#define VIDEO_H

#include <lcom/lcf.h>
#include "sprite.h"

/** @defgroup video
 * @{
 * 
 *  Functions related to the video card. 
 */

#define BG_COLOR 0x512D38
#define PRIMARY_COLOR 0x2CAE87
#define SECONDARY_COLOR 0xFFE9F3
#define ACCENT_COLOR 0xbb5062

#define MARGIN 15

/* Macros */

#define RED(n) ((n >> 16) & 0xff)
#define BLUE(n) ((n >> 8) & 0xff)
#define GREEN(n) (n & 0xff)

/**
 * @brief Map the physical memory needed for a certain mode.
 * @param mode_info Video mode info.
 * @return int 0 upon success, non-zero otherwise
 */
int (map_memory)(vbe_mode_info_t mode_info);

/**
 * @brief Obtains all the information about the specific mode
 * @param mode 2 byte hex value of the intended operating mode.
 * @return int 0 upon success, non-zero otherwise
 */
int (vbe_get_info_from_mode)(uint16_t mode);

/**
 * @brief Change the graphic's card operating mode.
 * @param mode 2 byte hex value of the intended operating mode.
 * @return Pointer to the mapped video memory.
 */
void* (set_mode)(uint16_t mode);

/**
 * @brief Draw a pixel to the screen.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color Pixel's color. Depends on the operating mode.
 * @return int 0 upon success, non-zero otherwise
 */
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draw a pattern to the string.
 * @param no_rectangles Number of rectangles in the matrix (N * N);
 * @param first First color in the pattern.
 * @param step Step of the pattern.
 * @return int 0 upon success, non-zero otherwise
 */
int (draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief Draw a xpm to the screen at a specific coordinate.
 * @param Sprite Sprite struct with image and pix map.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return int 0 upon success, non-zero otherwise
 */
int (draw_sprite)(Sprite sprite, uint16_t x, uint16_t y);

/**
 * @brief Draws a xpm to the screen. Works similar to draw_sprite but color can be defined.
 * @param Sprite Sprite struct with image and pix map.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color The color of the letter.
 * @return int 0 upon success, non-zero otherwise
 */
int(draw_letter)(Sprite sprite, uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Fill the whole screen black.
 * 
 */
void (clear_screen)();

/**
 * @brief Trigger a screen re-draw.
 * @return int 0 upon success, non-zero otherwise
 */
int (vg_refresh)();

/**
 * @brief Set the pixel to be displayed in the upper left corner of the display.
 * 
 * @param start Either 0 or vram_size.
 * @return int 0 upon success, non-zero otherwise.
 */
int vbe_set_display_start(uint16_t start);

/**
 * @brief Get the vertical resolution of the screen.
 * @return int16_t Screen vertical size in pixels.
 */
int16_t (video_get_v_res)(void);

/**
 * @brief Get the horizontal resolution of the screen.
 * @return int16_t Screen horizontal size in pixels.
 */
int16_t (video_get_h_res)(void);

/**@}*/

#endif
