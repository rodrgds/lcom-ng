#ifndef DRAW_BUTTONS_H
#define DRAW_BUTTONS_H

#include "../utils/position.h"
#include "../xpm/buttons/button_bucket.xpm"
#include "../xpm/buttons/button_clear_canvas.xpm"
#include "../xpm/buttons/button_eraser.xpm"
#include "../xpm/buttons/button_minus_size.xpm"
#include "../xpm/buttons/button_plus_size.xpm"
#include "../xpm/buttons/button_text.xpm"

#include "element.h"
#include "pixel_buffer.h"

/** @defgroup draw_buttons draw_buttons
 *  @{
 *
 * Constants, structures and functions used for programming the drawing of buttons
 */

#define BUTTON_FRAME_WIDTH 40  /**< @brief the width of the interactive buttons' frame */
#define BUTTON_FRAME_HEIGHT 40 /**< @brief the height of the interactive buttons' frame */
#define BUTTON_FRAME_COLOR 7   /**< @brief the color of the interactive buttons' frame */

/**
 * @brief Draws a button frame on a pixel buffer
 * @param buf the buffer where the frame will be drawn
 * @param topleft_pos the top left position of the frame
 *
 */
void(draw_button_frame)(pixel_buffer const *const buf, position topleft_pos);

/**
 * @brief Draws a color button on a pixel buffer
 * @param buf the buffer where the frame will be drawn
 * @param topleft_pos the top left position of the frame
 * @param color color of the button
 *
 * @return OK if successful, != OK otherwise
 */
int(draw_color_button)(pixel_buffer const *const buf, position topleft_pos, uint8_t color);

/**
 * @brief Loads the button xpms
 *
 */
void(load_button_xpms)();

/**
 * @brief Draws the button to increase the thickness of the cursor on a pixel buffer
 * @param buf the buffer where the frame will be drawn
 * @param topleft_pos the top left position of the frame
 *
 */
void(draw_plus_button)(pixel_buffer const *const buf, position topleft_pos);

/**
 * @brief Draws the button to decrease the thickness of the cursor on a pixel buffer
 * @param buf the buffer where the frame will be drawn
 * @param topleft_pos the top left position of the frame
 *
 */
void(draw_minus_button)(pixel_buffer const *const buf, position topleft_pos);

/**
 * @brief Draws the button to draw a straight line on a pixel buffer
 * @param buf the buffer where the frame will be drawn
 * @param topleft_pos the top left position of the frame
 *
 */
int(draw_sel_line_button)(pixel_buffer const *const buf, position topleft_pos);

/**
 * @brief Draws the button to draw a square on a pixel buffer
 * @param buf the buffer where the frame will be drawn
 * @param topleft_pos the top left position of the frame
 *
 */
int(draw_sel_square_button)(pixel_buffer const *const buf, position topleft_pos);

/**
 * @brief Draws the button to draw a circle on a pixel buffer
 * @param buf the buffer where the frame will be drawn
 * @param topleft_pos the top left position of the frame
 *
 */
int(draw_sel_circle_button)(pixel_buffer const *const buf, position topleft_pos);

/**
 * @brief Draws the button that allows the user to use an eraser on the canvas to a pixel buffer
 * @param buf the buffer where the frame will be drawn
 * @param topleft_pos the top left position of the frame
 *
 */
void(draw_sel_eraser_button)(pixel_buffer const *const buf, position topleft_pos);

/**
 * @brief Draws the button that allows the user to input text into the canvas to a pixel buffer
 * @param buf the buffer where the frame will be drawn
 * @param topleft_pos the top left position of the frame
 *
 */
void(draw_sel_text_button)(pixel_buffer const *const buf, position topleft_pos);

/**
 * @brief Draws the button that allows the user to clear the canvas to a pixel buffer
 * @param buf the buffer where the frame will be drawn
 * @param topleft_pos the top left position of the frame
 *
 */
void(draw_clear_canvas_button)(pixel_buffer const *const buf, position topleft_pos);

/**
 * @brief Draws the buttons on screen
 *
 * @return OK if successful, != OK otherwise
 */
int(buttons_draw)();

/**@}*/

#endif
