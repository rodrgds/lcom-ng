/**
 * @file mouse.h
 * @brief Function declarations and global variables for mouse handling.
 */

#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>
#include "../i8042.h"
#include "kbc.h"
#include <minix/syslib.h>

/**
 * @brief Counter of mouse packet bytes received.
 */
extern uint8_t byte_cnt;

/**
 * @brief Struct holding the parsed mouse packet.
 */
extern struct packet mouse_packet;

/**
 * @brief Subscribes mouse interrupts.
 * 
 * @param bit_no Pointer to variable where the interrupt bit number is stored.
 * @return 0 on success, 1 on failure.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts.
 * 
 * @return 0 on success, 1 on failure.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler.
 * 
 * Reads a byte from the mouse output buffer.
 */
void (mouse_ih)();

/**
 * @brief Synchronizes and stores mouse bytes into a packet buffer.
 * 
 * Ensures the packet starts with a properly aligned first byte (BIT(3) set).
 */
void (mouse_sync_packet)();

/**
 * @brief Parses the 3-byte mouse packet into a structured format.
 */
void (mouse_parse_packet)();

/**
 * @brief Sends a command to the mouse.
 * 
 * @param command The command byte to be sent to the mouse.
 * @return 0 on success (ACK received), 1 on failure.
 */
int (mouse_send_command)(uint8_t command);

/**
 * @brief Enables mouse interrupts.
 * 
 * @return 0 on success, 1 on failure.
 */
int (mouse_enable_int)();

/**
 * @brief Disables mouse interrupts.
 * 
 * @return 0 on success, 1 on failure.
 */
int (mouse_disable_int)();

#endif

