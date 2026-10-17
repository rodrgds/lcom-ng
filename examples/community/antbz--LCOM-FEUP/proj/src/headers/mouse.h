#include <lcom/lcf.h>
#include <lcom/utils.h>
#include <lcom/lab4.h>
#include <stdint.h>
#include <stdio.h>
#include "keyboard.h"
#include "../headers/mouse_defs.h"

/**
 * @file mouse.h
 * @brief Functions to interface with the mouse.
 * 
 * File containing the functions to subscribe and handle the mouse's interruptions.
*/

/**
 * @brief Subscribes mouse's interrupts.
 *  
 * Subscribes mouse's interrupt notifications using sys_irqsetpolicy().
 * 
 * @param bit_no pointer to the hook_id returned by the kernel in sys_irqsetpolicy().
 * @return 0 if the operation was successful, 1 otherwise
 */
int (subscribe_mouse)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse's interrupts.
 * 
 * Unsubscribes mouse's interrupt notifications using sys_irqrmpolicy(). 
 * 
 * @return 0 if the operation was successful, 1 otherwise.
 */
int (unsubscribe_mouse)();

/**
 * @brief Enables mouse's interrupts.
 * 
 * Enables interrupts on the mouse IRQ line (associated with the specified hook_id) by calling function sys_irqenable().
 * 
 * @return value returned by sys_irqenable().
 */
int enable_mouse();

/**
 * @brief Disables mouse's interrupts.
 * 
 * Disables interrupts on the mouse IRQ line (associated with the specified hook_id) by calling function sys_irqdisable().
 * 
 * @return value returned by sys_irqdisable().
 */
int disable_mouse();

/**
 * @brief Reads the mouse state
 * 
 * Reads the byte in the status register (port 0x64) and writes it to uint8_t* status
 * 
 * @param status pointer to where to store the status byte read
 * @return 0 if the operation was successful, 1 otherwise
 */
int mouse_get_status_byte(uint8_t *status);

/**
 * @brief Mouse interrupt handler
 * 
 * Reads the mouse status byte and checks if there is data available for reading.
 * If so, reads the byte in the output buffer and stores it in the correct byte of
 * the 3-byte data packet
 */
void (mouse_ih)();

/**
 * @brief Assembles the 3-byte data packet
 * 
 * Fills each field in the struct packet with the correct byte/bit from the bytes received
 * 
 * @param pp pointer to the struct packet to store the received information
 */
void assemble_packet(struct packet *pp);

/**
 * @brief Writes command.
 * 
 * Reads the status byte to check if the input buffer is full, if not writes command 0xD4 to
 * the kbc command register. Checks input buffer again and then writes the command byte
 * to port 0x60.
 * Re-sends command if failed in one byte, halts execution if there was an error in two consecutive bytes
 * 
 * @param command command byte to be written.
 * @return 0 if the operation was successful, 1 otherwise (-1 if there was an error)
 */
int mouse_cmd_write(uint8_t command);
