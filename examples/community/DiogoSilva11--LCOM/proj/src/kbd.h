#ifndef __KBD_H
#define __KBD_H

#include <stdbool.h>
#include <stdint.h>

/** @defgroup kbd kbd
 * @{
 *
 * Functions for using the i8042 keyboard controller
 */

/**
 * @brief Read the KBC state
 *
 * @param stat a pointer to the variable that is to be updated with the status byte
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_state)(uint8_t *stat);

/**
 * @brief Issuing a command or argument to the KBC
 *
 * @param info command byte or argument
 * @param arg indicates if an argument is being passed
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_cmd)(uint8_t info, bool arg);

uint8_t scancode[2]; // array with the scancode bytes

/**
 * @brief Reads scancode byte from the KBC's output buffer
 * 
 * @return Return 0 upon success and false otherwise
 */
int(kbd_scancode)();

/**
 * @brief Read return value/data from the KBD
 *
 * @param data a pointer to the variable that is to be updated with the return data
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_ret_value)(uint8_t *data);

/**
 * @brief Gets the size of the scancode (assuming there is one)
 * 
 * @return Return 2 if it's a two byte scancode and 1 otherwise
 */
int(kbd_scancode_size)();

/**
 * @brief Checks if the scancode is a make or a break code
 * 
 * @return Return true if it's make and false otherwise
 */
bool(kbd_make)();

/**
 * @brief Checks if a two byte scancode is incomplete (second byte yet to store)
 * 
 * @return Return true if incomplete and false otherwise
 */
bool(kbd_inc_code)();

/**
 * @brief Checks if the scancode corresponds to the ESC break code
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_esc_break)();

/**
 * @brief Subscribes and enables KBD interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes KBD interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_unsubscribe_int)();

/**
 * @brief Read the scan codes by polling
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_poll)();

/**
 * @brief Enables KBD interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbd_enable_int)();

/**
 * @brief Get the relevant byte of scancode 
 *
 * @return Return the relevant byte of scancode
 */

uint8_t(get_key)();

/**@}*/

#endif /* __KBD_H */
