/**
 * @file game_state.c
 * @brief Implements the management of the current game state.
 */

#include "game_state.h"

static GameState current_state = STATE_MENU;

/**
 * @brief Sets the current game state.
 * @param state The new game state to set.
 */
void game_set_state(GameState state) {
    current_state = state;
}

/**
 * @brief Gets the current game state.
 * @return The current game state.
 */
GameState game_get_state(void) {
    return current_state;
}
