#ifndef _LCOM_HELPCONTROLLER_H_
#define _LCOM_HELPCONTROLLER_H_

/**
 * @file helpController.h
 * 
 * @brief This file contains the controllers of the help menu.
*/

#include "controllers/keyboard/kbd.h"
#include "controllers/mouse/mouse.h"
#include "controllers/video_graphics/graphics.h"
#include "controllers/timer/timer.h"
#include "controllers/macros.h"
#include "model/game.h"
#include "view/assetDraw.h"

/**
 * @brief Checks if a given scancode corresponds to the ESC key break code.
 * 
 * @param scancode : The scancode to be checked.
 * @return int : Returns 1 if the ESC key was pressed, otherwise returns 0.
 */
int checkKeyboardInputHelp(uint8_t scancode);

#endif
