#ifndef _LCOM_GAMELOCALCONTROLLER_H_
#define _LCOM_GAMELOCALCONTROLLER_H_

/**
 * @file gameLocalController.h
 * 
 * @brief This file contains the controllers of the local game.
*/

#include "controllers/keyboard/kbd.h"
#include "controllers/mouse/mouse.h"
#include "controllers/video_graphics/graphics.h"
#include "controllers/timer/timer.h"
#include "controllers/macros.h"
#include "model/boats.h"
#include "view/gameLocalDraw.h"
#include "model/game.h"
#include "view/assetDraw.h"

/**
 * @brief Checks the keyboard input during the local game.
 * 
 * @param scancode : The scancode of the key pressed.
 * @return int : Returns 1 if the ESC key was pressed, otherwise returns 0.
 */
int checkKeyboardInputGameLocal(uint8_t scancode);

/**
 * @brief Checks and processes mouse clicks during the local game.
 */
void checkGameLocalClick();

#endif
