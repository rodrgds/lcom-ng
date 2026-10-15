/**
 * @file keyboard.h
 * @brief Contains functions for handling keyboard interrupts and commands.
 */


#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"



/**
 * @brief Subscribes to keyboard interrupts.
 *
 * @param bit_no Pointer to the variable that will store the bit number of the interrupt.
 * @return 0 on success, 1 on failure.
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);


/**
 * @brief Unsubscribes from keyboard interrupts.
 *
 * @return 0 on success, 1 on failure.
 */
int (keyboard_unsubscribe_int)();


/**
 * @brief Gets the status from the keyboard controller.
 *
 * @param status Pointer to the variable that will store the status.
 * @return 0 on success, 1 on failure.
 */
int (keyboard_get_status)(uint8_t* status);


/**
 * @brief Reads the output from the keyboard controller.
 *
 * @param output Pointer to the variable that will store the output.
 * @return 0 on success, 1 on failure.
 */
int (keyboard_read_output)(uint8_t* output);


/**
 * @brief Interrupt handler for keyboard controller.
 */
void (kbc_ih)();


/**
 * @brief Writes a command to the specified port of the keyboard controller.
 *
 * @param port The port to write the command to.
 * @param cmd The command to write.
 * @return 0 on success, 1 on failure.
 */
int (keyboard_restore_int)();


/**
 * @brief Restores the default interrupt settings of the keyboard controller.
 *
 * @return 0 on success, 1 on failure.
 */
int (keyboard_write_cmd)(uint8_t port ,uint8_t cmd);

#endif
