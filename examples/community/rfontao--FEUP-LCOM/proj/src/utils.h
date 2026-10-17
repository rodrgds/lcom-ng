#ifndef _UTILS_H_
#define _UTILS_H_

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

/** @defgroup Utils Utils
 * @{
 *
 * Auxiliary functions
 */

/**
 * @brief Returns the least significant byte of a 2 byte number val and return it in lsb
 * 
 * @param val           Value to get the LSB of
 * @param lsb           Return value
 * @return              1 on failure
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Returns the most significant byte of a 2 byte number val and return it in msb
 * 
 * @param val           Value to get the MSB of
 * @param msb           Return value
 * @return              1 on failure
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief a wrapper for the sys_inb function that get a uint8_t instead of a uint32_t
 * 
 * @param port          Port to read the information
 * @param value         Return value
 * @return              1 on failure
 */
int (util_sys_inb)(int port, uint8_t *value);

/**
 * @brief Transforms a 9-bit number to a 16-bit one used in the mouse
 * 
 * @param sign          Ninth bit(sign bit)
 * @param number        Number without the sign
 * @return              The 16 bit result
 */
uint16_t (ninebit16bit)(bool sign, uint8_t number);

/**@}*/

#endif
