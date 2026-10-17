/**
 * @file kbc_utils.h
 * @brief Utility functions for interacting with the i8042 keyboard controller (KBC).
 */

#ifndef _KBC_UTILS_H_
#define _KBC_UTILS_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"

/**
 * @brief Reads the status register of the KBC.
 * 
 * @param status Pointer to store the read status byte.
 * @return 0 on success, non-zero otherwise.
 */
int (read_KBC_status)(uint8_t* status);

/**
 * @brief Reads output from the KBC output buffer.
 * 
 * @param output Pointer to store the read output byte.
 * @param port Port to read from.
 * @param mouse Flag indicating if the expecting mouse data (1) or keyboard data (0).
 * @return 0 on success, non-zero otherwise.
 */
int (read_KBC_output)(uint8_t* output, uint8_t port, uint8_t mouse);

/**
 * @brief Writes a command to the specified KBC port.
 * 
 * @param port Port to write the command to.
 * @param cmd Command byte to send.
 * @return 0 on success, non-zero otherwise.
 */
int (write_KBC_command)(uint8_t port, uint8_t cmd);

/**
 * @brief Flushes the KBC input buffer.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (kbc_flush_input)();

#endif /* _KBC_UTILS_H_ */
