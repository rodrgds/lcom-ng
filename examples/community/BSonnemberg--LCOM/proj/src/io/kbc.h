#pragma once

#include "i8042.h"
#include <stdint.h>

#define TIMEOUT 1

/**
 * @brief Get status/ configuration of the kbc
 * @param st placeholder for status
 * @return 0 if successful, non-0 otherwise
*/
int kbc_get_status(uint8_t *st);

/**
 * @brief Read outbuf buffer of the kbc
 * @param buf placeholder for buffer
 * @return 0 if successful, non-0 otherwise
*/
int kbc_read_outbuf(uint8_t *buf);

/**
 * @brief Write byte to a kbc port
 * @param port port to write to
 * @param byte byte to be written
 * @return 0 if successful, non-0 otherwise
*/
int kbc_write(uint8_t port, uint8_t byte);
