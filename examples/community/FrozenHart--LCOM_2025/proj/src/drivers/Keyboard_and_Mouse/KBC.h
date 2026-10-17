#ifndef KBC_H
#define KBC_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "src/drivers/i8042.h"

extern int keyboard_hook_id;
extern uint8_t keyboard_data;

/**
 * @brief Checks the KBC configuration by reading the status register.
 * 
 * @param st Pointer to store the status register value.
 * @return 0 on success, 1 otherwise.
 */
int (kbc_check_config)(uint8_t *st);

/**
 * @brief Reads data from the KBC output buffer.
 * 
 * @param port The port to read from (e.g., 0x60 or 0x64).
 * @param data Pointer to store the data read from the output buffer.
 * @return 0 on success, 1 otherwise.
 */
int (kbc_read_data)(uint8_t port, uint8_t *data);

/**
 * @brief Writes a command to the KBC.
 * 
 * @param command The command to write.
 * @param command_byte Pointer to the command byte (used for custom commands).
 * @return 0 on success, 1 otherwise.
 */
int(kbc_write_command)(kbc_commands_t command, uint8_t *command_byte);

/**
 * @brief Modifies the KBC command byte to enable or disable specific features.
 * 
 * @param components Array of command byte components to modify.
 * @param size The size of the components array.
 * @return 0 on success, 1 otherwise.
 */
int(kbc_command_byte_IO)(kbc_command_byte_components_t components[], size_t size);

#endif // KBC_H
