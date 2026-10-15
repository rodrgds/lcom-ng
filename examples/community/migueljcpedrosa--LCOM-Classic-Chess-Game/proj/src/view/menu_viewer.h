/**
 * @file menu_viewer
 * @brief This file has the implementation of the functions related to the initial menu viewer.
 */

#ifndef _MENUVIEWER_H_
#define _MENUVIEWER_H_

#include "../drivers/video_card/gpu.h"
#include "sprite.h"

/**
 * @brief Draws buttons on the menu screen.
 * 
 * This function draws the play button and the exit button on the menu screen.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int draw_buttons();

/**
 * @brief Draws title on menu screen.
 * 
 * This function iterates over the words in the title and draws them on the menu screen.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int draw_title();

/**
 * @brief Takes a screenshot of the menu.
 * 
 * This function cleans the screen, draws the menu, and copies the buffer to a screenshot.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (menu_screenshot)();

/**
 * @brief Draws the complete menu.
 * 
 * This function draws the background, title, and buttons of the menu.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int draw_menu();

/**
 * @brief Refreshes the menu screen.
 * 
 * This function erases the date and cursor, draws the new date and cursor, 
 * and finally renders the screen.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int menu_refresh();

#endif
