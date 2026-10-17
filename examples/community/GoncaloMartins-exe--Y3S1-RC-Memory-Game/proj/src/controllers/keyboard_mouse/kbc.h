/**
*@file kbc.h
*@brief Keyboard Controller (KBC) interface for reading status, output, and sending commands.
*
*This header defines functions for interacting with the i8042 Keyboard Controller,
*including reading status, output data, and sending commands to specified ports. 
*/
#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>

/**
 * @brief Reads the status from the KBC status register.
*Retrieves the current status of the Keyboard Controller and stores it in the provided pointer.
*@param status Pointer to store the status byte.
*@return 0 on success, 1 on failure. / int (kbc_read_status)(uint8_t status);
 */
int (kbc_read_status)(uint8_t* status);

/**
 * @brief Reads data from the specified KBC port.
*Reads data from the given port (keyboard or mouse) and stores it in the output pointer.
*Validates the data source (mouse or keyboard) and checks for errors.
 */
int (kbc_read_output)(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Sends a command to the specified KBC port.
 *
*Writes a command byte to the specified port, ensuring the input buffer is ready.
*@param port The port to send the command to (e.g., KBC_CMD_REG for command register).
*@param commandByte The command byte to send.
*@return 0 on success, 1 on failure (timeout or write error). 
 */
int (kbc_send_cmd)(uint8_t port, uint8_t commandByte);

#endif /* _LCOM_KBC_H_ */
