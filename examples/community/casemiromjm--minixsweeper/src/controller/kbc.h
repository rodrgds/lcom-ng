/**
 * @file kbc.h
 * @brief Interface for interacting with the Keyboard Controller (KBC).
 */

#ifndef _KBC_H_
#define _KBC_H_

#include <lcom/lcf.h>
#include "../i8042.h"
#include <minix/sysutil.h>

/**
 * @brief Reads the KBC status register.
 * 
 * @param status Pointer to a variable to store the status byte.
 * @return 0 on success, 1 on failure.
 */
int (read_KBC_state)(uint8_t* status);

/**
 * @brief Reads a byte from the KBC output buffer.
 * 
 * This function checks if the data is from the expected device (mouse or keyboard)
 * and verifies for parity and timeout errors.
 * 
 * @param port The port to read from (0x60).
 * @param output Pointer to a variable to store the read byte.
 * @param mouse 1 if reading mouse data, 0 for keyboard data.
 * @return 0 on success, 1 on failure.
 */
int (read_KBC_output)(uint8_t port, uint8_t* output, uint8_t mouse);

/**
 * @brief Writes a command byte to the specified KBC port.
 * 
 * Waits until the input buffer is empty before writing.
 * 
 * @param port The port to write to (typically 0x64).
 * @param commandByte The command byte to write.
 * @return 0 on success, 1 on failure.
 */
int (write_KBC_command)(uint8_t port, uint8_t commandByte);

#endif

