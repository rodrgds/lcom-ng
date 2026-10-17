#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>

#include "kbc.h"

extern int32_t kbd_subscription_id; /**< Keyboard subscription ID */

/**
 * @brief Subscribes and enables Keyboard interrupts.
 *
 * @param bit_no Will hold the bit (BIT(hook_id)) to be set in the mask returned upon an interrupt.
 * @return Return 0 upon success and non-zero otherwise.
 */
int(kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Keyboard interrupts.
 *
 * @return Return 0 upon success and non-zero otherwise.
 */
int(kbd_unsubscribe_int)(void);

/**
 * @brief Keyboard interrupt handler
 *
 * Tries to read a scan code from the kbc, if any error
 * occurs tries again until READ_ATTEMPT_LIMIT.
 */
void(kbd_int_handler)(void);

/**
 * @brief Reads scan codes from the keyboard.
 *
 * @return Return 0 upon success and non-zero otherwise.
 */
int(kbd_read_scan_code)(void);

/**
 * @brief Checks if the scancode is complete.
 *
 * @return Whether the scancode is complete.
 */
bool(kbd_handle_scan_code)();

#endif /* __KBC_H */
