#ifndef __PROJ_UTILS_H
#define __PROJ_UTILS_H

#include <stdint.h>
#include <stdio.h>

/** @defgroup utils utils
 * @{
 *
 * Utility functions for bit manipulation and port I/O operations.
 */

#ifndef BIT
#  define BIT(n) (1 << (n))
#endif

/**
 * @brief Gets the least significant byte from a 16-bit integer
 *
 * @param val The 16-bit value to extract the LSB from
 * @param lsb Address of memory position to be filled with the LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Gets the most significant byte from a 16-bit integer.
 *
 * @param val The 16-bit value to extract the MSB from
 * @param msb Address of memory position to be filled with the MSB
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Reads a byte from the specified port.
 *
 * Wrapper for sys_inb() that handles the conversion from 32-bit to 8-bit values
 *
 * @param port The port to read from
 * @param value Address of memory position to be filled with the value read
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_sys_inb)(int port, uint8_t *value);

/**@}*/

#endif /* __PROJ_UTILS_H */
