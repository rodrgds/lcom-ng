#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

/**
 * @defgroup Device_Drivers Device Drivers
 * 
 * @brief Contains all the device drivers used
 * 
 * @{
 * 
 * @defgroup Utils Utils
 * 
 * @brief Utilitary functions used by the device drivers
 * 
 * @{
*/

/**
 * @brief Converts a number in BCD to decimal
 * 
 * @param bcd Number in BCD to be converted
 * 
 * @return Converted number in decimal
*/
int bcd_to_decimal(uint8_t bcd);

/**
 *  }@
 * }@
*/

#endif
