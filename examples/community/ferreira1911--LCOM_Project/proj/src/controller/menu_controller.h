#ifndef MENU_CONTROLLER_H
#define MENU_CONTROLLER_H

#include <lcom/lcf.h>

#include "model/menu.h"

#define NUM_MENU_ITEMS 5 /**< @brief Total number of menu items */

/**
 * @brief Initializes all the menu items with their respective positions and images.
 * 
 * This function must be called before rendering or interacting with the menu.
 */
void (menu_controller_init)();

/**
 * @brief Draws all visible menu items on the screen.
 * 
 * Should be called after initializing the menu items.
 */
void (menu_controller_draw)();

/**
 * @brief Clears the screen, draws the menu and the crosshair, and swaps the buffer.
 * 
 * @return 0 on success, 1 on failure.
 */
int (draw_menu_elements)();

/**
 * @brief Checks if a given (x, y) coordinate hits any clickable menu item.
 * 
 * @param x X coordinate of the click.
 * @param y Y coordinate of the click.
 * @return Index of the clicked menu item or 0 if none.
 */
int (menu_controller_check_click)(int x, int y);

/**
 * @brief Main loop of the menu.
 * 
 * Handles keyboard and mouse events to navigate the menu and select game modes.
 * 
 * @return 0 on successful transition to game or quit, 1 on error.
 */
int (menu_loop)();

#endif
