/**
 * @file instructions
 * 
 * @brief This file contains functions that will draw the instructions menu and the function to get the user back to the main menu if they 
 * click on ESC.
 * 
 * @author João Pedro Sousa, Bernardo Pinto, Carlos Madaleno
 * 
 * @date 28/05/2023  
*/

#ifndef PROJ_INSTRUCTIONS_H
#define PROJ_INSTRUCTIONS_H

#include <lcom/lcf.h>

#include "../devices/keyboard/i8042.h"
#include "../devices/video/video.h"
#include "../sprites/sprites.h"
#include "../MACROS.h"
#include "../devices/mouse/mouse.h"

/**
 * @brief Draws the instructions menu on the screen.
 * 
 * @return 0
*/

int drawInstructionsPage();

/**
 * @brief Processes keyboard input from the user and changes view based on it.
 * 
 * @param scancode represents the key pressed by the user
*/

void process_keyboard_instructions(uint8_t scancode);


#endif //PROJ_INSTRUCTIONS_H




