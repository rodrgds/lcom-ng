#ifndef G6_VIDEO_H
#define G6_VIDEO_H

#include <lcom/lcf.h>

/**
 * @brief Configures the video card for a given mode.
 * @param mode Mode to be configured.
 * @return 0 if successful, non-zero otherwise.
 */
int (vg_start) (uint16_t mode);

/**
 * @brief Sets the video card mode.
 * @param mode Mode to be set.
 * @return 0 if successful, non-zero otherwise.
 */
int (vg_set_mode) (uint16_t mode);

/**
 * @brief Maps the physical video RAM into the process space.
 * @param mode Current mode of the video card.
 * @return 0 if successful, non-zero otherwise.
 */
int (vg_map_vram) (uint16_t mode);

/**
 * @brief Gets the resolution of the current video mode.
 * @param hres Address where the horizontal resolution will be stored.
 * @param vres Address where the vertical resolution will be stored.
 * @return 0 if successful, non-zero otherwise.
 */
int (vg_get_resolution) (uint32_t* hres, uint32_t* vres);

/**
 * @brief Gives a color to a pixel in VRAM.
 * @param x Horizontal coordinates of the pixel.
 * @param y Vertical coordinates of the pixel.
 * @param color Color of the pixel.
 * @return 0 if successful, non-zero otherwise.
 */
int (vg_draw_pixel) (uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line in VRAM.
 * @param x Horizontal coordinates of the line start.
 * @param y Vertical coordinates of the line start.
 * @param len Line length.
 * @param color Line color.
 * @return 0 if successful, non-zero otherwise.
 */
int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a vertical line in VRAM.
 * @param x Horizontal coordinates of the line start.
 * @param y Vertical coordinates of the line start.
 * @param len Line length.
 * @param color Line color.
 * @return 0 if successful, non-zero otherwise.
 */
int (vg_draw_vline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle in VRAM.
 * @param x Horizontal coordinates of the rectangle top left corner.
 * @param y Vertical coordinates of the rectangle top left corner.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param color Line color.
 * @return 0 if successful, non-zero otherwise.
 */
int (vg_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a vertical line with a color pattern in VRAM.
 * @param x Horizontal coordinates of the line start.
 * @param y Vertical coordinates of the line start.
 * @param len Line length.
 * @param color Line color pattern.
 * @return 0 if successful, non-zero otherwise.
 */
int (vg_draw_vline_colormap) (uint16_t x, uint16_t y, uint16_t len, uint32_t* color_map);

/**
 * @brief Draws an XPM image in VRAM.
 * @param x Horizontal coordinates of the top left corner.
 * @param y Vertical coordinates of the top left corner.
 * @param width Image width.
 * @param size Image size.
 * @param pixmap Image pixmap.
 * @return 0 if successful, non-zero otherwise.
 */
int (vg_draw_xpm) (uint16_t x, uint16_t y, int16_t width, uint32_t size, uint8_t* pixmap);

/**
 * @brief Swaps the current VRAM buffer via copying.
 */
void (swap_buffer) ();

/**
 * @brief Frees the second VRAM buffer allocated as dynamic memory.
 */
void (vg_free_buffer) ();

#endif //G6_VIDEO_H
