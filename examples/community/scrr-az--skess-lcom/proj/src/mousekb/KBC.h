#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include "i8042.h"
#include <lcom/lcf.h>

/**
 * @brief Reads the status register of KBC and checks if it is valid.
 * 
 * @param status Pointer to a variable where the status byte will be stored.
 * @return 0 on success.
 */
int (read_KBC_status)(uint8_t* status);

/**
 * @brief Reads the output buffer of the KBC when it is full.
 *
 * @param output Pointer to store data read from output buffer.
 * @param mouse for that is set to true if expecting mouse data or false if expecting keyboard data.
 * @return 0 on success or 1 if process fails.
 */
int (read_KBC_output)(uint8_t *output, bool mouse);

/**
 * @brief Writes a command byte to a specific KBC port.
 * 
 * @param port the I/O port to write the command to.
 * @param commandByte the command byte to be written.
 * @return 0 on success or 1 if process fails after 10 tries.
 */
int (write_KBC_command)(uint8_t port, uint8_t commandByte);

#endif /*_LCOM_KBC_H_*/
