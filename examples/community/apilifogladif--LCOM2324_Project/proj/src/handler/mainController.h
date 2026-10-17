#ifndef _LCOM_MAINCONTROLLER_H_
#define _LCOM_MAINCONTROLLER_H_

/**
 * @file mainController.h
 * 
 * @brief This file contains the main controllers.
*/

#include "controllers/keyboard/kbd.h"
#include "controllers/mouse/mouse.h"
#include "controllers/video_graphics/graphics.h"
#include "controllers/timer/timer.h"
#include "controllers/sprites/sprite.h"
#include "controllers/serialport/serialport.h"
#include "controllers/macros.h"
#include "model/game.h"

#include "view/gameRemoteDraw.h"
#include "handler/gameRemoteController.h"

#include "view/gameLocalDraw.h"
#include "handler/gameLocalController.h"

#include "view/helpDraw.h"
#include "handler/helpController.h"

#include "view/menuDraw.h"
#include "handler/menuController.h"

#include "view/placingDraw.h"
#include "handler/placingController.h"

/**
 * @brief Enum for the different modes in the menu.
 */
enum MODE {
    MAINMENU,
    PLACINGMENU,
    HELP,
    QUIT,
    GAMELOCAL,
    GAMEREMOTE
};

/**
 * @brief This function is the main function of the game, this is the function that checks the type of interrupt and calls the game functions
 */
void controller();

/**
 * @brief This function is used to subscribe the interrupts.
 */
void subscribe_int();

/**
 * @brief This function is used to unsubscribe the interrupts.
 */
void unsubscribe_int();

/**
 * @brief This function is used to change the mouse position.
 */
void changeMousePosition();

/**
 * @brief This function is used to draw the mouse.
 */
void drawMouse();

/**
 * @brief This function is used to check mouse click.
 */
bool checkClick();

#endif
