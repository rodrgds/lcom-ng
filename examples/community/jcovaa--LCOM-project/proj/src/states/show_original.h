/**
 * @file show_original.h
 * @brief Functions for managing and displaying the "show original" game state.
 */

#ifndef _SHOW_ORIGINAL_H_
#define _SHOW_ORIGINAL_H_

#include "../game.h"

/**
 * @brief Initializes the "show original" state.
 */
void init_show_original();

/**
 * @brief Initializes the "show original" state in revisit mode.
 */
void init_show_original_revisit();

/**
 * @brief Draws the original drawing and manages the state transition.
 * 
 * @param state Pointer to the current game state variable. Will be updated when the state should change.
 */
void draw_show_original(GameState *state);

#endif /* _SHOW_ORIGINAL_H_ */
