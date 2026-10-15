/**
 * @file name_controller
 * @brief This file has the implementation of the functions related to the name controller.
 */

#ifndef _KEYBOARD_CONTROLLER_H_
#define _KEYBOARD_CONTROLLER_H_

#include "../drivers/keyboard/keyboard.h"

/**
 * @brief Processes keyboard inputs within the player name context.
 * 
 * @param scan_code An array of scan codes to process.
 * @param size The size of the scan_code array.
 *
 * This function listens for keyboard inputs when in the player name state. 
 * It changes the state to MENU when the ESC key is pressed, and to GAME 
 * when the ENTER key is pressed. For any other key press, it calls 
 * letter_input function to process the keystroke.
 */
void (player_name_kbd)(uint8_t scan_code[2], int size);

/**
 * @brief Processes a keyboard scan code into a corresponding letter.
 * @param code The scan code to process.
 * 
 * This function translates a scan code into a character corresponding to 
 * a keyboard key, and adds the character to the players name input. 
 * The space bar adds a space to the name, and backspace deletes the 
 * last character from the name.
 * 
 * @return Returns 0 upon successful letter conversion, and 1 if the letter 
 * is the same as the previous keystroke.
 */
int (letter_input)(unsigned char code);

#endif

