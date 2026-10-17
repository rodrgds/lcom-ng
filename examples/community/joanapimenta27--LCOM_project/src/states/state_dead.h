/**
 * @file state_dead.h
 * @brief Header file for the dead state.
 */

#ifndef _STATE_DEAD_H
#define _STATE_DEAD_H

#include <lcom/lcf.h>
#include "../game/game_utils.h"
#include "../game/game_play.h"
#include "../visual/visual_menu.h"
#include "../sprites/sprites.h"
#include "../visual/visual.h"
#include "../states/state_menu.h"

/**
 * Handles keyboard input in the dead state.
 * @param scancode The scancode of the pressed key.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_keyboard_in_dead)(uint8_t scancode);

/**
 * Handles mouse input in the dead state.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_mouse_in_dead)();

#endif
