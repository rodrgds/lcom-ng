#ifndef _STATE_H_
#define _STATE_H_

/**
 * @file state.h
 * @brief Header file containing function declarations for state update functions.
 */

#include "menu_state.h"
#include "game_state.h"
#include "instructions_state.h"
#include "game_over_state.h"
#include "level_select_state.h"

/** @defgroup state state
 * @{
  */

typedef enum {
    MENU,
    LEVEL_SELECTION,
    INSTRUCTIONS, 
    GAME,
    GAME_OVER,
    EXIT
} Menu_state;
/**
 * @brief Updates the state based on the keyboard input.
 */
void (update_keyboard_state)();
/**
 * @brief Updates the state based on the mouse input.
 */
void (update_mouse_state)();
/**
 * @brief Updates the state based on the timer interrupt.
 */
void (update_timer_state)();
/**
 * @brief Updates the cursor position based on the mouse input.
 */
void (update_cursor_position)();

//bool select_item(Sprite* item);

#endif
