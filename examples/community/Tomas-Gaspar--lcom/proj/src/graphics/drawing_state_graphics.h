#ifndef _DRAWING_STATE_GRAPHICS_H_
#define _DRAWING_STATE_GRAPHICS_H_

#include "../cursor.h"

/**
 * @ingroup Graphics
 * @{
 * 
 * @defgroup drawing_state_graphics Drawing State Graphics
 * 
 * @brief Drawing State Graphics module
 * 
 * This module provides functionality for handling graphics related to the drawing state. It includes enums for representing different buttons
 * and drawing states, as well as functions for handling drawing gestures, changing states, interpreting states, and clicking on buttons.
 * 
 * @{
*/

/**
 * @brief Enum representing different buttons available in the drawing stat
 */
typedef enum {
    B_RED,      /*!< Red button */
    B_YELLOW,   /*!< Yellow button */
    B_BLUE,     /*!< Blue button */
    B_GREEN,    /*!< Green button */
    B_BLACK,    /*!< Black button */
    B_WHITE,    /*!< White button */
    B_CLEAR,    /*!< Clear button */
    B_SMALL,    /*!< Small button */
    B_MEDIUM,   /*!< Medium button */
    B_LARGE,    /*!< Large button */
    NO_BUTTON   /*!< No button */
} button;

/**
 * @brief Enum representing different states in the drawing state.
 */
typedef enum {
    /** @brief Init Drawing*/
    INIT_DRAWING,
    /** @brief Idle Other
     * 
     * State to block a left click starting on some place affecting somwhere else
    */
    IDLE_OTHER,
    /** @brief Idle Draw
     * 
     * State to allow the drawing close to the limits without having to release and click again on the left button
    */
    IDLE_DRAW,
    /** @brief Draw
     * 
     * State for drawing
    */
    DRAW,
    /** @brief Button
     * 
     * State representing the pressing of a button
    */
    BUTTON,
    /** @brief Button action
     * 
     * State representing the releasing of a button and corresponding action
    */
    B_PAINT
} drawing_state;

/**
 * @brief Drawing movement enumeration
 * 
 * Enum representing different types of movement.
*/
typedef enum {
    /**
     * @brief Left draw
     * 
     * Left click on a drawing area, which is the canvas where the user can draw
    */
    L_DRAW,
    /**
     * @brief Left button
     * 
     * Left click on a button
    */
    L_BUTTON,
    /**
     * @brief Left other
     * 
     * Left click on some place that is neither a button nor a drawing area
    */
    L_OTHER,
    /**
     * @brief No Left draw
     * 
     * Represents that the left button is not being pressed and the cursor is on a drawing area
    */
    NL_DRAW,
    /**
     * @brief No Left button
     * 
     * Represents that the left button is not being pressed and the cursor is on a button
    */
    NL_BUTTON,
    /**
     * @brief Left other
     * 
     * Represents that the left button is not being pressed and the cursor is on some place that is neither a button nor a drawing area
    */
    NL_OTHER
} drawing_movement;

/**
 * @brief Handle drawing gestures
 * 
 * Based on the cursor position and on the left button status, it handles the drawing gestures
 * 
 * @param cursor Pointer to the cursor
 * @param lb The left button status (true if pressed, false otherwise)
 * @param delta_x The delta movement in the X direction
 * @param delta_y The delta movement in the Y direction
*/
void (drawing_gesture_handler)(Cursor* cursor, bool lb, int16_t delta_x, int16_t delta_y);

/**
 * @brief Change the drawing state
 * 
 * Based on a movement and on the current state, it updates the current state
 * 
 * @param state Pointer to the drawing state variable
*/
void (drawing_change_state)(drawing_state* state);

/**
 * @brief Interpret the drawing state
 * 
 * Interprets the current state and makes some action according to it
 * 
 * @param cursor Pointer to the cursor
 * @param state Pointer to the drawing state variable
*/
void (drawing_interpret_state)(Cursor* cursor, drawing_state* state);

/**
 * @brief Click on a button
 * 
 * Checks if the cursor is on a button and returns the corresponding button enum value.
 * 
 * @param cursor Pointer to the cursor
 * @return The button enumeration value if the cursor is on a button, NO_BUTTON otherwise
*/
button (click_on_button)(Cursor* cursor);

/**
 *  }@
 * }@
*/

#endif
