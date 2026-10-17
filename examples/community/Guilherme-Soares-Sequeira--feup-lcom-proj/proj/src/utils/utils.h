#include <lcom/lcf.h>

#ifndef UTILS_H
#  define UTILS_H

/** @defgroup utils utils
 *  @{
 * 
 * Utility functions and constants used throughout our program
 */

/* masks used to fetch the LSB and MSB of a 16-bit integer */

#  define LSB_MASK16 0x00FF     /**< @brief a mask used to get the least significant byte of a 16 bit variable */
#  define LSB_MASK32 0x000000FF /**< @brief a mask used to get the least significant byte of a 32 bit variable */
#  define MSB_MASK16 0xFF00     /**< @brief a mask used to get the most significant byte of a 16 bit variable */
#  define MSB_MASK32 0XFF000000 /**< @brief a mask used to get the most significant byte of a 32 bit variable */

/**
 * @brief Gets the least significant byte of a 16 bit variable
 *
 * @param val the 16 bit variable whose LSB will be returned
 *
 * @param lsb pointer to the variable where the LSB will be stored
 *
 * @return OK if successful
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Gets the most significant byte of a 16 bit variable
 *
 * @param val the 16 bit variable whose MSB will be returned
 *
 * @param msb pointer to the variable where the MSB will be stored
 *
 * @return OK if successful
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Performs sys_inb on a temporary 32 bit variable and returns it in on an 8 bit format
 *
 * @param port the port that should be used in sys_inb
 *
 * @param value pointer to the 8 bit variable where the result of sys_inb will be stored
 *
 * @return 0 if successful, 1 otherwise
 */
int(util_sys_inb)(int port, uint8_t *value);

/**
 * @brief Represents a 16 bit value in a BCD format. Should be handled with care so as to not occur overflow
 *
 * @param val the value that will be converted to BCD
 *
 * @return the BCD representation of val
 */
uint16_t(to_bcd)(uint16_t val);

/**@}*/

#endif
