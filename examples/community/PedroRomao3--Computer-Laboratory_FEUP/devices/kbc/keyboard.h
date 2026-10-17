/**
 * @file keyboard.h
 * @brief ...
 * @author musta
 * @date 15/04/2023
 */

#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "KBC.h"

#define mask_keyboard BIT(1)

/**
 @brief Subscribes keyboard interrupts.
This function subscribes to keyboard interrupts, sets the appropriate policy, and
stores the bit mask for the interrupt notification in the provided pointer.
Time Complexity: O(1) - Constant time complexity.
 @param bit_no uint8_t* - Pointer to store the bit mask for interrupt notifications.
 @return int - Returns 0 upon success and non-zero otherwise.
*/
int (keyboard_subscribe_int)(uint8_t *bit_no);


/**
 @brief Unsubscribes keyboard interrupts.
This function unsubscribes from keyboard interrupts and removes the interrupt policy.
Time Complexity: O(1) - Constant time complexity.
 @return int - Returns 0 upon success and non-zero otherwise.
*/
int (keyboard_unsubscribe_int)();


/**
 @brief Keyboard Controller Interrupt Handler.
This function reads the scancode from the KBC output and handles any errors that occur.
Time Complexity: O(1) - Constant time complexity.
*/
void (kbc_ih)();

/**
 @brief Gets the corresponding letter from the scancode.
This function converts the scancode of a key to the corresponding
lowercase letter, if the scancode represents a letter key.
Time Complexity: O(1) - Constant time complexity.
 @return The corresponding lowercase letter, backspace ('\b') if the scancode is for the backspace key,     or a null character ('\0') if the scancode does not represent a letter key.
*/
char getLetter();

/**
 @brief Checks if a character is a lowercase letter.
This function determines if a given character is a lowercase letter in the English alphabet.
 @param l The character to check.
 @return true if the character is a lowercase letter, false otherwise.
*/
bool isl(char l);

/**
 @brief Checks if a character is a digit.
This function determines if a given character is a digit (0-9).
 @param l The character to check.
 @return true if the character is a digit, false otherwise.
*/
bool isd(char l);

#endif
