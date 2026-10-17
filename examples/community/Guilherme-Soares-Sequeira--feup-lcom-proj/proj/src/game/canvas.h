#ifndef CANVAS_H
#define CANVAS_H

#include <lcom/lcf.h>

#include "../devices/kbc/mouse.h"
#include "../devices/video/graphics.h"
#include "../devices/video/video_gr.h"
#include "../utils/color.h"
#include "cursor.h"
#include "pixel_buffer.h"

/** @defgroup canvas canvas
 *  @{
 *
 * Constants and functions used for programming the canvas
 */

#define CANVAS_MARGIN 60                         /**< @brief the margin of the canvas */
#define CANVAS_WIDTH (H_RES - CANVAS_MARGIN * 2) /**< @brief the width of the canvas relative to the window dimensions */
#define CANVAS_HEIGHT (708)                      /**< @brief the height of the canvas relative to the window dimensions */

#define CANVAS_TOP_VISIBLE_LIMIT CANVAS_MARGIN                      /**< @brief the top limit of the canvas*/
#define CANVAS_BOTTOM_VISIBLE_LIMIT (CANVAS_MARGIN + CANVAS_HEIGHT) /**< @brief the bottom limit of the canvas */
#define CANVAS_LEFT_VISIBLE_LIMIT CANVAS_MARGIN                     /**< @brief the left limit of the canvas */
#define CANVAS_RIGHT_VISIBLE_LIMIT (H_RES - CANVAS_MARGIN)          /**< @brief the right limit of the canvas */

#define CANVAS_BYPP 1 /**< @brief number of bytes that each canvas pixel takes in physical memory */

/**
 * @brief Loads canvas information and allocates the necessary memory
 *
 */
void(canvas_load)();

/**
 * @brief Returns the canvas buffer
 *
 * @return canvas_buffer
 */
pixel_buffer const *(get_canvas_buffer) ();

/**
 * @brief Draws the canvas to the back buffer
 *
 */
void(canvas_draw)();

/**
 * @brief Fills the entire screen with the given color and updates the "canvas_background_color" variable
 * @param color color to fill the area with
 *
 */
void(canvas_fill)(uint8_t color);

/**
 * @brief Fills the canvas with a white color
 *
 */
void(canvas_clear)();

/**
 * @brief Handles mouse events that happen within the canvas section of the screen
 * @param _ unused
 *
 */
void(canvas_mouse_handler)(uint8_t _);

/**
 * @brief Draws a circle on screen at the given cursor position
 *
 * @return 0 if success, 1 otherwise
 */
int(canvas_draw_pencil_circle)();

/**
 * @brief Draws a square on screen at the given cursor position
 *
 * @return 0 if success, 1 otherwise
 */
int(canvas_draw_pencil_square)();

/**
 * @brief Handles the drawing of straight lines made by clicking on an initial and final positions
 *
 */
void(canvas_handle_line)();

/**
 * @brief Handles the filling of the canvas with the cursor color
 *
 */
void(canvas_handle_bucket)();

/**
 * @brief Function that is supposed to replace an area of color "color_to_replace_with" with a new color "color_to_replace". Utilizes a linked list / queue but is unused at the moment
 *
 * @param curr_pos the position of the first pixel to be replaced
 *
 * @param color_to_replace the color that will be replaced
 *
 * @param color_to_replace_with the color that will be used to replace the other color
 */
void(canvas_flood_fill)(position curr_pos, uint8_t color_to_replace, uint8_t color_to_replace_with);

/**
 * @brief Handles how clicks on the canvas while the cursor is in text state should behave
 *
 */
void(canvas_handle_text)();


/**@}*/

#endif
