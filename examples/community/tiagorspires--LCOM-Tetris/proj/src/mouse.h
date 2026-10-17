/**
 * @file mouse.h
 * @brief Mouse interface functions and definitions.
 */

#ifndef MOUSE_H
#define MOUSE_H

#include "i8042.h"
#include <lcom/lcf.h>

/**
 * @brief Subscribes to mouse interrupts.
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * @return 0 upon success, 1 otherwise.
 */
int (mouse_subscribe_int)(uint8_t* bit_no);

/**
 * @brief Unsubscribes from mouse interrupts.
 * 
 * @return 0 upon success, 1 otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler.
 * Reads the mouse data from the output buffer.
 */
void (mouse_ih)();

/**
 * @brief Sends a command to the mouse.
 * 
 * @param command The command to send.
 * @return 0 upon success, 1 otherwise.
 */
int (send_command)(uint8_t command);

/**
 * @brief Reads data from the output buffer.
 * 
 * @param data Pointer to store the read data.
 * @return 0 upon success, 1 otherwise.
 */
int (read_buffer)(uint8_t *data);

/**
 * @brief Checks if the input buffer is full.
 * 
 * @return True if the input buffer is full, false otherwise.
 */
bool is_input_buffer_full();

/**
 * @brief Changes the data report mode of the mouse.
 * 
 * @param set The command to set the data report mode.
 * @return 0 upon success, 1 otherwise.
 */
int change_data_report_mode(uint8_t set);

/**
 * @brief Handles mouse events and updates the cursor position.
 * 
 * @param pp The packet structure containing the mouse data.
 */
void mouse_event_handler(struct packet *pp);

#endif // MOUSE_H
