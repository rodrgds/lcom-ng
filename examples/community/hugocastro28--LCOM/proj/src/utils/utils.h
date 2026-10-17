#ifndef _LCOM_UTILS_H_
#define _LCOM_UTILS_H_

/**
 * @brief Computes the least significant byte (LSB) of a 16-bit number.
 * @param val is the number whose LSB will be computed.
 * @param lsb is the variable where the LSB will be stored.
 * @return 0 upon success and non-zero otherwise.
 * 
 */
int (util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Computes the most significant byte (MSB) of a 16-bit number.
 * @param val is the number whose LSB will be computed.
 * @param msb is the variable where the MSB will be stored.
 * @return 0 upon success and non-zero otherwise.
 * 
 */
int (util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Reads the value stored in a memory position and transforms it to a 8-bit number.
 * @param port is the memory position.
 * @param value is the variable where the read value will be stored.
 * @return 0 upon success and non-zero otherwise.
 */
int (util_sys_inb)(int port, uint8_t *value);

#endif
