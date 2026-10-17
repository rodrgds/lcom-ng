/**
 * @file keyboard.h
 * @brief file that has functions to allow configurating and using the keyboard, as well as some functions that can be used by either keyboard or mouse
 */

#ifndef _KEYBOARD_
#define _KEYBOARD_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"


/**
 * @brief this function subscribes the keyboard interrupts
 * @return 0 if sucess, otherwise 1
 */
int (keyboard_subscribe_interrupts)();


/**
 * @brief this function unsubscribes the keyboard interrupts
 * @return 0 if sucess, otherwise 1
 */
int (keyboard_unsubscribe_interrupts)();


/**
 * @brief this function reads the current status of the mouse or keyboard
 * @param mouse simple variable just to know when we want to read from keyboard or from mouse
 * @param port the port where the output is gone be read from
 * @param output the current data that is avaiable in the port
 * @return 0 if sucess, otherwise 1
 */
int (keyboard_mouse_read_output)(uint8_t mouse, uint8_t port, uint8_t *output);


/**
 * @brief this function tries to write the given command to the given port
 * @param port where the command is going to be written
 * @param command the configuration command that is going to be written
 * @return 0 if sucess, otherwise 1
 */
int (keyboard_mouse_write_command)(uint8_t port, uint8_t command);


/**
 * @brief this function reads the current status of the mouse or keyboard
 * @param status variable where the current keyboard or mouse status is
 * @return 0 if sucess, otherwise 1
 */
int (keyboard_mouse_read_status)(uint8_t* status);

#endif /* _KEYBOARD_ */
