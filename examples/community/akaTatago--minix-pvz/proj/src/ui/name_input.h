/**
 * @file name_input.h
 * @brief Text input system for the player's name.
 */

#ifndef __NAME_INPUT_H
#define __NAME_INPUT_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME_LEN 12

/**
 * @brief Initializes the name input state and clears the buffer.
 */
void name_input_init();

/**
 * @brief Handles keyboard input during the name entry screen.
 * * @param scancode The hardware scancode.
 * @return true when ENTER is pressed to confirm the name, false otherwise.
 */
bool name_input_on_key(uint8_t scancode);

/**
 * @brief Renders the name input screen and cursor.
 */
void name_input_render();

/**
 * @brief Retrieves the current player name.
 * * @return A const pointer to the player name string.
 */
const char* name_input_get_name();

/**
 * @brief Retrieves the length of the current player name.
 * * @return Integer representing the string length.
 */
int name_input_get_length();

#endif
