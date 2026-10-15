/**
 * @file game_over_menu.h
 * @brief Interface for game over menu logic and state management.
 */

#ifndef _LCOM_GAME_OVER_MENU_H_
#define _LCOM_GAME_OVER_MENU_H_

#include <lcom/lcf.h>
#include "controllers/KBC/keyboard.h"
#include "controllers/KBC/mouse.h"
#include "controllers/video/video.h"
#include "view/game_over_view.h"
#include "model/state.h"
#include "model/modes.h"

/// Stores the currently selected option in the Game Over menu (0–1) 
extern int game_over_menu_selected_option;

/**
 * @brief Initializes the Game Over menu.
 * 
 * Resets selection and draws the Game Over screen.
 */
void game_over_init();

/**
 * @brief Keyboard interrupt handler for Game Over menu.
 * 
 * Handles user input with ENTER to choose an option.
 */
void keyboard_game_over_menu();

/**
 * @brief Mouse interrupt handler for Game Over menu.
 * 
 * Updates option selection and handles click.
 */
void mouse_game_over_menu();

/**
 * @brief Timer interrupt handler for Game Over menu.
 * 
 * Redraws the Game Over screen periodically.
 */
void game_over_timer_ih();

#endif /* _LCOM_GAME_OVER_MENU_H_ */
