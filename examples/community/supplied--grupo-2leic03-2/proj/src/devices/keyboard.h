/**
 * @file keyboard.h
 * @brief Keyboard device interface.
 */
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include <stdint.h>

/**
 * @brief Subscribes keyboard interrupts.
 * @param irq_set Pointer to store the IRQ bitmask.
 * @return 0 upon success, non-zero otherwise.
 */
int keyboard_subscribe(uint32_t *irq_set);

/**
 * @brief Unsubscribes keyboard interrupts.
 * @return 0 upon success, non-zero otherwise.
 */
int keyboard_unsubscribe(void);

/**
 * @brief Keyboard interrupt handler.
 */
void keyboard_ih(void);

/**
 * @brief Gets the last read scancode.
 * @return The scancode.
 */
uint8_t keyboard_get_scancode(void);
#endif
