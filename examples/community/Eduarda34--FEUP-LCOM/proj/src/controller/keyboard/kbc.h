#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @brief Reads the KBC status register
 * 
 * @param status Pointer to store the status byte
 * 
 * @return 0 on success, 1 on failure
 */
int (read_kbc_status)(uint8_t* status);

/**
 * @brief Reads a byte from the KBC output buffer
 * 
 * @param port Port to read from (keyboard or mouse)
 * @param out Pointer to store the read byte
 * @param mouse Boolean flag indicating whether to read from mouse buffer
 * 
 * @return 0 on success, 1 on failure
 */
int (read_kbc_output_buffer)(uint8_t port, uint8_t* out, uint8_t mouse);

/**
 * @brief Writes a command to the KBC
 * 
 * @param port Port to write to
 * @param commandByte The command to send
 * 
 * @return 0 on success, 1 on failure
 */
int (write_kbc_command)(uint8_t port, uint8_t commandByte);

#endif
