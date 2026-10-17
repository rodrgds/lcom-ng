/**
 * @file results.h
 * @brief Functions for managing and displaying the results state.
 */

#ifndef _RESULTS_H_
#define _RESULTS_H_

#include <lcom/lcf.h>
#include "../game.h"
#include <stdbool.h>

/**
 * @brief Initializes the results state.
 * 
 * @param success Whether the user successfully recreated the drawing.
 */
void init_results(bool success, int level);

/**
 * @brief Draws the results screen.
 * 
 * @param state Pointer to the current game state.
 */
void draw_results(GameState *state);

/**
 * @brief Handles the action for the next level or restart option.
 * 
 * @param state Pointer to the current game state.
 */
void handle_next_restart_action(GameState *state);

/**
 * @brief Handles keyboard input for the results state.
 * 
 * @param state Pointer to the current game state.
 */
void handle_results_input(GameState *state);


#endif // _RESULTS_H_
