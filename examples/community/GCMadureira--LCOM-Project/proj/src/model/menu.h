/** 
 * @file menu.h
 * @brief Header file for @ref menu.c
 * 
 * This file declares all the functions to be used for the menu struct.
 */


#ifndef _MENU_PROJ_H_
#define _MENU_PROJ_H_

#include <lcom/lcf.h>

#include "../drivers/drivers.h"
#include "../viewer/static_images.h"
#include "structs.h"

/**
 * @brief Moves the selected option of the menu up
 * 
 * If the option is out of bounds, it circles back to the other end.
 * 
 * @param menu The pointer to the menu to change
 */
void (menu_select_option_up)(menu* menu);

/**
 * @brief Moves the selected option of the menu down
 * 
 * If the option is out of bounds, it circles back to the other end.
 * 
 * @param menu The pointer to the menu to change
 */
void (menu_select_option_down)(menu* menu);

/**
 * @brief Creates a main menu
 * 
 * Dynamically allocates memory for the menu and sets its initial state.
 *
 * @return The pointer to the new menu
 */
menu* (menu_create_main)();

/**
 * @brief Creates a game over menu
 * 
 * Dynamically allocates memory for the menu and sets its initial state.
 *
 * @return The pointer to the new menu
 */
menu* (menu_create_game_over)();

/**
 * @brief Destroys a menu by freeing its memory
 * 
 * Frees the memory of the sprite array and then the menu itself.
 * 
 * @param menu The pointer to the menu to be destroyed
 * @return 0 in case of success, 1 otherwise
 */
int (menu_destroy)(menu* menu);

#endif /*_MENU_PROJ_H_ */
