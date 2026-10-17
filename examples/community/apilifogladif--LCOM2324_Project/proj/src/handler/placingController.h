#ifndef _LCOM_PLACINGCONTROLLER_H_
#define _LCOM_PLACINGCONTROLLER_H_

/**
 * @file placingController.h
 * 
 * @brief This file contains the placing boats controllers.
 */

#include "controllers/keyboard/kbd.h"
#include "controllers/mouse/mouse.h"
#include "controllers/video_graphics/graphics.h"
#include "controllers/timer/timer.h"
#include "controllers/macros.h"
#include "model/boats.h"
#include "model/game.h"
#include "view/assetDraw.h"

/**
 * @brief Checks and processes mouse clicks in pop up menus in placing menu.
 * 
 * @return int : -1 to go to main menu, 0 to stay in placing menu and 1 to start the game.
 */
int checkPlacingClick();

/**
 * @brief Checks the keyboard input during the placing menu.
 * 
 * @param scancode : The scancode of the key pressed.
 */
void checkKeyboardInputPlacing(uint8_t scancode);

#endif
