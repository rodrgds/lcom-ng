#ifndef _UTILS_H_
#define _UTILS_H_

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Gets the least significant byte of a 16-bit value.
 * @param val The 16-bit value.
 * @param lsb Pointer to the variable where the LSB will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Gets the most significant byte of a 16-bit value.
 * @param val The 16-bit value.
 * @param msb Pointer to the variable where the MSB will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Reads a byte from a port.
 * @param port The port to read from.
 * @param value Pointer to the variable where the value will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int (util_sys_inb)(int port, uint8_t *value);

#endif // _UTILS_H_
