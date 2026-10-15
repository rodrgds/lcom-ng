#ifndef _MENU_H_
#define _MENU_H_

#include <lcom/lcf.h>

#include "timer_info.h"
#include "kbd.h"
#include "vbe_func.h"

#include "info.h"
#include "controller.h"
#include "sprites.h"

/** @defgroup menu menu
 * @{
 * 
 * Functions for the menu.
 */

/**
 * @brief Main menu loop and logic
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (main_menu)();

/**
 * @brief Draws the initial menu on the console screen
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (displayMenu)();

/**@}*/

#endif /* _MENU_H */
