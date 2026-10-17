#ifndef _LCOM_MENUDRAW_H_
#define _LCOM_MENUDRAW_H_

/**
 * @file menuDraw.h
 * 
 * @brief This file contains the menu drawing functions.
*/

#include "controllers/keyboard/kbd.h"
#include "controllers/mouse/mouse.h"
#include "controllers/video_graphics/graphics.h"
#include "controllers/timer/timer.h"
#include "controllers/rtc/rtc.h"
#include "controllers/sprites/sprite.h"
#include "controllers/macros.h"
#include "handler/mainController.h"

/**
 * @brief Draws the main menu with different themes based on the current time.
 */
void drawMainMenu();

/**
 * @brief Draws the appropriate cursor based on its position relative to menu buttons.
 */
void drawMainCursor();

#endif
