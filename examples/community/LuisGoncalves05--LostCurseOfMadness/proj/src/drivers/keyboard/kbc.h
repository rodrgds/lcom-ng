#ifndef KBC_H
#define KBC_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

extern int32_t kbd_subscription_id; /**< Keyboard subscription ID */

/**
 * @brief Writes byte in KBC port.
 *
 * @param port KBC port to write command into.
 * @param byte Byte to write in port.
 * @return Return 0 upon success and non-zero otherwise.
 */
int(kbc_write_byte)(uint8_t port, uint8_t byte);

/**
 * @brief Writes command to KBC_COMMAND_REGISTER.
 *
 * @param command Command to be written in port.
 * @return Return 0 upon success and non-zero otherwise.
 */
int(kbc_write_command)(uint8_t command);

/**
 * @brief Writes a command's argument to KBC_ARGUMENT_REGISTER.
 *
 * @param argument Argument to be written.
 * @return Return 0 upon success and non-zero otherwise.
 */
int(kbc_write_command_arguments)(uint8_t argument);

/**
 * @brief Reads byte from KBC port.
 *
 * @param port KBC port to read command from.
 * @param byte Will hold the value read from port.
 * @param mouse Reading for mouse if true else for keyboard.
 * @return Return 0 upon success and non-zero otherwise.
 */
int(kbc_read_byte)(uint8_t port, uint8_t *byte, bool mouse);

/**
 * @brief Reads a command's return value.
 * Reads the return value of a command from KBC_RETURN_REGISTER to retv.
 *
 * @param retv Will hold the value returned by command.
 * @param mouse Reading for mouse if true else for keyboard.
 * @return Return 0 upon success and non-zero otherwise.
 */
int(kbc_read_command_return)(uint8_t *retv, bool mouse);

#endif /* __KBC_H */
