#ifndef CURSOR_H
#define CURSOR_H

#include <lcom/lcf.h>
#include <lcom/video_gr.h>
#include <stdbool.h>

#include "../devices/video/video_gr.h"
#include "../utils/color.h"
#include "../utils/const.h"
#include "../utils/position.h"
#include "element.h"

/** @defgroup cursor cursor
 *  @{
 *
 * Constants, structures and functions used for programming the cursor
 */

#define MIN_THICKNESS 1  /**< @brief the minimum thickness of the cursor */
#define MAX_THICKNESS 24 /**< @brief the maximum thickness of the cursor */

/**
 * @brief States that define what the mouse will draw and how it looks when hovering the canvas
 *
 */
typedef enum cursor_style {
  CURSOR_DSTATE_CIRCLE,
  CURSOR_DSTATE_SQUARE,
  CURSOR_DSTATE_TEXT,
  CURSOR_DSTATE_LINE,
  CURSOR_DSTATE_ERASER,
  CURSOR_DSTATE_BUCKET
} cursor_state_style_t;

/**
 * @brief Loads the xpm image of the cursor and sets it's initial coordinates
 *
 */
void(cursor_load)();

/**
 * @brief Moves the cursor around by updating it's coordinates
 * @param delta_x x axis offset
 * @param delta_y y axis offset
 *
 */
void(cursor_move)(int16_t delta_x, int16_t delta_y);

/**
 * @brief Returns the current position of the cursor
 *
 * @return current position of the structure
 */
position(cursor_get_pos)();

/**
 * @brief Gets the current style of the cursor
 *
 * @return cursor style
 */
cursor_state_style_t(cursor_get_style)();

/**
 * @brief Set cursor state
 * @param state state to be set
 *
 */
void(cursor_set_style)(uint8_t style);

/**
 * @brief Get cursor thickness
 *
 * @return cursor thickness
 */
uint8_t(cursor_get_thickness)();

/**
 * @brief Set cursor thickness
 * @param thickness cursor thickness to be set
 *
 */
void(cursor_set_thickness)(uint8_t thickness);

/**
 * @brief Increases the thickness of the cursor
 * @param _ unused
 *
 */
void(cursor_increase_thickness)(uint8_t _);

/**
 * @brief Decreases the thickness of the cursor
 * @param _ unused
 *
 */
void(cursor_decrease_thickness)(uint8_t _);

/**
 * @brief Draws the cursor on screen
 *
 */
void(cursor_draw)();


/**
 * @brief Assigns a new value to the left button of the cursor
 * @param new_lb new value of the left button
 *
 */
void(cursor_set_lb)(bool lb);

/**
 * @brief Indicates whether the left button was pressed or not
 *
 * @return true if the left button was pressed, false otherwise
 */
bool(cursor_lb_was_pressed)();

/**
 * @brief Returns the current color of the cursor
 *
 * @return cursor_color
 */
uint8_t(cursor_get_color)();

/**
 * @brief Sets the cursor color
 * @param color color to be set
 *
 */
void(cursor_set_color)(uint8_t color);

/**
 * @brief Returns the line counter of the cursor
 *
 * @return cursor_line_counter
 */
uint8_t(cursor_get_line_counter)();

/**
 * @brief Returns the intial position of the line to be drawn
 *
 * @return line_initial_position
 */
position(cursor_get_initial_line_position)();

/**
 * @brief Sets the initial position of the line to be drawn
 * @param pos position to be set
 *
 */
void(cursor_set_initial_line_position)(position pos);

/**
 * @brief Increases the counter of the line and indicates the number of clicks done by the user
 *
 */
void(cursor_increase_line_counter)();

/**
 * @brief Returns the initial position of where the text should be printedthe text
 *
 * @return
 */
position(get_text_initial_position)();

/**
 * @brief Getter for the "typing" variable
 *
 * @return true if the user is currently in a state of drawing or editing text, false otherwise
 */
bool(get_is_typing)();

/**
 * @brief Setter for the "typing" variable
 *
 * @param new_is_typing the value "typing" should be set to. Should be true when the user is supposed to be writing text and false otherwise
 */
void(set_is_typing)(bool new_is_typing);

/**
 * @brief Setter for the initial position where the text should start being drawn
 *
 * @param pos position to be set
 */
void(set_text_initial_position)(position pos);

/**@}*/

#endif
