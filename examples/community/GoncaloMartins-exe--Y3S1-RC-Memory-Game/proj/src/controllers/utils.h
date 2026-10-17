#ifndef UTILS_H
#define UTILS_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "keyboard_mouse/i8042.h"
#include "timer/i8254.h"
#include "keyboard_mouse/kbc.h"
#include "keyboard_mouse/keyboard.h"


/**
 * @brief Retrieves the Least Significant Byte (LSB) of a 16-bit value.
 *
 * @param val The 16-bit value.
 * @param lsb Pointer to store the resulting 8-bit LSB.
 * @return 0 on success, non-zero otherwise.
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Retrieves the Most Significant Byte (MSB) of a 16-bit value.
 *
 * @param val The 16-bit value.
 * @param msb Pointer to store the resulting 8-bit MSB.
 * @return 0 on success, non-zero otherwise.
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Reads an 8-bit value from the specified port using sys_inb.
 *
 * @param port The I/O port to read from.
 * @param value Pointer to store the read value.
 * @return 0 on success, 1 on error.
 */
int util_sys_inb(int port, uint8_t *value);

/**
 * @brief Polls the keyboard controller for output.
 *
 * @param data Pointer to store the output data.
 * @return 0 if data was read successfully, 1 otherwise.
 */
int poll_kbc(uint8_t *data);

/**
 * @brief Enables keyboard interrupts by modifying the command byte.
 *
 * @return 0 on success, 1 on failure.
 */
int enable_kbd_interrupts();

/**
 * @brief Updates the cursor position based on the keycode (arrow keys).
 *
 * @param keycode The keycode indicating the direction of movement.
 */
void update_cursor_position(uint8_t keycode); // Update based on keycode

/**
 * @brief Waits for the ESC key to be pressed, using keyboard interrupts.
 *
 * @return 0 on ESC press and proper exit, 1 on error.
 */
int ESC_wait();     // Espera até que seja clicada a tecla ENTER.

#endif // UTILS_H
