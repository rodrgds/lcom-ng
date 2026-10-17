#ifndef KEYBOARD_H
#define KEYBOARD_H


#include <lcom/lcf.h>
#include "i8042.h"


/**
 * @brief scancode that we read
 * 
 */
extern uint8_t scancode;

/**
 * @brief indicates if we're done doing a scancode
 * 
 */
extern bool complete_scancode;


/**
 * @brief subscribes keyboard interrupts
 * 
Reads the output buffer of the mouse
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief unsubscribes keyboard interrupts
 * 
 * @return int 0 in case it goes successfully,1 otherwise
 */
int (kbd_unsubscribe_int)();

/**
 * @brief reads the output buffer of the keyboard
 * 
 */
void (kbc_ih)();

/**
 * @brief reads the status register and the output buffer, verifying if everything we're doing is valid
 *
 * @return int 0 in case it goes successfully,1 otherwise 
 */
int (kbc_get_scancode)();


/**
 * @brief checks if we're reading a 2 bytes or 1 byte scancode
 *
 */
void (kbc_verify_scancode)();


#endif
