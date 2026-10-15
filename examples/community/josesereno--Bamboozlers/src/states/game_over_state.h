#ifndef _GAME_OVER_STATE_H_
#define _GAME_OVER_STATE_H_

/**
 * @file game_over_state.h
 * @brief Header file containing function declarations for game over state update functions.
 */


#include "controller/KBC/KBC.h"
#include "controller/mouse/mouse.h"
#include "controller/keyboard/keyboard.h"
#include "controller/video/graphics.h"
#include "viewer/viewer.h"
#include "state.h"

/** @defgroup game_over_state game_over_state
 * @{
  */

extern Sprite* cursor;

/**
 * @brief Updates the keyboard input in the game over state.
 * 
 * Reads the current scancode and updates the menu state based on the pressed key.
 */
void (update_keyboard_game_over)();
/**
 * @brief Updates the mouse input in the game over state.
 * 
 * Checks the mouse button status and updates the menu state based on the clicked item.
 */
void (update_mouse_game_over)();

#endif
