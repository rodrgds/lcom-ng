#ifndef KBC_H
#define KBC_H

#include <lcom/lcf.h>
#include "i8042.h"

#define DELAY_US 20000

/** @defgroup kbc kbc
 *  @brief Functions for using the KBC
*/

/**
 * @brief Gets the status of the KBC
 * @param st Pointer to the variable where the status will be stored
 * @return 0 upon success, 1 otherwise
*/
int (kbc_get_status)(uint8_t *st);

/**
 * @brief Reads the data from the KBC
 * @param port Port to read from
 * @param output Pointer to the variable where the data will be stored
 * @param mouse True if the data is from the mouse, false otherwise
 * @return 0 upon success, 1 otherwise
*/
int (kbc_read_data)(uint8_t port, uint8_t *output, bool mouse);

/**
 * @brief Writes a command to the KBC
 * @param port Port to write to
 * @param cmd Command to write
 * @return 0 upon success, 1 otherwise
*/
int (kbc_write_cmd)(uint8_t port, uint8_t cmd);

#endif
