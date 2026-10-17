#ifndef _LCOM_PLACINGDRAW_H_
#define _LCOM_PLACINGDRAW_H_

/**
 * @file placingDraw.h
 * 
 * @brief This file contains the placing boats menu drawing functions.
*/

#include "controllers/keyboard/kbd.h"
#include "controllers/mouse/mouse.h"
#include "controllers/video_graphics/graphics.h"
#include "controllers/timer/timer.h"
#include "controllers/macros.h"
#include "model/boats.h"
#include "model/game.h"
#include "assetDraw.h"

/**
 * @enum PLACINGSTATE
 * @brief Placing menu state enum
*/
enum PLACINGSTATE {
    ASKINGQUIT,
    PLAYER1,
    ASKINGPLAYER2,
    PLAYER2,
    ASKINGGAME,
};

/**
 * @brief Draws the placing menu on the screen.
 */
void drawPlacingMenu();

/**
 * @brief Draws the previous state of placing menu on the screen.
 */
void drawPreviousPlacingMenu();

/**
 * @brief Draws the placing menu cursor on the screen.
 */
void drawPlacingCursor();

#endif
