#ifndef VIDEO_GR_H
#define VIDEO_GR_H

/** @defgroup video_gr video_gr
 *  @{
 *
 * Functions used for the programming of the video graphics card
 */

/**
 * @brief Draws a pixel on a given position
 * @param x x coordinate of the pixel
 * @param y y coordinate of the pixel
 * @param color color of the pixel
 */
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a circle on a given position
 * @param x x coordinate of the center of the circle
 * @param y y coordinate of the center of the circle
 * @param radius radius of the circle
 * @param color color of the circle
 *
 */
int(vg_draw_circle)(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);

/**
 * @brief Draws a line on a given position
 * @param from_x starting position x coordinate
 * @param from_y starting position y coordinate
 * @param to_x final position x coordinate
 * @param to_y final position y coordinate
 * @param color color of the line
 *
 */
int(vg_draw_line)(uint16_t from_x, uint16_t from_y, uint16_t to_x, uint16_t to_y, int color);

/**
 * @brief Draws an xpm image on a given position
 * @param xpm_info xpm image to be drawn
 * @param x x coordinate
 * @param y y coordinate
 *
 */
int(vg_draw_xpm)(const xpm_image_t xpm_info, uint16_t x, uint16_t y);

/**
 * @brief Loads an xpm map and returns an xpm image containing relevant information
 * @param map xpm map to be loaded
 *
 * @return xpm_image
 */
xpm_image_t(vg_load_xpm)(const xpm_map_t map);

/**
 * @brief Fills the screen with a given color
 * @param color color to fill the screen with
 *
 */
void(clear_screen)(uint8_t color);

/**
 * @brief Switches the back buffer with the front buffer
 *
 */
void(flip)();

/**
 * @brief Returns the back buffer
 *
 * @return back_buffer
 */
void *(vg_get_back_buffer) ();

/**@}*/

#endif
