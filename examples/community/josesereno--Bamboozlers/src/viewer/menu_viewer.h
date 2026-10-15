#ifndef _MENU_VIEWER_H_
#define _MENU_VIEWER_H_

/**
 * @file menu_viewer.h
 * @brief Header file containing function declarations for menu viewer functions.
 */

#include "viewer.h"

/** @defgroup menu_viewer menu_viewer
 * @{
  */
  
extern Sprite* levels;
extern Sprite* instructions;
extern Sprite* logo;
extern Sprite* cursor;

/**
 * @brief Draws the main menu with the menu options.
 *
 * This function draws the main menu on the screen. It displays the logo and menu options
 * for the user to choose from.
 *
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_main_menu)();

/**
 * @brief Draws the menu cursor based on the selected menu option.
 *
 * This function draws the menu cursor on the selected menu option. It updates the cursor and
 * menu sprites based on the current selection.
 *
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_menu_cursor)();
#endif
