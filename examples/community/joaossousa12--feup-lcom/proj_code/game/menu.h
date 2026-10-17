/**
 * @file menu
 * 
 * @brief This file contains functions that will draw the main menu and the function that allows a user to either go to the game in itself (press 1 on the keyboard),
 * go to the instructions menu (press 2 on the keyboard) and quit the game (press 3 on the keyboard).
 * 
 * @author João Pedro Sousa, Bernardo Pinto, Carlos Madaleno
 * 
 * @date 28/05/2023  
*/
#ifndef PROJ_MENU_H
#define PROJ_MENU_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <string.h>

#include "../devices/keyboard/i8042.h"
#include "../devices/video/video.h"
#include "../sprites/sprites.h"
#include "../devices/mouse/mouse.h"
#include "../MACROS.h"
#include "game.h"

/**
 * @brief Draws the main menu on the screen.
 * 
 * @return 0 on success, 1 if there was an error drawing the menu.
*/

int drawMenu();

/**
 * @brief Processes keyboard input from the user and changes view based on it.
 * 
 * @param scancode represents the key pressed by the user
*/

void process_keyboard_menu(uint8_t scancode);

#endif //PROJ_MENU_H
