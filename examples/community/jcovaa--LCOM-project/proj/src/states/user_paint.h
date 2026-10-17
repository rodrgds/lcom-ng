/**
 * @file user_paint.h
 * @brief Functions for managing and displaying the user painting state.
 */

#ifndef _USER_PAINT_H_
#define _USER_PAINT_H_

#include "../game.h"

/**
 * @brief Initializes the user paint state, clearing the grid and resetting the timer.
 */
void init_user_paint();

/**
 * @brief Draws the user paint interface, handles the timer, and displays win/lose result.
 * 
 * @param state Pointer to the current game state variable.
 */
void draw_user_paint(GameState *state);

/**
 * @brief Handles mouse input for painting on the grid.
 */
void handle_mouse_painting();

/**
 * @brief Handles keyboard input for the user paint state.
 * 
 * @param state Pointer to the current game state variable.
 */
void handle_user_paint_input(GameState *state);

#endif /* _USER_PAINT_H_ */
