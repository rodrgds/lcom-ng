/**
 * @file mouse
 * @author Diogo Pintado, Gabriel Machado Jr., José Costa, Linda Rodrigues
 * 
 *
 * @brief Contains declarations for functions and data structures related to handling the mouse.
 * 
 */

#ifndef _MOUSE_KEYBOARD_H_
#define _MOUSE_KEYBOARD_H_

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

/**
 * @brief Subscribes to the mouse interrupts
 * 
 * @param bit_no The pointer to the mouse's bit number to be set
 * @return int Returns 0 upon success, non-zero otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes to the mouse interrupts
 *
 * @return int Returns 0 upon success, non-zero otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Initializes the packet of bytes received from the mouse
 * 
 */
void (sync_mouse_bytes)();

/**
 * @brief The interrupt handler for mouse interrupts
 * 
 */
void (mouse_ih)();

/**
 * @brief Reads the output from the keyboard controller
 * 
 * @param port The port to read from
 * @param output The pointer to the variable where the output should be stored
 * @param mouse A flag indicating whether the device is a mouse
 * @return Returns 0 upon success, non-zero otherwise
 */
int (read_kbc_output)(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Reads the status of the keyboard controller
 * 
 * @param status The pointer to the variable where the status should be stored
 * @return Returns 0 upon success, non-zero otherwise
 */
int (read_kbc_status)(uint8_t *status);

/**
 * @brief Writes a command to the keyboard controller
 * 
 * @param port The port to write to
 * @param command_byte The command to be written
 * @return Returns 0 upon success, non-zero otherwise
 * 
 */
int (write_kbc_command)(uint8_t port, uint8_t command_byte);

/**
 * @brief Writes a command to the mouse
 * 
 * @param command The command to be written
 * @return Returns 0 upon success, non-zero otherwise
 */
int (write_to_mouse)(uint8_t command);

/**
 * @brief Builds a packet from the bytes received from the mouse
 * 
 */
void (build_mouse_packet)();

#endif /*_MOUSE_KEYBOARD_H_*/
