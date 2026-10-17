/**
 * @file keyboard.h
 * 
 */

#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <stdbool.h>
#include <stdint.h>

#include "KBC.h"

/* IRQ */
#define KBD_IRQ 1

/* SCANCODES */
#define KBD_ESC_BREAKCODE 0x81
#define KBD_2B_SCANCODE 0xE0
#define breakcode(makecode) (makecode | BIT(7))
#define is_makecode(scancode) !(scancode & BIT(7))

/**
 * @brief struct representing the keyboard data the KBC provides
 * @param two_byte indicates if the current scancode is part of a two-byte scancode or not
 * @param valid indicates if the data is valid
 * @param error indicates if there was an error retrieving the data
 */
typedef struct {
    uint8_t scancodes[2];
    bool two_byte;
    bool valid;
    bool error;
} kbd_data_t;

// for interrupts
/**
 * @brief attempts to enables keyboard interrupts
 * @param wait_ticks the number of attempts that will be made
 * @return zero on success, non-zero otherwise
 */
int (kbd_enable_int)(uint32_t wait_ticks);

/**
 * @brief subscribes the keyboard interrupts
 * @param bit_no pointer to a number representing the position dedicated to the keyboard in the interrupt vector
 * @return zero on success, non-zero otherwise
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief unsubscribes keyboard interrupts
 * @return zero on success, non-zero otherwise
 */
int (kbd_unsubscribe_int)();

/**
 * @brief attempts to read a scancode from the KBC
 * @param kbd_data the struct where the scancode will be placed
 * @param wait_ticks the number of attempts that will be made
 */
void (kbd_get_scancode)(kbd_data_t* kbd_data, uint32_t wait_ticks);

#endif // _LCOM_KEYBOARD_H_
