#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/syslib.h>
#include "i8042.h"
#include <lcom/lcf.h>


/**
 * Retrieves the status byte from the Keyboard Controller (KBC).
 *
 * @param status Pointer to a variable where the status byte will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int (KBC_get_status)(uint8_t *status);
 
/**
 * Reads the output from the specified KBC port.
 *
 * @param port The port to read from.
 * @param out Pointer to a variable where the read value will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int (KBC_read_output)(uint8_t port, uint8_t *out);
 
/**
 * Writes a command byte to the specified KBC port.
 *
 * @param port The port to write to (e.g., KBC command register).
 * @param cmd The command byte to write.
 * @return 0 on success, non-zero otherwise.
 */
int (KBC_write_cmd_byte)(uint8_t port, uint8_t cmd);
 
/**
 * Resets the Keyboard Controller (KBC).
 *
 * @return 0 on success, non-zero otherwise.
 */
int (KBC_reset)();

#endif
