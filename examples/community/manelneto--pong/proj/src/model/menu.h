#ifndef _MENU_H_
#define _MENU_H_

#include <stdbool.h>
#include <stdint.h>

/** @defgroup menu menu
 * @{
 *
 * Functions related to the menu
 */

/**
 * @brief Starts the menu
 *
 * Constructs the buttons (play and quit) and the cursor. Initializes global variables (x_max, y_max, x and y).
 *
 * @param xResolution horizontal resolution of the screen in pixels
 * @param yResolution vertical resolution of the screen in pixels
 * @return Return 0 upon success and non-zero otherwise
 */
int start_menu(uint16_t xResolution, uint16_t yResolution);

/**
 * @brief Handles an interrupt from the keyboard when in the menu state
 *
 * Selects/Unselects the buttons
 */
void keyboard_menu_handler();

/**
 * @brief Handles an interrupt from the mouse when in the menu state
 *
 * Moves the cursor and selects/unselects the buttons
 */
void mouse_menu_handler();

/**
 * @brief Checks if the play button is selected
 *
 * @return Return true if the play button is selected, false otherwise
 */
bool check_play();

/**
 * @brief Checks if the quit button is selected
 *
 * @return Return true if the quit button is selected, false otherwise
 */
bool check_quit();

/**
 * @brief Ends the menu
 *
 * Destroys the buttons (play and quit) and the cursor.
 */
void end_menu();

/**@}*/

#endif /* _MENU_H_ */
