#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
#include "kbc.h"

/**
 * @file mouse.h
 * @brief Mouse management
 * 
 * Functions to interact with the mouse.
 */

/**
 * @brief Returns a pointer to the packet struct
 */
struct packet* get_packet();

/**
 * @brief Subscribes mouse interrupts
 * 
 * @param bit_no Pointer to store the bit number set in the interrupt mask
 * @return 0 on success, 1 on failure
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 * 
 * @return 0 on success, 1 on failure
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler
 */
void (mouse_ih)();

/**
 * @brief Parses the packet acording to the packet struct
 */
void packet_parse();

/**
 * @brief Checks if the packet is complete
 * 
 * @return 1 if the packet is complete, 0 otherwise
 */
int check_packet_complete();

/**
 * @brief Writes a command to the mouse, used for enabling and disabling data reporting
 */
int mouse_write_command(uint8_t command);

/**@}*/

#endif /* KEYBOARD_H */
