/**
 * @file game_over_controller
 * @brief This file has the implementation of the functions related to the game over controller.
 */

#ifndef _GAME_OVER_CONTROLLER_
#define _GAME_OVER_CONTROLLER_

#include <lcom/lcf.h>
#include "model/cursor/cursor_input.h"

/**
 * @brief Processes keyboard inputs within the game over context.
 * 
 * @param scan_code An array of scan codes to process.
 * @param size The size of the scan_code array.
 *
 * This function listens for keyboard inputs when in the game over state. 
 * It changes the state to MENU when the ESC key is pressed, 
 * and to GAME when the ENTER key is pressed.
 * 
 * @return void.
 */
void game_over_kbd(uint8_t scan_code[2], int size);

/**
 * @brief Handles cursor inputs within the game over context.
 * 
 * @param input The cursor input to handle.
 *
 * This function manages mouse click events in the game over screen. 
 * If the 'Play Again' option is clicked, the state is changed to GAME. 
 * If the 'Exit' option is clicked, the state is set to MENU.
 * 
 * @return void.
 */
void game_over_cursor(CursorInput* input);

#endif
