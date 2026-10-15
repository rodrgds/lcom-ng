/**
 * @file menu_controller
 * @brief This file has the implementation of the functions related to the menu controller.
 */

#ifndef _MENU_CONTROLLER_H_
#define _MENU_CONTROLLER_H_

#include <lcom/lcf.h>
#include "model/cursor/cursor_input.h"

/**
 * @brief Processes keyboard inputs within the menu context.
 * 
 * @param scan_code An array of scan codes to process.
 * @param size The size of the scan_code array.
 *
 * This function listens for keyboard inputs when in the menu state.
 * It changes the state to EXIT when the ESC key is pressed, 
 * and to WRITE_NAME when the ENTER key is pressed.
 */
void menu_kbd(uint8_t scan_code[2], int size);

/**
 * @brief Handles cursor inputs within the menu context.
 * 
 * @param input The cursor input to handle.
 *
 * This function manages mouse click events in the menu. 
 * If the 'Play' menu option is clicked, the state is changed to WRITE_NAME. 
 * If the 'Exit' menu option is clicked, the state is set to EXIT.
 */
void menu_cursor(CursorInput* input);

#endif
