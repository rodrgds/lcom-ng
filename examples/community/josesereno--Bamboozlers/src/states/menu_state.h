#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

/**
 * @file menu_state.h
 * @brief Header file containing function declarations for menu state update functions.
 */

#include "controller/KBC/KBC.h"
#include "controller/mouse/mouse.h"
#include "controller/keyboard/keyboard.h"
#include "controller/video/graphics.h"
#include "viewer/viewer.h"
#include "state.h"

/** @defgroup menu_state menu_state
 * @{
  */
extern Sprite* cursor;
/**
 * @brief Updates the menu state based on the keyboard input in the main menu.
 */
void (update_keyboard_menu)();

/**
 * @brief Updates the menu state based on the mouse input in the main menu.
 */
void (update_mouse_menu)();

#endif
