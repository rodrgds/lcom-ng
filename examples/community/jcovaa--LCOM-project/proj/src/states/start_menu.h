/**
 * @file start_menu.h
 * @brief Functions for managing and displaying the start menu state.
 */

#ifndef _START_MENU_H_
#define _START_MENU_H_

#include <lcom/lcf.h>
#include "../game.h"

/**
 * @brief Initializes the start menu state.
 */
void init_start_menu();

/**
 * @brief Draws the start menu on the screen.
 */
void draw_start_menu();

/**
 * @brief Handles keyboard input for the start menu.
 * 
 * @param state Pointer to the current game state variable. Will be updated when the state should change.
 */
void handle_menu_input(GameState *state);

#endif /* _START_MENU_H_ */
