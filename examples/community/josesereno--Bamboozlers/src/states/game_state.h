#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

/**
 * @file game_state.h
 * @brief Header file containing function declarations for game state update functions.
 */


#include "controller/game/map_controller.h"
#include "controller/game/item_controller.h"
#include "controller/game/panda_controller.h"
#include "viewer/viewer.h"
#include "state.h"

/** @defgroup game_state game_state
 * @{
  */
/**
 * @brief Updates the game state based on the keyboard input.
 */
void (update_keyboard_game)();
/**
 * @brief Updates the game state based on the mouse input.
 */
void (update_mouse_game)();
/**
 * @brief Updates the game state based on the timer interrupt.
 */
void (update_timer_game)();
/**
 * @brief Updates the state of the panda character.
 */
void (update_panda_state)();

#endif
