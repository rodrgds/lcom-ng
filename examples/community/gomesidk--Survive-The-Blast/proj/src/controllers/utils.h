#ifndef _UTILS_H_
#define _UTILS_H_

#include <lcom/lcf.h>

/**
 * @file utils.h
 * @brief Utility functions for the game
 * 
 * Functions for utility operations
 */


/**
* @brief Converts a 16-bit value to its least significant byte
*
* @param val Value to convert
* @param lsb Pointer to store the least significant byte
* @return Return 0 upon success and non-zero otherwise
*/
int (util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Converts a 16-bit value to its most significant byte
 *
 * @param val Value to convert
 * @param msb Pointer to store the most significant byte
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Reads a byte from a given port
 * 
 * @param port Port to read from
 * @param value Variable to store the read byte
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_sys_inb)(int port, uint8_t *value);

/**@}*/

#endif
