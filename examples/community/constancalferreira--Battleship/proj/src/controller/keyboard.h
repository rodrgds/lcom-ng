#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "KBC.h"

/**
 * @brief Keyboard interrupt handler. 
 * 
 * Reads the scancode from the output buffer.
 */
void (kbc_ih)();

/**
 * @brief Subscribes keyboard interrupts.
 * 
 * @param bit_no Pointer to store the hook ID bit mask.
 * @return 0 on success, non-zero otherwise.
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (kbd_unsubscribe_int)();

/**
 * @brief Restores the keyboard to its default state (used in polling mode).
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (kbd_restore)();

/**
 * @brief Processes a scancode, handling multi-byte sequences.
 * 
 * @param scancode The latest byte of the scancode to process.
 */
void (process_scancode)(uint8_t scancode);

#endif
