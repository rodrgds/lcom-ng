/**
 * @file instructions.h
 * @brief Functions for managing and displaying the instructions state.
 */

#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include <lcom/lcf.h>
#include "../game.h"

/**
 * @brief Draws the instructions screen.
 */
void draw_instructions();

/**
 * @brief Handles keyboard input for the instructions state.
 *
 * @param state Pointer to the current game state variable. Will be updated when the state should change.
 */
void handle_instructions_input(GameState *state);

#endif /* _INSTRUCTIONS_H_ */
