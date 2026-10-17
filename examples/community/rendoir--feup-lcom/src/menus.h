/**
	@file menus.h
	@author Daniel Marques
	@author João Almeida

	Prototype of the menus.c function
*/


#ifndef MENUS_H
#define MENUS_H
/** @defgroup menus menus
 * @{
 *
 * Functions responsible for handling the menu buttons
 */

#include "game.h"
/*!
  @brief Receives information about key pressed and current menu option, and changes it accordingly
  @param current_option Current option of the menu
  @param key Make/Break code of key pressed
  @param enter_pressed boolean to tell wether ENTER has been pressed or not

  @return 2 if option changed , 1 if START/RESUME game was pressed , 0 if EXIT was pressed
 */
unsigned int menuHandler(options_t *current_option, unsigned long int *key , unsigned char *enter_pressed	);



#endif
