/**
 * @file KBC.h
 * @brief PS/2 Keyboard Controller (KBC) interface functions
 * 
 * This file provides low-level functions for interacting with the PS/2
 * Keyboard Controller, which manages communication with both keyboard
 * and mouse devices. These functions handle reading the KBC status,
 * reading from the output buffer, and writing commands to the KBC.
 */

#ifndef KBC_H
#define KBC_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"

/**
 * @brief Reads the status byte from the KBC status register
 * 
 * @param[out] status Pointer to store the status byte
 * @return 0 upon success, 1 on error reading from the status register
 */
int (read_KBC_status)(uint8_t* status);

/**
 * @brief Reads output from the KBC's output buffer
 * 
 * Attempts to read the output buffer after checking the status register
 * to ensure it contains data. Includes timeout and error checking with
 * multiple retry attempts if the output buffer is not immediately ready.
 * 
 * @param[in] port The port to read from (typically OUT_BUF)
 * @param[out] output Pointer to store the byte read from the output buffer
 * @return 0 upon success, 1 on error (e.g., timeout, parity error, or read failure)
 */
int (read_KBC_output)(uint8_t port, uint8_t *output);

/**
 * @brief Writes a command byte to the specified KBC port
 * 
 * Attempts to write a command byte to the KBC after checking that the input
 * buffer is not full. Includes multiple retry attempts if the input buffer
 * is initially full.
 * 
 * @param[in] port The port to write to (typically KBC_CMD_REG or KBC_ARGS)
 * @param[in] commandByte The command byte to write
 * @return 0 upon success, 1 on error (e.g., write failure or input buffer always full)
 */
int (write_KBC_cmd)(uint8_t port, uint8_t commandByte);

#endif 
