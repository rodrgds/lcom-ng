#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>

#include <math.h>

#include "../../game/charset.h"
#include "../../game/cursor.h"
#include "../../game/draw_text.h"
#include "../../game/pixel_buffer.h"
#include "../../game/text.h"
#include "../../utils/color.h"
#include "../../utils/const.h"
#include "../../utils/position.h"
#include "video_gr.h"

/** @defgroup graphics graphics
 *  @{
 *
 * Functions used for the programming of the graphics
 */

void(load_backbuffer)();

/**
 * @brief Returns the back buffer
 *
 */
pixel_buffer const *(get_back_buffer) ();

/**
 * @brief Draws a pixel on a pixel_buffer
 *
 * @param buf buffer where the pixel is going to be drawn
 * @param pos position of the pixel on screen
 * @param color color of the pixel
 *
 * @return 0 if successful, non-zero otherwise
 */
int(buf_draw_pixel)(pixel_buffer const *const buf, position pos, uint8_t color);

/**
 * @brief Draws a line on a pixel buffer
 * @param buf buffer where the line is going to be drawn
 * @param from_pos starting position of the line
 * @param to_pos final position of the line
 * @param color color of the line
 *
 * @return 0 if successful, non-zero otherwise
 */
int(buf_draw_line)(pixel_buffer const *const buf, position from_pos, position to_pos, uint8_t color);

/**
 * @brief Draws a rectangle on a pixel buffer
 * @param pos upper-left corner coordinates of the rectangle on screen
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param color color of the rectangle
 *
 * @return 0 if successful, non-zero otherwise
 */
int(buf_draw_rectangle)(pixel_buffer const *const buf, position pos, uint16_t width, uint16_t height, uint8_t color);

/**
 * @brief Draws a circle on a pixel buffer
 * @param pos coordinates of the center of the circle on screen
 * @param radius radius of the circle
 * @param color color of the circle
 *
 * @return 0 if successful, non-zero otherwise
 */
int(buf_draw_circle)(pixel_buffer const *const buf, position pos, uint16_t radius, uint8_t color);

/**
 * @brief Draws an horizontal line on a pixel buffer
 * @param pos starting position of the line
 * @param len length of the line
 * @param color color of the line
 *
 * @return true if successful, false otherwise
 */
int(buf_draw_hline)(pixel_buffer const *const buf, position pos, uint16_t len, uint8_t color);

/**
 * @brief Draws a vertical line on a pixel buffer
 * @param pos starting position of the line
 * @param len length of the line
 * @param color color of the line
 *
 * @return true if successful, false otherwise
 */
int(buf_draw_vline)(pixel_buffer const *const buf, position pos, uint16_t len, uint8_t color);

/**
 * @brief Draws a XPM to the given pixel buffer.
 *
 * @param buf the buffer to which the XMP will be drawn
 * @param xpm_info info regarding the XPM to draw
 * @param pos position to draw the given XPM
 *
 * @return 0 on success
 */
int(buf_draw_xpm)(pixel_buffer const *const buf, const xpm_image_t xpm_info, position pos);

/**
 * @brief draws the given string to the given buffer on the given position.
 *
 * @param buf the buffer to which the text will be drawn
 * @param text the text to draw
 * @param pos the position in which to draw the text
 * @param alignment the alignment of the text relative to the chosen position
 *
 * @return 0 on success
 */
int(buf_draw_text)(pixel_buffer const *const buf, char *text, position pos, text_alignment_t alignment);

/**@}*/

#endif
