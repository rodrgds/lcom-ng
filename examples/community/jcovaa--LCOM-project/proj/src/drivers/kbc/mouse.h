/**
 * @file mouse.h
 * @brief Functions for handling mouse interrupts and communication.
 */

#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <minix/sysutil.h>
#include "i8042.h"
#include "kbc_utils.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Subscribes mouse interrupts.
 * 
 * @param bit_no Pointer to a variable where the bit number will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_subscribe_interrupts)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_unsubscribe_interrupts)();

/**
 * @brief Initializes the mouse by setting it to stream mode and enabling data reporting.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_init)();

/**
 * @brief Resets the mouse by disabling data reporting.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_reset)();

/**
 * @brief Mouse interrupt handler.
 */
void (mouse_ih)();

/**
 * @brief Sends a command to the mouse.
 * 
 * @param command The command byte to send.
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_send_command)(uint8_t command);

/**
 * @brief Synchronizes the mouse packet by reading bytes from the KBC output buffer.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_packet_sync)();

/**
 * @brief Parses the mouse packet and updates the mouse state.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_parse_packet)();

/**
 * @brief Updates the mouse position based on the parsed packet.
 */
void (mouse_update_position)();

#endif /* _MOUSE_H_ */
