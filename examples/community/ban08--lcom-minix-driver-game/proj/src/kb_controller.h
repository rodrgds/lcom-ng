#ifndef _LCOM_KB_CONTROLLER_H_
#define _LCOM_KB_CONTROLLER_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "i8042.h"

/**
 * @brief Reads the status register of the keyboard controller.
 *
 * This function uses util_sys_inb to read the status register of the 8042 keyboard controller.
 *
 * @param status Pointer to a variable where the status byte will be stored.
 * @return 0 on success, non-zero on failure.
 */
int (readKBControllerstat)(uint8_t* status);

/**
 * @brief Reads the output from the keyboard or mouse controller.
 *
 * This function checks the output buffer of the keyboard controller and reads the data if available.
 * It also performs error checking for parity, timeout, and data source (keyboard vs mouse).
 *
 * @param port The I/O port to read from.
 * @param output Pointer to a variable where the output data will be stored.
 * @param mouse Boolean indicating if the data is from the mouse (1) or keyboard (0).
 * @return 0 on success, non-zero on error (e.g., timeout, parity error, or invalid source).
 */
int (readKBControlleroutput)(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Writes a command byte to the keyboard controller.
 *
 * This function waits for the input buffer to be clear before sending a command byte
 * to the specified port.
 *
 * @param port The I/O port to write to.
 * @param cmd_byte The command byte to send.
 * @return 0 on success, non-zero on failure.
 */
int (writeKBControllercommand)(uint8_t port, uint8_t cmd_byte);

#endif // _LCOM_KB_CONTROLLER_H_
