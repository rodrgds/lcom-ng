#ifndef __KEYBOARD_H
#define __KEYBOARD_H


#include <stdint.h>
#include <stdbool.h>




/** @defgroup keyboard
 * @{
 *
 *
 *
 * Functions for using the keyboard
 */

/**
 * @brief Subscribes and enables keyboard interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_subscribe_int) (uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_unsubscribe_int) ();

/**
 * @brief Reads the kbc status
 * @param st status byte
 * @return Return 1 if an error occurred, 0 otherwise
 */
int (kbc_get_status) (uint8_t *st);

/**
 * @brief Set the command byte of the kbc
 * @param commandByte command byte
 * @return Return 1 if an error occurred, 0 otherwise
 */
int (kbc_set_command_byte) (uint8_t commandByte);

/**
 * @brief Read the command byte of the kbc
 * @param commandByte Stores the byte read from the kbc
 * @return Return 1 if an error occurred, 0 otherwise
 */
int(kbc_get_command_byte)(uint8_t *commandByte);

/**
 * @brief Reads from the output buffer
 * @param buffer Data from the output buffer
 * @return Return 1 if an error occured, 0 otherwise
 */
int (kbc_read_out_buf) (uint8_t *buffer);

/**
  * @brief  Checks if the code is from a make or a break code
  * @return Return 1 if it is a make code, 0 othewise
*/
int (is_make_code) ();

/**
 * @brief Get the size of the scancode
 * @return Size of the scancode
 */
int (get_scancode_size) ();

/**
 * @brief Set the size of the scancode
 * @param size Size of the scancode
 */
void (set_scancode_size) (int size);

/**
 * @brief Get the scancode
 * @return The scancode
 */
uint8_t* (get_scancode)();

/**
 * @brief Checks if the code is 2 byte long
 * @return true if the code is is 1 byte long, false otherwise
 */
int (two_byte_code)();

/**
 * @brief Checks if an error occurred
 * @return 1 if an error occurred, 0 otherwise
 */
int (is_error)();

/**
 * @brief
 * @param
 * @return
 */
int (kbc_populate_buff)(uint8_t buffer);

void delay();

int (kbc_polling)();

int (kbc_write_command)(uint8_t port, uint8_t command);

int (kbc_read_data)(uint8_t *value);

/** @defgroup keyboard_states
 * @{
 *
 * Enumerable containing all possible keyboard states
 */
typedef enum {
    KBD_IN_GAME,
    KBD_GETTING_NICKNAME_1,
    KBD_GETTING_NICKNAME_2,
    KBD_CHOOSING_WORD,
    KBD_GETTING_GUESSES
} KeyboardState;

#endif /* __KEYBOARD_H */
