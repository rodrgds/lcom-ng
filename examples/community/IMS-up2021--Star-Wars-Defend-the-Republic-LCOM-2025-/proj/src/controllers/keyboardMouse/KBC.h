#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/syslib.h>
#include "i8042.h" 
#include <lcom/lcf.h> 

/**
 * @file kbc.h
 * @brief Low-level functions for interacting with the i8042 Keyboard Controller (KBC).
 *
 * This file declares utility functions for reading the KBC status register,
 * writing commands or data to KBC ports, and reading data from the KBC output buffer.
 * These functions often form the building blocks for higher-level keyboard and mouse drivers.
 */

/** @defgroup kbc KBC Utilities
 * @{
 *
 * Low-level utility functions for direct interaction with the i8042 Keyboard Controller.
 */

/**
 * @brief Reads the status register of the Keyboard Controller (KBC).
 *
 * This function polls the KBC status register (KBC_STAT_REG, 0x64) to retrieve its current state.
 * The status byte contains flags indicating buffer states (Input Buffer Full, Output Buffer Full),
 * error conditions (Parity, Timeout), and other KBC states.
 *
 * @param status Pointer to a variable where the status byte read from the KBC will be stored.
 * @return int 0 upon success, 1 if there was a timeout or error reading the status.
 */
int (read_KBC_status)(uint8_t* status);

/**
 * @brief Writes a byte to a specified KBC port.
 *
 * This function attempts to write a command or data byte (`cmdByte`) to the specified KBC I/O port (`port`).
 * It typically checks the KBC status register (specifically the Input Buffer Full - IBF flag)
 * to ensure the KBC is ready to accept data before writing. It will retry a few times if the IBF is set.
 *
 * @param port The KBC I/O port to write to (e.g., KBC_IN_CMD for KBC commands, KBC_OUT_CMD for data to keyboard/mouse if 0xD4 was issued).
 * @param cmdByte The byte (command or data) to be written to the port.
 * @return int 0 upon success (byte written), 1 if there was a timeout (KBC not ready after retries) or other error.
 */
int (kbc_write_byte)(uint8_t port, uint8_t cmdByte);


/**
 * @brief Reads a byte from the KBC output buffer.
 *
 * This function attempts to read a byte from the KBC output buffer (KBC_OUT_CMD, 0x60).
 * It first checks the KBC status register (specifically the Output Buffer Full - OBF flag)
 * to ensure data is available. It will retry a few times if OBF is not set.
 * The `mouse` parameter can influence error checking or handling, for instance,
 * to distinguish parity/timeout errors relevant to mouse communication.
 *
 * @param port The KBC I/O port to read from (typically KBC_OUT_CMD, 0x60).
 * @param cmdByteOut Pointer to a variable where the byte read from the KBC output buffer will be stored.
 * @param mouse Flag indicating if the read operation is related to mouse data (1 for mouse, 0 for keyboard/other).
 *              This can affect error checking (e.g., expecting AUX bit in status for mouse data).
 * @return int 0 upon success (byte read), 1 if there was a timeout (no data available after retries) or other error.
 */
int (read_kbc_out)(uint8_t port, uint8_t *cmdByteOut, uint8_t mouse);


/**@}*/

#endif /* _LCOM_KBC_H_ */
