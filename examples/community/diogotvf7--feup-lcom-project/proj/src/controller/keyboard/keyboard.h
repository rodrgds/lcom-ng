#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <lcom/lcf.h>
#include "../utils.h"
#include "i8042.h"

/**
 * @brief Retrieves the scancode.
 *
 * @return The scancode.
 */
uint8_t (get_scancode)();

/**
 * @brief Subscribes to keyboard interrupts.
 *
 * @param bit_no Pointer to store the bit number associated with the keyboard interrupts.
 * @return 0 upon success, non-zero otherwise.
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from keyboard interrupts.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler.
 */
void (kbc_ih)();

/**
 * @brief Reads the keyboard status.
 *
 * @param st Pointer to store the keyboard status.
 * @return 0 upon success, non-zero otherwise.
 */
int (read_keyboard_status)(uint8_t *st);

/**
 * @brief Reads the scancode from the keyboard.
 *
 * @param st Keyboard status.
 * @param scancode Pointer to store the scancode.
 * @return 0 upon success, non-zero otherwise.
 */
int (read_kbc_scancode)(uint8_t st, uint8_t *scancode);

/**
 * @brief Writes a command to the keyboard controller.
 *
 * @param port Port to write the command to.
 * @param cmd Command to be written.
 * @return 0 upon success, non-zero otherwise.
 */
int (keyboard_write)(int port, uint8_t cmd);

/**
 * @brief Restores the keyboard's default settings.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int (keyboard_restore)();

/**
 * @brief Reads the letter corresponding to the scancode.
 *
 * @param scancode Scancode of the pressed key.
 * @param word_guess Pointer to the word guess array.
 * @param number_letters Pointer to the number of letters in the word.
 * @return 0 upon success, non-zero otherwise.
 */
int read_letter(int scancode, uint8_t *word_guess, int* number_letters);


#endif /* __KEYBOARD_H */
