#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Returns the last scancode byte read by kbc_ih().
 */
uint8_t (kbc_get_scancode)(void);

/**
 * @brief Returns true if the last kbc_ih() call produced a hardware error.
 */
bool (kbc_get_error)(void);

/**
 * @brief Subscribes keyboard IRQ notifications with IRQ_EXCLUSIVE policy.
 * @param bit_no Pointer to store the hook bitmask.
 * @return 0 on success, 1 on failure.
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard IRQ notifications.
 * @return 0 on success, 1 on failure.
 */
int (kbd_unsubscribe_int)(void);

/**
 * @brief Keyboard interrupt handler. Reads one byte from the KBC output buffer
 * and stores it for kbc_get_scancode(). Sets the error flag on failure.
 */
void (kbc_ih)(void);

/**
 * @brief Reads a scancode in polling mode (busy-wait until OBF is set).
 * @return 0 on success, 1 on error or timeout.
 */
int (kbc_read_scancode_poll)(void);

/**
 * @brief Assembles complete make/break scancodes from successive kbc_ih() calls.
 *
 * Handles the 0xE0 two-byte prefix transparently.  Call after every kbc_ih()
 * that did not set the error flag.
 *
 * @param is_two_bytes  In/out: true while waiting for the second byte of a 2-byte code.
 * @param size          Out: 1 or 2 depending on scancode length.
 * @param bytes         Out: raw scancode bytes (bytes[0] and optionally bytes[1]).
 * @param make          Out: true = make (key press), false = break (key release).
 * @param is_esc_pressed Out: set to true when ESC break code is detected.
 * @return true when a complete scancode is ready; false if still waiting for more bytes.
 */
bool (kbc_process_scancode)(bool *is_two_bytes, uint8_t *size, uint8_t bytes[], bool *make, bool *is_esc_pressed);

/**
 * @brief Re-enables the keyboard interrupt in the KBC command byte.
 *
 * Must be called when unsubscribing in polling mode to restore normal
 * keyboard operation.
 *
 * @return 0 on success, 1 on failure.
 */
int (kbc_restore_keyboard)(void);

#endif /* _LCOM_KEYBOARD_H_ */
