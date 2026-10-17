/*
* @file keyboard.h
* @brief Keyboard driver interface
*/
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>
#include <stdint.h>

/** @brief Counter for keyboard controller interrupts */
extern uint32_t counter_KBC;

/** @brief Last received keycode from keyboard */
extern uint8_t last_keycode;

/**
 * @brief Subscribe keyboard interrupts
 * @param bit_no Pointer to store the bit number for interrupt mask
 * @return 0 on success, non-zero on error
*/
extern uint32_t counter_KBC;

/// Last processed scancode from the keyboard
extern uint8_t last_keycode;

/**
 * @brief Subscribes to keyboard interrupts (IRQ1).
 * 
 * @param[out] bit_no Pointer to store the bit number of the IRQ line.
 * @return 0 on success, non-zero on failure.
 */
int kbd_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribe keyboard interrupts
 * @return 0 on success, non-zero on error
 */
int kbd_unsubscribe_int();

/**
 * @brief Keyboard interrupt handler
 * Reads the scancode from the keyboard controller and updates internal state.
 */
void kbd_ih();

/**
 * @brief Get scancode from keyboard
 * @param scancode Pointer to store the retrieved scancode
 * @return true if scancode was successfully retrieved, false otherwise
 */
bool keyboard_get_scancode(uint8_t *scancode);

#endif
