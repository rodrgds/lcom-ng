#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>

#define ATTEMPTS 10

/**
 * @brief Reads the status of the KBC
 * 
 * @param status Variable to store the status
 * @return 0 on success, 1 otherwise
 */
int (kbc_read_status)(uint8_t *status);

/**
 * @brief Reads the output buffer of the KBC
 * 
 * @param port Port to read from
 * @param output Variable to store the output
 * @param is_mouse Whether the output is from the mouse or not
 * @return 0 on success, 1 otherwise
 */
int (kbc_read_output)(int port, uint8_t *output, bool is_mouse);

/**
 * @brief Writes a command to the KBC
 * 
 * @param port Port to write to
 * @param command Command to write
 * @return 0 on success, 1 otherwise
 */
int (kbc_write_command)(int port, uint8_t command);

#endif // _LCOM_KBC_H_
