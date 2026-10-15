/**
 * @file video.h
 * @brief Video initialisation, double buffering, and primitive drawing.
 */
#ifndef _TECLAS_VIDEO_H_
#define _TECLAS_VIDEO_H_

#include <lcom/lcf.h>

/** @brief Current screen width in pixels. */
extern uint16_t screen_width;

/** @brief Current screen height in pixels. */
extern uint16_t screen_height;

/**
 * @brief Initialize the video subsystem in the given VBE mode.
 *
 * @param mode VBE mode number.
 * @return 0 on success, non-zero on failure.
 */
int video_init(uint16_t mode);

/**
 * @brief Swap the back buffer to the screen (double buffering).
 */
void video_flip();

/**
 * @brief Clear the back buffer with a solid color.
 *
 * @param color 24-bit RGB color value.
 */
void video_clear(uint32_t color);

/**
 * @brief Draw a single pixel on the back buffer.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color 24-bit RGB color value.
 * @return 0 on success, non-zero if out of bounds.
 */
int video_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draw a horizontal line on the back buffer.
 *
 * @param x Starting X coordinate.
 * @param y Y coordinate.
 * @param len Length in pixels.
 * @param color 24-bit RGB color value.
 * @return 0 on success, non-zero on failure.
 */
int video_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draw a filled rectangle on the back buffer.
 *
 * @param x Top-left X coordinate.
 * @param y Top-left Y coordinate.
 * @param w Width in pixels.
 * @param h Height in pixels.
 * @param color 24-bit RGB color value.
 * @return 0 on success, non-zero on failure.
 */
int video_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color);

/**
 * @brief Blit a full-screen background buffer to the back buffer.
 *
 * @param buf Pre-rendered background pixel data.
 */
void video_draw_background(const uint8_t *buf);

/**
 * @brief Draw a pre-cached XPM image at the given position.
 *
 * @param colors Cached pixel data from video_cache_xpm().
 * @param img XPM image metadata.
 * @param x X coordinate on screen.
 * @param y Y coordinate on screen.
 * @return 0 on success, non-zero on failure.
 */
int video_draw_xpm_cached(uint8_t *colors, xpm_image_t img, uint16_t x, uint16_t y);

/**
 * @brief Convert an XPM map into a cached pixel buffer.
 *
 * @param xpm XPM map source data.
 * @return Pointer to the allocated pixel buffer, or NULL on failure.
 */
uint8_t *video_cache_xpm(xpm_map_t xpm);

/**
 * @brief Capture the current back buffer to start a dissolve transition.
 */
void video_start_dissolve();

/**
 * @brief Draw one frame of a dissolve transition.
 *
 * @param tick Current tick within the transition.
 * @param max_ticks Total number of ticks in the transition.
 */
void video_draw_dissolve(int tick, int max_ticks);

#endif
