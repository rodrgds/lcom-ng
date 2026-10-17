#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "MACROS.h"

/**
 * @brief Subscribes to the keyboard's interrupts
 * 
 * @param bit_no
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the keyboard's interrupts 
 */
int (kbd_unsubscribe_int)();

/**
 * @brief Issues a given command to the KBC, can be used to get the result of that command or issue
 *        an argument to that command
 * 
 * @param command       Command that will be issued
 * @param arg_or_res    Argument or result
 * @param expect_res    True if a result is expected, false if an argument to command
 *                      is being passed
 * 
 * @return int Always returns 0
 */
int (kbc_cmd)(uint8_t command, uint8_t *arg_or_res, bool expect_res);

/**
 * @brief Keyboard interrupt handler. This function handles the interrupts that are originated by 
 *        the keyboard.
 */
void (kbc_ih)(void);

/**
 * @brief This funtion takes in a scancode as an argument and interprets it in accordance
 *        to a keymap defined by us, turning a scancode into a char
 * 
 * @param code    Scancode
 * @return char   Char that corresponds to the scancode in the keymap
 */
char key_handler(uint8_t code);


#endif
