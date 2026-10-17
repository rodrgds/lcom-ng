#ifndef __KBC_H
#define __KBC_H

#include <stdint.h>

#include "i8042.h"

/**
 * @brief Gets the status from the keyboard register.
 *
 * @param st Variable where the status will be stored.
 * @return int 1 if an error occurs, 0 otherwise
 */
int kbc_get_status(uint8_t* st);

/**
 * @brief Reads a byte from the output buffer.
 *
 * @param data Variable where the byte will be stored.
 * @return int 1 if an error occurs, 0 otherwise
 */
int kbc_read_data(uint8_t* data);

/**
 * @brief Writes a command to the keyboard register.
 *
 * @param port The port to write the command to.
 * @param cmd The command to be written.
 * @return int 1 if an error occurs, 0 otherwise
 */
int kbc_write(uint8_t port, uint8_t cmd);

/**
 * @brief Writes a command to the keyboard register.
 *
 * @param cmd The command to be written.
 * @return int 1 if an error occurs, 0 otherwise
 */
int kbc_write_cmd(uint8_t cmd);

/**
 * @brief Writes an argument to the keyboard arguments register.
 *
 * @param arg The argument to be written.
 * @return int 1 if an error occurs, 0 otherwise
 */
int kbc_write_arg(uint8_t arg);

/**
 * @brief Checks if the output buffer is full.
 *
 * @return Returns 1 if the output buffer is full, 0 otherwise.
 */
int obf_full();

/**
 * @brief Checks if the input buffer is empty.
 *
 * @return Returns 1 if the input buffer is empty, 0 otherwise.
 */
int ibf_empty();

/**
 * @brief Checks if the a buffer is full.
 *
 * @param flag The flag to chose the buffer to check (KBC_IN_FULL or KBC_OUT_FULL).
 * @return Returns 1 if the buffer is full, 0 otherwise.
 */
int buffer_full(uint8_t flag);

/**
 * @brief Initializes the keyboard and the mouse.
 * Subscribes to the mouse and the keyboard and enables mouse data report
 * @param irq_keyboard Variable where the keyboard hook_id will be stored.
 * @param irq_mouse Variable where the mouse hook_id will be stored.
 * @return int 1 if an error occurs, 0 otherwise
 */
int kbc_init(uint8_t* irq_keyboard, uint8_t* irq_mouse);

/**
 * @brief Disables the keyboard and the mouse
 * Usubscribes to the mouse and the kayboard and disables the mouse data report
 * @return int 1 if an error occurs, 0 otherwise
 */
int kbc_disable();

#endif /* __KBC_H */
