#ifndef __UTILS_H
#define __UTILS_H

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Extracts the nth byte from a 32-bit value.
 *
 * @param val The 32-bit value.
 * @param n The byte to extract.
 * @return The extracted byte.
 */
uint8_t (util_get_n_byte)(uint32_t val, int n);

/**
 * @brief Retrieves the least significant byte from a 16-bit value.
 *
 * @param val The 16-bit value.
 * @param lsb Pointer to store the least significant byte.
 * @return 0 upon success, non-zero otherwise.
 */
int (util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Retrieves the most significant byte from a 16-bit value.
 *
 * @param val The 16-bit value.
 * @param msb Pointer to store the most significant byte.
 * @return 0 upon success, non-zero otherwise.
 */
int (util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Reads a byte from the specified port.
 *
 * @param port The I/O port to read from.
 * @param value Pointer to store the read byte.
 * @return 0 upon success, non-zero otherwise.
 */
int (util_sys_inb)(int port, uint8_t *value);

/**
 * @brief Converts a BCD number to binary.
 *
 * @param bcd_number The BCD number to convert.
 * @return The binary representation of the BCD number.
 */
uint8_t to_binary(uint8_t bcd_number);

/**
 * @brief Prints the bits of a value.
 *
 * @param size The size of the value.
 * @param ptr Pointer to the value.
 */
void printBits(size_t const size, void const * const ptr);

#endif
