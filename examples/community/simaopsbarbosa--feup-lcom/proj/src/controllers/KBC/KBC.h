/**
 * @file KBC.h
 * @brief Interface for low-level KBC operations.
 *
 * Provides functions to read status/output and write commands to the KBC, used by keyboard and mouse drivers.
 */

#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include "i8042.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>

/**
 * @brief Reads the status register of the KBC.
 *
 * @param status Pointer to variable where the status byte will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int KBC_read_status(uint8_t *status);

/**
 * @brief Reads the output buffer from the KBC or mouse.
 *
 * @param port The port to read from.
 * @param output Pointer to variable where the read byte will be stored.
 * @param mouse 0 if reading from keyboard, non-zero if reading from mouse.
 * @return 0 on success, non-zero otherwise.
 */
int KBC_read_output(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Writes a command byte to the specified KBC port.
 *
 * @param port The port to write to.
 * @param commandByte The command byte to write.
 * @return 0 on success, non-zero otherwise.
 */
int KBC_write_command(uint8_t port, uint8_t commandByte);

#endif /* _LCOM_KBC_H_ */
