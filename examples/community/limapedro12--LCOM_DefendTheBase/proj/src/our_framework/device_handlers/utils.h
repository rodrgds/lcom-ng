#ifndef UTILS_C
#define UTILS_C

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief gets the least significant byte of a 16 bit number
 * @param val 16 bit number
 * @param lsb pointer to the least significant byte
 * @return 0 
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief gets the most significant byte of a 16 bit number 
 * @param val 16 bit number
 * @param msb pointer to the most significant byte
 * @return 0 
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief does the same as sys_inb but with a uint8_t pointer
 * @param port port to read from
 * @param  value pointer to the value read
 * @return true if success, false otherwise
 */
int (util_sys_inb)(int port, uint8_t *value);


#endif

