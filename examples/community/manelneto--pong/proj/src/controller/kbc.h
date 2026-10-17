#ifndef _KBC_H_
#define _KBC_H_

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

/** @defgroup kbc kbc
 * @{
 *
 * Functions for using the i8042 KBC
 */

/**
 * @brief Reads the KBC status
 *
 * @param status address of memory position to be filled with the KBC status
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_read_status)(uint8_t *status);

/**
 * @brief Reads the KBC output
 *
 * @param output address of memory position to be filled with the KBC output
 * @return Return 0 upon success and non-zero otherwise (-1 if invalid data, 1 if internal error, 2 if exceeded maximum number of attempts to read KBC output)
 */
int(kbc_read_output)(uint8_t *output);

/**
 * @brief Reads the KBC acknowledgment byte from the output buffer
 *
 * @param acknowledgment_byte address of memory position to be filled with the KBC acknowledgment byte
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_read_acknowledgment_byte)(uint8_t *acknowledgment_byte);

/**
 * @brief Writes command to KBC
 *
 * @param command command to be written to KBC
 * @return Return 0 upon success and non-zero otherwise (1 if internal error, 2 if exceeded maximum number of attempts to write command to KBC)
 */
int(kbc_write_command)(uint8_t command);

/**
 * @brief Writes argument to KBC
 *
 * @param argument argument to be written to KBC
 * @return Return 0 upon success and non-zero otherwise (1 if internal error, 2 if exceeded maximum number of attempts to write argument to KBC)
 */
int(kbc_write_argument)(uint8_t argument);

/**@}*/

#endif /* _KBC_H_ */
