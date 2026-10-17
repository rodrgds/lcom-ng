#ifndef MENU_H
#define MENU_H

/** @defgroup state menu
 * @{
 *
 * Functions related to the menu state.
 */

#include <lcom/lcf.h>
#include "driver.h"
#include "proj.h"

#define TITLE_POS_Y 200
#define BUTTON_POS_Y 350

/**
 * @brief Starts up the menu state.
 * There's no need to manually call this function, 
 * as it is used after the first call to menu_step.
 * 
 */
void menu_init();

/**
 * @brief Draws the menu to the screen.
 * 
 */
void menu_draw();

/**
 * @brief Handle an event.
 * 
 * @param event Event struct.
 */
void menu_handle_event(Event event);

/**
 * @brief Handles events and draws the menu.
 * 
 * @param event Last event that occurred.
 */
void menu_step(Event event);

/**@}*/

#endif
