#ifndef KBD_H
#define KBD_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
#include "kbc.h"

/**
 * @file kbd.h
 * 
 * @brief Keyboard management
 * Functions to interact with the keyboard.
 */

/**
 * @brief Returns a pointer to the scancode byte read
 * 
 * @return Pointer to the scancode byte
 */
uint8_t* get_scancode();

/**
 * @brief Returns a pointer to the scancode array
 * 
 * @return Pointer to the scancode array
 */
uint8_t* get_scancode_array();

/**
 * @brief Returns the index of the scancode array
 * 
 * @return Index of the scancode array
 */
uint8_t get_scancode_index();

/**
 * @brief Subscribes keyboard interrupts
 * 
 * @param bit_no Pointer to store the bit number set in the interrupt mask
 * @return 0 on success, 1 on failure
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 * 
 * @return 0 on success, 1 on failure
 */
int (kbd_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler
 */
void (kbc_ih)();

/**
 * @brief Checks if a scancode is complete
 * 
 * @return 0 if the scancode is complete, 1 otherwise
 */
int (check_scancode_complete)();

/**
 * @brief Enables keyboard interrupts
 * 
 * @return 0 on success, 1 on failure
 */
int (kbd_enable_interrupts)();

/**@}*/

#endif /* KBD_H */
