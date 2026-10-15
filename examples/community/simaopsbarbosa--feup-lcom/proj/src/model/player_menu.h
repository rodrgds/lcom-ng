/**
 * @file player_menu.h
 * @brief Interface for player menu logic and state management.
 */

#ifndef _LCOM_PLAYER_MENU_H_
#define _LCOM_PLAYER_MENU_H_

#include "controllers/KBC/keyboard.h"
#include "controllers/KBC/mouse.h"
#include "controllers/video/video.h"
#include "view/player_menu_view.h"
#include "model/state.h"
#include "model/modes.h"

/// Stores the currently selected option in the player menu (0–3)
extern int player_menu_selected_option;

/**
 * @brief Initializes the Player Menu.
 * 
 * Sets default values and draws the initial player menu frame.
 */
void player_menu_init();

/**
 * @brief Keyboard interrupt handler for the Player Menu.
 * 
 * Handles menu option selection via keyboard input.
 */
void keyboard_player_menu();

/**
 * @brief Mouse interrupt handler for the Player Menu.
 * 
 * Handles menu option selection via mouse cursor and left click.
 */
void mouse_player_menu();

/**
 * @brief Timer interrupt handler for the Player Menu.
 * 
 * Periodically redraws the player menu screen if in player menu mode.
 */
void player_menu_timer_ih();

#endif /* _LCOM_PLAYER_MENU_H_ */
