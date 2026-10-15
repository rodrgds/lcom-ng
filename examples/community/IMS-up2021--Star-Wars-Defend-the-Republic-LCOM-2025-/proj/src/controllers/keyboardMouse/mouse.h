#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"  
#include "entity.h" 

struct packet;

/**
 * @file mouse.h
 * @brief Interface for programming the PS/2 Mouse via the i8042 Keyboard Controller (KBC).
 *
 * This file declares functions for subscribing and unsubscribing mouse interrupts,
 * handling mouse interrupts (reading and parsing mouse packets), and sending commands to the mouse.
 */

/** @defgroup mouse Mouse Interface
 * @{
 *
 * Functions for interacting with the PS/2 Mouse.
 */


extern struct packet mouse_packet; 

/**
 * @brief Subscribes to mouse interrupts.
 *
 * Sets up the system to receive interrupts from the PS/2 mouse (via the KBC).
 * The IRQ line used by the mouse (IRQ_MOUSE) will be enabled, and a hook ID
 * will be associated with it. It also attempts to enable mouse data reporting.
 *
 * @param bit_no_out Pointer to a variable where the bit number (hook_id) for mouse interrupts will be stored.
 *                   This bit number should be used to check for mouse interrupts in the main loop.
 * @return int 0 upon success, non-zero otherwise.
 */
int (mouse_subscribe_int)(uint8_t *bit_no_out);

/**
 * @brief Unsubscribes from mouse interrupts.
 *
 * Disables mouse interrupts and removes the associated hook ID.
 * It also attempts to disable mouse data reporting.
 *
 * @return int 0 upon success, non-zero otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler.
 *
 * This function should be called when a mouse interrupt occurs.
 * It reads a byte from the KBC's output buffer (KBC_OUT_CMD).
 * This byte is part of a 3-byte mouse packet. The function manages the synchronization
 * and assembly of these bytes.
 */
void (mouse_ih)();

/**
 * @brief Synchronizes mouse packet bytes.
 *
 * This internal helper function, likely called by `mouse_ih`, processes a byte read
 * from the mouse. It ensures that the bytes are correctly ordered to form a complete
 * 3-byte packet, handling the first byte detection (which has BIT(3) set).
 * Once three bytes are collected, it calls `mouse_bytes_to_packet`.
 */
void (mouse_sync_bytes)(); // Note: Typically, mouse_ih would call this or implement its logic.
                           // If mouse_ih directly reads and stores bytes, and mouse_sync_bytes is separate,
                           // its role is to take the latest raw byte and add it to a temporary buffer,
                           // checking for packet completion.

/**
 * @brief Converts the raw mouse bytes into a structured `mouse_packet`.
 *
 * Once three valid bytes of a mouse packet have been collected (typically by `mouse_sync_bytes`
 * or within `mouse_ih`), this function parses these bytes and populates the fields
 * of the global `mouse_packet` structure (e.g., button states, X/Y deltas, overflow flags).
 */
void (mouse_bytes_to_packet)();

/**
 * @brief Writes a command byte to the mouse.
 *
 * Sends a command to the PS/2 mouse. This involves first sending a "Write Byte to Mouse"
 * command (0xD4) to the KBC (port 0x64), and then sending the actual mouse command
 * (e.g., MOUSE_ENBL_DATA_REP, MOUSE_SET_STREAM) to the KBC's input buffer (port 0x60).
 * It waits for an acknowledgement (MOUSE_ACK) from the mouse.
 *
 * @param command The command byte to send to the mouse (e.g., ENBL_DATA_REP, DIS_DATA_REP).
 * @return int 0 upon success (MOUSE_ACK received), non-zero otherwise (e.g., MOUSE_NACK or timeout).
 */
int (mouse_write)(uint8_t command);


/**@}*/

#endif /* _LCOM_MOUSE_H_ */
