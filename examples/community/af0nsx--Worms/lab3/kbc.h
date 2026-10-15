#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <stdint.h>

/**
 * @brief Reads the KBC status register.
 * @param status Pointer to store the status byte.
 * @return 0 on success, 1 on failure.
 */
int (kbc_read_status)(uint8_t *status);

/**
 * @brief Reads one byte from the KBC output buffer.
 *
 * Polls until OBF is set, validates parity/timeout errors and the expected
 * data source (keyboard vs. mouse).
 *
 * @param port   Port to read from (KBC_OUT_BUF = 0x60).
 * @param data   Pointer to store the byte read.
 * @param mouse  KBC_SOURCE_MOUSE or KBC_SOURCE_KEYBOARD.
 * @return 0 on success, 1 on error.
 */
int (kbc_read_data)(uint8_t port, uint8_t *data, uint8_t mouse);

/**
 * @brief Writes a command byte to a KBC port.
 *
 * Polls until IBF is clear before writing.
 *
 * @param port         Port to write to (KBC_CMD_REG or KBC_IN_BUF).
 * @param command_byte The command/data byte to send.
 * @return 0 on success, 1 on error.
 */
int (kbc_write_command)(uint8_t port, uint8_t command_byte);

/**
 * @brief Non-blocking read from the KBC output buffer for use in interrupt handlers.
 *
 * Unlike kbc_read_data(), this function never calls tickdelay(). It reads the
 * status register once: if OBF is set and the source matches, it reads the byte
 * and returns 0. Otherwise it returns 1 immediately.
 *
 * @param data   Pointer to store the byte read.
 * @param mouse  KBC_SOURCE_MOUSE or KBC_SOURCE_KEYBOARD.
 * @return 0 on success, 1 if OBF not set / error / wrong source.
 */
int (kbc_read_data_ih)(uint8_t *data, uint8_t mouse);

#endif /* _LCOM_KBC_H_ */
