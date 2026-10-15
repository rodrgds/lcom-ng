#ifndef _LCOM_DRIVER_UTILS_H_
#define _LCOM_DRIVER_UTILS_H_

#include <lcom/lcf.h>

/** @defgroup drivers Drivers */

/**
 * @defgroup driver_utils Utils
 * @ingroup drivers
 * @{
 * 
 * @brief Utility functions.
 */

/**
 * @brief Get the most least bit of a value.
 * 
 * @param val Value
 * @param lsb Pointer to where the result will be saved
 * @return 0 if sucessful, 1 otherwise
*/
int (util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Get the most significant bit of a value.
 * 
 * @param val Value
 * @param msb Pointer to where the result will be saved
 * @return 0 if sucessful, 1 otherwise
*/
int (util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Reads the information in a register and converts 
 * it to a 8-bit unsigned integer.
 * 
 * @param port Register code
 * @param value Pointer to where the result will be saved
 * @return 0 if sucessful, 1 otherwise
*/
int (util_sys_inb)(int port, uint8_t *value);

/** @} */

#endif /* _LCOM_DRIVER_UTILS_H */
