/**
 * @file mouse
 * 
 * @brief This file contains functions and constants related to mouse handling and configuration.
 * 
 * @author João Pedro Sousa, Bernardo Pinto, Carlos Madaleno
 * 
 * @date 28/05/2023
*/

#ifndef SRC_MOUSE_H
#define SRC_MOUSE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <minix/sysutil.h>
#include "KBC_mouse.h"
#include "i8042.h"

/**
 * @brief Uses irq_setpolicy for the mouse interrupts.
 * 
 * @return 0 on success or 1 otherwise
*/

int (mouse_subscribe_int)();

/**
 * @brief Unsubscribes mouse interrupts.
 * 
 * @return 0 on success, 1 on error
*/

int (mouse_unsubscribe_int)();

/**
 * @brief Mouse input handler.
*/

void (mouse_ih)();

/**
 * @brief Synchronizes and processes mouse data received from the mouse controller.
*/

void (mouse_sync)();

/**
 * @brief Converts mouse data bytes to a mouse packet struct.
 * 
 * @return 0 on success or 1 otherwise
*/

int (to_packet) ();

/**
 * @brief Writes a command to the mouse controller.
 * 
 * @param command The command to be sent to the mouse controller.
 * 
 * @return 0 on success, non-zero if there was an error sending the command or receiving the response.
*/

int (mouse_write)(uint8_t command);

/**
 * @brief Refreshes the mouse location based on the delta values in the mouse packet.
*/

void (refresh_mouse_location)();

#endif //SRC_MOUSE_H
