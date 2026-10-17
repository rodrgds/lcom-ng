#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <lcom/lcf.h>
#include <lcom/utils.h>

/**
 * @defgroup keyboard keyboard
 * @{
 * 
 * Module for the keyboard
 */

/**
 * @brief Function responsible for subscribing the interrupts from the keyboard
 * 
 * @param bit_no Identifies what bit will be set when an keyboard interruption occurs
 * @return Return 0 upon success and non-zero otherwise 
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Function responsible for unsubscribing the interrupts from the keyboard
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Function responsible for reading the output buffer of the KBC
 * 
 * @return uint8_t Byte reaf from the output buffer
 */
uint8_t (read_from_kbc)();

/**
 * @brief Function that writes a command to the command register of the KBC
 * 
 * @param cmd Command written to the command register
 * @return uint8_t Return 0 upon success and non-zero otherwise 
 */
uint8_t (issue_command)(uint8_t cmd);

/**
 * @brief Function that writes the arguments of a command 
 * to the arguments register of the KBC
 * 
 * @param arg Argument written to the argument register
 * @return uint8_t Return 0 upon success and non-zero otherwise 
 */
uint8_t (issue_arg)(uint8_t arg);

/**
 * @}
 */

#endif /* __KEYBOARD_H */
