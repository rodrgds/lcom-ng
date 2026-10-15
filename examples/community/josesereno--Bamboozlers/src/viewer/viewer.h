#ifndef _VIEWER_H_
#define _VIEWER_H_

/**
 * @file viewer.h
 * @brief Header file containing function declarations for viewer functions.
 */

#include "controller/video/graphics.h"
#include "model/model.h"
#include "states/state.h"
#include "game_viewer.h"
#include "menu_viewer.h"
#include "instructions_viewer.h"
#include "game_over_viewer.h"
#include "level_select_viewer.h"

/** @defgroup viewer viewer
 * @{
  */
/**
 * @brief Draws a sprite on the screen.
 *
 * This function draws a sprite on the screen at the specified position. It iterates over the
 * sprite's pixmap array and draws each pixel on the screen using the specified color.
 *
 * @param sprite The sprite to be drawn.
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_sprite)(Sprite *sprite);

/**
 * @brief Draws the current menu on the screen.
 *
 * This function draws the current menu on the screen based on the current menu state.
 * It calls the corresponding menu drawing functions based on the menu state.
 *
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_menu)();

/**
 * @brief Draws the cursor on the screen based on the current menu state.
 *
 * This function draws the cursor on the screen based on the current menu state.
 * It calls the corresponding cursor drawing functions based on the menu state.
 *
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_cursor)();

#endif
