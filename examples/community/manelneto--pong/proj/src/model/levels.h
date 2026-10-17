#ifndef _LEVELS_H_
#define _LEVELS_H_

#include <stdbool.h>
#include <stdint.h>

/** @defgroup levels levels
 * @{
 *
 * Functions related to the levels
 */

/**
 * @brief Starts the levels
 *
 * Constructs the buttons (easy, medium and hard) and the cursor.
 *
 * @param xResolution horizontal resolution of the screen in pixels
 * @param yResolution vertical resolution of the screen in pixels
 * @return Return 0 upon success and non-zero otherwise
 */
int start_levels(uint16_t xResolution, uint16_t yResolution);

/**
 * @brief Handles an interrupt from the keyboard when in the levels state
 *
 * Selects/Unselects the buttons
 */
void keyboard_levels_handler();

/**
 * @brief Handles an interrupt from the mouse when in the levels state
 *
 * Moves the cursor and selects/unselects the buttons
 */
void mouse_levels_handler();

/**
 * @brief Checks if the easy button is selected
 *
 * @return Return true if the easy button is selected, false otherwise
 */
bool check_easy();

/**
 * @brief Checks if the medium button is selected
 *
 * @return Return true if the medium button is selected, false otherwise
 */
bool check_medium();

/**
 * @brief Checks if the hard button is selected
 *
 * @return Return true if the hard button is selected, false otherwise
 */
bool check_hard();

/**
 * @brief Ends the levels
 *
 * Destroys the buttons (easy, medium and hard) and the cursor.
 */
void end_levels();

/**@}*/

#endif /* _LEVELS_H_ */
