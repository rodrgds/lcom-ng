/**
 * @file menu
 * @brief Functions for the game menu
 */

#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

/** @defgroup Menu Menu
 * @{
 *
 * Functions for the game menu
 */

#include "../game_definition.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/keyboard/kbc.h"
#include "../game_utils/game_utils.h"

/**
 * @brief Display on the screen the elements to create the menu page
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (display_menu_page)();

/**
 * @brief Display the mouse used for navigate on the menu
 *
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 * @return Return 0 upon success and 1 otherwise
 */
int (menu_display_mouse)(int x, int y);

/**
 * @brief Controls what the mouse can do in the menu page
 *
 * @param pp The mouse parsed packet to check its click
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 */
void (menu_control_mouse)(struct packet *pp, int x, int y);

/**
 * @brief Controls what the keyboard can do in the menu page
 *
 * @param bytes The scancode read from the keyboard
 */
void (menu_control_kbc)(uint8_t *bytes);

/**@}*/

#endif
