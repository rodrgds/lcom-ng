#ifndef __KBC_H_
#define __KBC_H_

#include <lcom/lcf.h>

/**
 * @defgroup kbc KBC
 * @{
 *
 * Functions for interacting with the keyboard controller
 */

/**
 * @brief Writes a command to the keyboard controller
 *
 * @param port Port to write the command to
 * @param command Command to write
 * @return Return 0 upon success, non-zero otherwise
 */
int (kbc_write_command)(uint8_t port, uint8_t command);

/**
 * @brief Reads the output from the keyboard controller
 *
 * @param port Port to read from
 * @param cmd Pointer to store the read command
 * @param mouse Flag indicating whether the output is from a mouse
 * @return Return 0 upon success, non-zero otherwise
 */
int (kbc_read_output)(uint8_t port, uint8_t *cmd, uint8_t mouse);

#endif /* __KBC_H_ */
