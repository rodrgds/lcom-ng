#ifndef __PROJ_KBC_H
#define __PROJ_KBC_H

#include "controller/kbc/i8042.h"

/** @defgroup kbc kbc
 * @{
 *
 * Functions for using the i8042 Controller.
 */

/**
 * @brief Checks if the KBC is ready
 *
 * @param st The KBC status byte
 * @return Return 0 if ready, non-zero otherwise
 */
int(kbc_ready)(uint8_t st);

/**
 * @brief Reads the status from the KBC
 *
 * @param st Address of memory position to be filled with the status
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_read_st)(uint8_t *st);

/**
 * @brief Reads data from the KBC
 *
 * @param data Address of memory position to be filled with the data
 * @param expect_mouse_data Boolean indicating whether the expected data is from the mouse (true) or from the keyboard (false).
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_read_data)(uint8_t *data, bool expect_mouse_data);

/**
 * @brief Writes a command to the KBC
 *
 * @param port Port to write to
 * @param cmd Command to write
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_write_cmd)(int port, uint8_t cmd);

/**@}*/

#endif /* __PROJ_KBC_H */
