#ifndef _LCOM_MENUCONTROLLER_H_
#define _LCOM_MENUCONTROLLER_H_

/**
 * @file menuController.h
 * 
 * @brief This file contains the menu controllers.
*/

#include "controllers/keyboard/kbd.h"
#include "controllers/mouse/mouse.h"
#include "controllers/video_graphics/graphics.h"
#include "controllers/timer/timer.h"
#include "controllers/rtc/rtc.h"
#include "controllers/sprites/sprite.h"
#include "controllers/macros.h"
#include "mainController.h"

/**
 * @brief Checks if a click is within the bounds of any main menu button.
 * 
 * @return bool : True if a button was clicked, false otherwise.
 */
bool checkMainClick();

/**
 * @brief Handles keyboard input in the main menu.
 * 
 * @param scancode : Scancode of the key pressed.
 * @return int : 0 upon success.
 */
int checkKeyboardInputMainMenu(uint8_t scancode);

#endif
