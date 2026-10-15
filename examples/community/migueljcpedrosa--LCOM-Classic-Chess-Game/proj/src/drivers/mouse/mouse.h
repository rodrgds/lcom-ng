/**
 * @file mouse
 * @brief This file has the implementation of the functions related to the mouse.
 */

#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "i8042.h"
#include <lcom/lcf.h>

/**
 * @brief Subscribes to mouse interrupts with policies IRQ_EXCLUSIVE and IRQ_REENABLE.
 * 
 * @param bit_no Pointer to store the IRQ line bitmask of the mouse.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (mouse_subscribe_int)(uint8_t* bit_no);

/**
 * @brief Unsubscribes from mouse interrupts.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler function.
 * 
 * Checks the status of the mouse buffer, reads a byte from the buffer if available, 
 * decodes it, and adds it to a packet of mouse data. The packet is ready when three bytes have been read.
 * 
 * @return void.
 */
void (mouse_ih)();

/**
 * @brief Writes a command with optional arguments to the mouse, and optionally reads a response.
 * 
 * @param cmd The command to be written.
 * @param response Pointer to store the read response. If NULL, no response is read.
 * @param args Array of command arguments to be written. If NULL, no arguments are written.
 * @param arg_n The number of command arguments.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (write_command)(uint8_t cmd, uint8_t* response, uint8_t args[], uint32_t arg_n);

/**
 * @brief Disables the data reporting from the mouse.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (disable_data_reporting)();

/**
 * @brief Enables the data reporting from the mouse.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (enable_data_reporting)();

/**
 * @brief Waits until a response is ready to be read from the output buffer.
 * 
 * @param response Pointer to store the read response.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (get_response)(uint8_t* response);

/**
 * @brief Waits until the input buffer is free to write to.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (wait_for_input_buffer)();

/**
 * @brief Writes a command byte to the mouse.
 * 
 * @param cmd The command to be written.
 * 
 * @return Returns 0 upon success, 1 if there was an error, and 2 if the mouse did not acknowledge the command.
 */
int (write_to_mouse)(uint8_t cmd);

#endif
