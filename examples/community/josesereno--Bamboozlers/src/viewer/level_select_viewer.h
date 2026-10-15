#ifndef _LEVEL_SELECT_VIEWER_H_
#define _LEVEL_SELECT_VIEWER_H_

/**
 * @file level_select_viewer.h
 * @brief Header file containing function declarations for level select viewer functions.
 */

#include "viewer.h"

/** @defgroup level_select_viewer level_select_viewer
 * @{
  */
  
extern Sprite* level_one;
extern Sprite* level_two;
extern Sprite* level_three;
/**
 * @brief Draws the level selection menu with the level options.
 *
 * This function draws the level selection menu on the screen. It displays the level options
 * for the user to choose from.
 *
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_level_select_menu)();
/**
 * @brief Draws the level cursor based on the selected level.
 *
 * This function draws the level cursor on the selected level option. It updates the cursor and
 * level sprites based on the current level state.
 *
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_level_cursor)();
#endif
