/**
 * @file keyboard.h
 * @brief function declarations to configure the keyboard
 */

#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>
#include "../helper/i8042.h"

extern uint8_t scancode;

/**
 * @brief subscribes keyboard interrupts
 * 
 * @param bit_no pointer to a variable storing the bit number
 * 
 * @return 0 on success, 1 on failure
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief unsubscribes keyboard interrupts
 * 
 * @return 0 on success, 1 on failure
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief handles keyboard interrupts
 */
void (kbc_ih)();

/**
 * @brief reads data from the controller
 * 
 * @param port the port to read from
 * @param output the data in the port
 * @param mouse to tell if the data is from the mouse (1) or keyboard (0)
 * 
 * @return 0 on success, 1 on failure
 */
int read_controller(uint8_t port, uint8_t *output, uint8_t mouse);


#endif
