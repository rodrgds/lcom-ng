#ifndef UTILS_H
#define UTILS_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i8042_kbd.h"
#include "kbd.h"

/**
 * @brief Checks if a given character is printable.
 *
 * This function checks if the provided character is printable, meaning it corresponds 
 * to a visible character that can be displayed on the screen.
 *
 * @param ch The character to check.
 * @return Returns true if the character is printable, false otherwise.
 */
bool is_char(char ch);

/**
 * @brief Converts a keyboard keycode to its corresponding character.
 *
 * This function takes a keyboard keycode and returns the corresponding character.
 * Note: This function assumes a specific keyboard layout and may not work correctly 
 * for all keycodes.
 *
 * @param keycode The keycode to convert.
 * @return Returns the corresponding character if found, otherwise returns '\0'.
 */
char get_key(uint8_t keycode);


#endif
