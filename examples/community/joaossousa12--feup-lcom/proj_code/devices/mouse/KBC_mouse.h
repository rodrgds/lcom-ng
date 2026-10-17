/**
 * @file KBC_mouse
 * 
 * @brief Header file for KBC functions related to the mouse.
 * 
 * @author João Pedro Sousa, Bernardo Pinto, Carlos Madaleno
 * 
 * @date 28/05/2023
*/


#ifndef _LCOM_KBC_MOUSE_H_
#define _LCOM_KBC_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @brief Reads the status register of the KBC for the mouse.
 * 
 * @param status A pointer to the variable where the status register value will be stored.
 * 
 * @return 0 on success, non-zero if there was an error reading the status register.
*/

int (read_KBC_status_mouse)(uint8_t* status);

/**
 * @brief Reads the output data from the KBC for the mouse.
 * 
 * @param port where the info is going to be written to.
 * 
 * @param output Pointer to a variable where the output data will be stored.
 * 
 * @param mouse Indicates whether the data is coming from the mouse or keyboard (1 for mouse, 0 for keyboard).
 * 
 * @return 0 on success, non-zero if there was an error reading the output data or if the data is not valid.
*/

int (read_KBC_output_mouse)(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Writes a command byte to the KBC for the mouse.
 * 
 * @param port The port to which the command byte will be written (0x64).
 * 
 * @param commandByte The command byte to be written.
 * 
 * @return 0 on success, non-zero if there was an error writing the command byte.
*/

int (write_KBC_command_mouse)(uint8_t port, uint8_t commandByte);

#endif
