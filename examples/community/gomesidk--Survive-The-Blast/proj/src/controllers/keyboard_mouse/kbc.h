#ifndef _KBC_H_
#define _KBC_H_

#include <lcom/lcf.h>
#include "i8042.h"
#include "controllers/utils.h"

/** 
 * @file kbc.h
 * 
 * 
 * Functions to interact with the i8042 KBC.
 */


 /**
 * @brief Reads the status of the kbc.
 * 
 * @param st Variable to store the status
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_get_status)(uint8_t *st);

/**
 * @brief Reads data from a given port of the kbc.
 * 
 * @param port Port to read from
 * @param data Variable to store the read byte
 * @param mouseData Flag to check if the data is from the mouse or the keyboard
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_read_data)(uint8_t port, uint8_t *data, uint8_t mouseData);

/**
 * @brief Writes a command to a given port of the kbc.
 * 
 * @param port Port to write to
 * @param cmd Command to write
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_write_command)(uint8_t port, uint8_t cmd);

#endif /* _KBC_H */
