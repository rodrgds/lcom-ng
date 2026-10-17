#ifndef _LCOM_UTILS_H_
#define _LCOM_UTILS_H_

#include <lcom/lcf.h>
#include "src/controllers/libkbc/i8042.h"
#include <stdint.h>

/*!
 * @brief Reads a byte from the specified port.
 *
 * Reads a byte from the given port and stores it in the provided variable.
 *
 * @param[in] port Port to read from.
 * @param[out] value Pointer to store the read value.
 * @return 0 on success, non-zero otherwise.
 */
int (util_sys_inb)(int port, uint8_t *value);

/*!
 * @brief Reads the status register of the keyboard controller.
 *
 * Reads the status register of the keyboard controller and stores its value in a global variable.
 *
 * @return 0 on success, non-zero otherwise.
 */
int read_status_register();

/*!
 * @brief Reads a scancode from the keyboard buffer.
 *
 * Reads a scancode from the keyboard buffer, handling errors and special cases.
 *
 * @return 0 on success, non-zero otherwise.
 */
int read_buffer();

/*!
 * @brief Prints the current scancode.
 *
 * Prints the scancode read from the keyboard buffer using the appropriate library function.
 *
 * @return 0 on success, non-zero otherwise.
 */
int print_code();

/*!
 * @brief Extracts the least significant byte (LSB) of a 16-bit value.
 *
 * Extracts the LSB of the given 16-bit value and stores it in the provided variable.
 *
 * @param[in] val 16-bit value to extract the LSB from.
 * @param[out] lsb Pointer to store the extracted LSB.
 * @return 0 on success, non-zero otherwise.
 */
int (util_get_LSB)(uint16_t val, uint8_t *lsb);

/*!
 * @brief Extracts the most significant byte (MSB) of a 16-bit value.
 *
 * Extracts the MSB of the given 16-bit value and stores it in the provided variable.
 *
 * @param[in] val 16-bit value to extract the MSB from.
 * @param[out] msb Pointer to store the extracted MSB.
 * @return 0 on success, non-zero otherwise.
 */
int (util_get_MSB)(uint16_t val, uint8_t *msb);

#endif
