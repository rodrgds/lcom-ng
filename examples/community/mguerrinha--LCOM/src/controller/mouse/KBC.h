#ifndef _LCOM_KBC_MOUSE_H_
#define _LCOM_KBC_MOUSE_H_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>

/**
 * @brief Function that reads the status of the KBC.
 * @param status Variable to store the status of the KBC.
 * @return 0 if the status was successfully read, 1 otherwise.
*/
int (read_KBC_status)(uint8_t *status);

/**
 * @brief Function that reads the output of the KBC.
 * @param port Port to read the output from.
 * @param output Variable to store the output of the KBC.
 * @param mouse Flag to check if the output is from the mouse.
 * @return 0 if the output was successfully read, 1 otherwise.
*/
int (read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Function that writes a command to the KBC.
 * @param port Port to write the command to.
 * @param commandByte Command to write to the KBC.
 * @return 0 if the command was successfully written, 1 otherwise.
*/
int (write_KBC_command)(uint8_t port, uint8_t commandByte);

#endif
