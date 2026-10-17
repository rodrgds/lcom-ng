/**
 * @file state_menu.h
 * @brief Header file for the menu state.
 */

#ifndef _STATE_MENU_H
#define _STATE_MENU_H

#include <lcom/lcf.h>
#include "../game/game_utils.h"
#include "../visual/visual_menu.h"
#include "../game/game_play.h"

/**
 * Relevant sprites for the main menu.
 */
extern sprite_t *menu_exit;
extern sprite_t *menu_title;
extern sprite_t *menu_background;
extern sprite_t *menu_levels;
extern sprite_t *menu_level_1;
extern sprite_t *menu_level_2;
extern sprite_t *menu_level_3;
extern sprite_t *menu_back;

/**
 * Handles keyboard input in the main menu.
 * @param scancode The scancode of the pressed key.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_keyboard_in_menu)(uint8_t scancode);

/**
 * Handles keyboard input in the pre-level menu.
 * @param scancode The scancode of the pressed key.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_keyboard_in_pre_level)(uint8_t scancode);

/**
 * Handles mouse input in the main menu.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_mouse_in_menu)();

/**
 * Handles mouse input in the pre-level menu.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_mouse_in_pre_level)();

/**
 * Checks if the mouse is over a sprite.
 * @param s The sprite to check.
 * @return true if the mouse is over the sprite, false otherwise.
 */
bool (mouse_over_sprite)(sprite_t *s);

/**
 * Handles keyboard input in the scoreboard menu.
 * @param scancode The scancode of the pressed key.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_keyboard_in_scoreboard)(uint8_t scancode);

/**
 * Handles mouse input in the scoreboard menu.
 * @return 0 upon success, non-zero otherwise.
 */
int (handle_mouse_in_scoreboard)();

#endif
