/**
 * @file state_game.h
 * @brief Header file for the game state.
 */

#ifndef _STATE_GAME_H
#define _STATE_GAME_H

#include <lcom/lcf.h>
#include "../game/game_utils.h"
#include "../game/game_play.h"
#include "../visual/visual_menu.h"
#include "../sprites/sprites.h"
#include "../visual/visual.h"
#include "../states/state_menu.h"
#include "../game/game_play.h"

/**
 * Handles keyboard input during gameplay.
 * @param scancode The scancode of the pressed key.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_keyboard_in_level)(uint8_t scancode);

/**
 * Handles keyboard input while the game is paused.
 * @param scancode The scancode of the pressed key.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_keyboard_in_paused)(uint8_t scancode);

/**
 * Handles mouse input during gameplay.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_mouse_in_level)();

/**
 * Handles mouse input while the game is paused.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_mouse_in_paused)();

/**
 * Handles mouse input during the game.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_mouse_in_game)();

#endif
