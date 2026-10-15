#ifndef _KBC_H_
#define _KBC_H_

/**
 * @file KBC.h
 * @brief Keyboard Controller (KBC) interface functions.
 */

#include <lcom/lcf.h>
#include "i8042.h"

/** @defgroup KBC KBC
 * @{
 */

/**
 * @brief Retrieves the status register of the KBC.
 *
 * @param status Pointer to a variable where the status byte will be stored.
 * @return 0 on success, 1 on failure.
 */

int (get_KBC_status)(uint8_t *status);

/**
 * @brief Checks the status byte for errors.
 *
 * @param status The status byte to be checked.
 * @return Non-zero value if an error is detected, 0 otherwise.
 */
int (check_KBC_status)(uint8_t status);

/**
 * @brief Writes a command to a given port.
 *
 * @param port The port to which the command is to be written.
 * @param command The command to be written.
 * @return 0 on success, 1 on failure.
 */
int (write_command)(int port, uint8_t command);

/**
 * @brief Reads the output data from a given port.
 *
 * @param port The port from which the output data is to be read.
 * @param output Pointer to a variable where the output data will be stored.
 * @param keyboard Indicates whether the output is from the keyboard (true) or mouse (false).
 * @return 0 on success, 1 on failure.
 */
int (read_output)(int port, uint8_t *output, bool keyboard);

/** @} */ // end of KBC group
#endif
