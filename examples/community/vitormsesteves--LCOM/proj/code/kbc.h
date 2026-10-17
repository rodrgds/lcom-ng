#pragma once

/** @defgroup kbc kbc
 * @{
 * Functions for keyboard controller
 */


/**
 * @brief Write a kbc command to a port
 *
 * @param mouse Command to write
 * @param port Port to write the command
 * @return validation value
 */
int kbc_write(unsigned char cmd, unsigned long port);

/**
 * @brief Read a kbc command
 *
 * @return read value
 */
int kbc_read();
