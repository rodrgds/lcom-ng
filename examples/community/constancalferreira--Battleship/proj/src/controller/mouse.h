/**
 * @file mouse.h
 * @brief PS/2 Mouse driver interface
 * 
 * This file contains declarations for functions that handle PS/2 mouse 
 * initialization, interrupt handling, command writing, and packet processing.
 * It provides a complete interface for mouse event handling in the Battleship game.
 */

#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdbool.h>

#include "i8042.h"
#include "KBC.h"



/**
 * @brief Subscribes mouse interrupts
 * 
 * Sets up the interrupt service routine for the mouse and enables
 * mouse interrupts.
 * 
 * @param[out] irq_set Pointer to variable that will be filled with the bit mask for mouse interrupts
 * @return 0 upon success, 1 otherwise
 */
int (mouse_subscribe_int) (uint32_t* irq_set);

/**
 * @brief Unsubscribes mouse interrupts
 * 
 * Disables mouse interrupts and removes the interrupt handler.
 * 
 * @return 0 upon success, 1 otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler
 * 
 * Called when a mouse interrupt occurs. Reads a byte from the mouse
 * and stores it in the global variable current_byte.
 */
void (mouse_ih)();

/**
 * @brief Writes a command to the mouse
 * 
 * Sends a command to the mouse through the KBC, handling the protocol
 * for mouse command forwarding and acknowledgment.
 * 
 * @param command Command byte to send to the mouse
 * @return 0 upon success, 1 otherwise
 */
int (mouse_write_cmd)(uint8_t command);

/**
 * @brief Sets the mouse to stream mode
 * 
 * Configures the mouse to operate in stream mode, where it automatically
 * sends packets when movement or button events occur.
 * 
 * @return 0 upon success, 1 otherwise
 */
int (mouse_set_stream_mode)();

/**
 * @brief Enables data reporting on the mouse
 * 
 * Makes the mouse start sending data packets when events occur.
 * 
 * @return 0 upon success, 1 otherwise
 */
int (_mouse_enable_data_reporting)();

/**
 * @brief Disables data reporting on the mouse
 * 
 * Makes the mouse stop sending data packets, even when events occur.
 * 
 * @return 0 upon success, 1 otherwise
 */
int (_mouse_disable_data_reporting)();

/**
 * @brief Processes mouse bytes to build complete packets
 * 
 * Uses the current_byte global variable to build a complete mouse packet.
 * Checks for packet synchronization and tracks position within the
 * packet sequence.
 * 
 * @return true when a complete packet has been assembled, false otherwise
 */
bool (mouse_sync_bytes)();

/**
 * @brief Creates a mouse packet from the collected bytes
 * 
 * Populates the mouse_packet global variable with processed data from
 * the collected bytes, including button states and movement deltas.
 */
void (create_packet)();


#endif 
