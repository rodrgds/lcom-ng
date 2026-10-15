/**
 * @file kbc
 * @brief Functions for using the keyboard
 */

#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

/** @defgroup Keyboard Keyboard
 * @{
 *
 * Functions for using the keyboard
 */

#include <stdint.h>
#include "i8042.h"

/**
 * @brief Subscribes keyboard interrupt
 *
 * @param bit_no Bit number of keyboard IRQ
 * @return Return 0 upon success and 1 otherwise
 */
int (kbc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupt
 * @return Return 0 upon success and 1 otherwise
 */
int (kbc_unsubscribe_int)();

/**
 * @brief Reads the keyboard configuration
 *
 * @param value The value of the keyboard configuration
 * @return Return 0 upon success and 1 otherwise
 */
int (read_kbc_config)(uint8_t *value);

/**
 * @brief Reads the keyboard output buffer 
 *
 * @param sc The data in the output buffer
 * @return Return 0 upon success and 1 otherwise
 */
int (read_kbc_out_buf)(uint8_t *sc);

/**
 * @brief Keyboard interrupt handler
 */
void (kbc_ih)();

/**
 * @brief Performs action in the keyboard with the command byte
 *
 * @param cmdb Keyboard command
 * @param cmd  Second argument for the command byte
 * @return Return 0 upon success and 1 otherwise
 */
int (kbc_cmd)(uint8_t cmdb, uint8_t *cmd);

/**
 * @brief Writes the command byte in the port specified
 *
 * @param port Port to write the command byte
 * @param cmd  The command byte to write in the port 
 * @return Return 0 upon success and 1 otherwise
 */
int (write_cmd_byte)(int port, uint8_t cmd);

/**@}*/

#endif
