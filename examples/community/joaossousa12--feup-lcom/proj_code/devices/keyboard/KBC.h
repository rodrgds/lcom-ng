/**
 * @file KBC
 * 
 * @brief KBC functions for reading status, output data, and writing command bytes.
 * 
 * @author João Pedro Sousa, Bernardo Pinto, Carlos Madaleno
 * 
 * @date 28/05/2023
*/


#ifndef _LCOM_KBC_MOUSE_H_
#define _LCOM_KBC_MOUSE_H_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>

/**
 * @brief Reads the status register of the KBC.
 * 
 * @param status A pointer to the variable where the status register value will be stored.
 * 
 * @return 0 on success, non-zero if there was an error reading the status register.
*/

int (read_KBC_status)(uint8_t* status);

/**
 * @brief Reads the output data from the KBC.
 * 
 * @param port The port from which to read the output data.
 * 
 * @param output Pointer to a variable where the output data will be stored.
 * 
 * @param mouse Indicates whether the data is coming from the mouse or keyboard (1 for mouse, 0 for keyboard).
 * 
 * @return 0 on success, non-zero if there was an error reading the output data or if the data is not valid.
*/

int (read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Writes a command byte to the KBC.
 * 
 * @param port The port to which the command byte will be written.
 * 
 * @param commandByte The command byte to be written.
 * 
 * @return 0 on success, non-zero if there was an error writing the command byte.
*/

int (write_KBC_command)(uint8_t port, uint8_t commandByte);

#endif
