#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdbool.h>
#include <stdint.h>
int size;
uint8_t scanCode[2];
uint8_t statusCode;
int done;

/**
 * @brief Subscribes and enables Keyboard interrupts
 *
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @param hook hookId value
 * @return Return 0 upon success and non-zero otherwise
 */
int keyboard_subscribe(uint8_t *bit_no, int hook);

/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int keyboard_unsubscribe();

/**
 * @brief get the scancode of the button pressed
 * 
 */
void kbc_ih();

#endif /* _KEYBOARD_H_ */
