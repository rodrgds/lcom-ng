#ifndef _LCOM_UTILS_H_
#define _LCOM_UTILS_H_

#include <stdint.h>

/**
 * @brief receives a 16 bit value and extracts least signifcant bits
 * 
 * @param val 16 bit value
 * @param lsb pointer to least significant bits destination
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief receives a 16 bit value and extracts most signifcant bits
 * 
 * @param val 16 bit value
 * @param msb pointer to most significant bits destination
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief receives a 32 bit value and extracts 8 least signifcant bits
 * 
 * @param port address of the port to be read
 * @param byte pointer 8 least significant bits destination
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (util_sys_inb)(int port, uint8_t *byte);

#endif /* _LCOM_UTILS_H_ */
