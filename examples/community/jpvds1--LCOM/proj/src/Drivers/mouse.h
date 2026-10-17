/**
@file mouse.h 
@brief This file contains the declarations of functions related to the mouse controller and its interactions with the PC.
*/
#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include "i8042.h"

/**
@brief Writes a given command to the mouse controller.
@param command The command to be sent to the mouse controller.
@return Return 0 upon success and non-zero otherwise. */ 
int (write_to_mouse)(uint8_t command);

/**
@brief Writes a given command to the keyboard controller.
@param port The controller's port where the command has to be written. 
@param command The command to be sent to the keyboard controller.
@return Return 0 upon success and non-zero otherwise. */ 
int (mouse_write_to_KBC)(uint8_t port, uint8_t command);

/**

@brief Enables data reporting from the mouse and sets the appropriate packet format.
@return Return 0 upon success and non-zero otherwise. */
int (proj_enable_data_reporting)();

/**
@brief Interrupt handler for mouse events.
*/
void (mouse_ih)();

/**
@brief Sends a command to the mouse controller to request data from it.
@return Return 0 upon success and non-zero otherwise. */ 
int (mouse_write_to_packet)();

/**
@brief Updates the mouse coordinates and sets the corresponding events based on the mouse packet received.
*/ 
void (mouse_update_coords)();

/**
@brief Subscribes and enables interrupts from the mouse controller.
@param irq_set The bit number associated with the mouse controller's interrupt.
@return Return 0 upon success and non-zero otherwise. */
int (mouse_subscribe_int)(uint8_t *irq_set);

/**
@brief Unsubscribes interrupts from the mouse controller.
@return Return 0 upon success and non-zero otherwise. */
int (mouse_unsubscribe_int)();

/**
@brief Reads output information from the mouse controller.
@param output The 32-bit variable where the output data is going to be stored.
@return Return 0 upon success and non-zero otherwise. */
int (mouse_read_output)(uint32_t *output);

#endif //MOUSE_H
