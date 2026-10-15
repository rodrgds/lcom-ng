/**
 * @file gamemode_menu.h
 * @brief Interface for game mode menu logic and state management.
 */

#ifndef _LCOM_GAME_MENU_H_
#define _LCOM_GAME_MENU_H_

#include "controllers/KBC/keyboard.h"
#include "controllers/KBC/mouse.h"
#include "controllers/video/video.h"
#include "view/gamemode_menu_view.h"
#include "model/state.h"
#include "model/modes.h"

/// Stores the currently selected option in the Game Mode menu (0–3)
extern int game_menu_selected_option;

/**
 * @brief Initializes the Game Mode menu.
 * 
 * Resets selection and draws the initial frame.
 */
void gamemode_menu_init();

/**
 * @brief Keyboard interrupt handler for Game Mode menu.
 * 
 * Handles user selection with ENTER based on current option.
 */
void keyboard_game_menu();

/**
 * @brief Mouse interrupt handler for Game Mode menu.
 * 
 * Updates selection based on cursor position and left mouse button click.
 */
void mouse_game_menu();

/**
 * @brief Timer interrupt handler for Game Mode menu.
 * 
 * Draws the menu frame if in the correct mode.
 */
void gamemode_menu_timer_ih();

#endif /* _LCOM_GAME_MENU_H_ */
