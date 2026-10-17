/**
 * @file keyboard.h
 * @brief Interface for handling i8042 keyboard controller operations.
 *
 * This header provides functions for subscribing to keyboard interrupts,
 * handling keyboard input, reading and writing bytes to the keyboard controller,
 * and interpreting scancodes. It defines the key structure and utility functions
 * for scancode processing.
 *
 */

#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>
#include "../i8042.h"

/** @defgroup keyboard keyboard
 * @{
 *
 * @brief Functions for handling the i8042 keyboard
 * @{
 */

/**
 * @brief Structure representing a key and its scancode data.
 */
struct key {
  uint8_t bytes[2]; /**< Scancode bytes of the key */
  int size;         /**< Size of the scancode (1 or 2 bytes) */
};

/**
 * @brief Subscribes and enables Keyboard interrupts.
 *
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * @return Return 0 upon success and 1 otherwise.
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Keyboard interrupts.
 *
 * @return Return 0 upon success and 1 otherwise.
 */
int (kbd_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler.
 *
 * Reads the scancode and stores it in a global variable.
 */
void (kbd_int_handler)();

/**
 * @brief Reads a scancode from the keyboard controller output buffer.
 *
 * @param scan_code Pointer to a variable to store the scancode byte.
 * @return Return 0 upon success and 1 otherwise.
 */
int (kbd_read_scancode)(uint8_t *scan_code);

/**
 * @brief Determines the size of a scancode sequence.
 *
 * @param code First byte of the scancode.
 * @return 1 if it's a single-byte scancode, 2 if it's a multi-byte scancode.
 */
int (kbd_code_size)(uint8_t code);

/**
 * @brief Checks whether a given scancode represents a key press (make code).
 *
 * @param code Scancode to check.
 * @return true if it's a make code, false if it's a break code.
 */
bool (kbd_code_make)(uint8_t code);

/**
 * @brief Reads a byte from the keyboard controller output buffer.
 *
 * @param data Pointer to store the byte read.
 * @return Return 0 upon success and 1 otherwise.
 */
uint8_t (read_byte)(uint8_t *data);

/**
 * @brief Writes a byte to the specified destination (e.g., command or argument register).
 *
 * @param cmd Command or argument byte to write.
 * @param dest Destination port (e.g., KBC command register).
 * @return Return 0 upon success and 1 otherwise.
 */
int (write_byte)(uint8_t cmd, uint8_t dest);

/**
 * @brief Retrieves the last scancode read by the interrupt handler.
 *
 * @return The latest scancode byte stored.
 */
uint8_t (get_scan_code)();

/** @} */

#endif // __KEYBOARD_H 
