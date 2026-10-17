#ifndef KBD_H
#define KBD_H

#include <stdio.h>
#include <lcom/lcf.h>
#include <stdint.h>
#include <lcom/utils.h>
#include "kbd_defs.h"

/**
 * @file keyboard.h
 * @brief Functions to interface with the PC's keyboard controller.
 * 
 * File containing the functions to subscribe and handle the keyboard's interruptions.
*/

/**
 * @brief Subscribes keyboard's interrupts.
 *  
 * Subscribes keyboard's interrupt notifications using sys_irqsetpolicy().
 * 
 * @param bit_no pointer to the hook_id returned by the kernel in sys_irqsetpolicy().
 * @return 0 if the operation was successful, 1 otherwise
 */
int (subscribe_kbc)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard's interrupts.
 * 
 * Unsubscribes keyboard's interrupt notifications using sys_irqrmpolicy(). 
 * 
 * @return 0 if the operation was successful, 1 otherwise.
 */
int (unsubscribe_kbc)();

/**
 * @brief Reads command byte.
 * 
 * Writes KBC command 0x20 to port 0x64, reading the byte in the output buffer to uint8_t* out.
 * 
 * @param out pointer to where to store the command byte read.
 * @return 0
 */
int kbc_cmd_read(uint8_t *out);

/**
 * @brief Writes command.
 * 
 * Writes KBC command 0x60 to port 0x64, and then writes the new value of the command byte to port 0x60.
 * 
 * @param command command byte to be written.
 * @return 0
 */
int kbc_cmd_write(uint8_t command);

/**
 * @brief Reads the KBC state
 * 
 * Reads the byte in the status register (port 0x64) and checks if it's valid. 
 * Discards the value read if the parity, timeout or aux bits are set and checks 
 * the obf bit to see if there is data available for reading from the buffer.
 * 
 * @param preserve bool that gets if what's read is in a valid state or not
 * @return 0 if the operation was successful, 1 otherwise
 */
int kbd_get_status_byte(bool *preserve);

#endif
