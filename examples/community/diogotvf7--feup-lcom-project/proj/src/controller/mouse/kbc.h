#ifndef __KBC_H
#define __KBC_H

#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @brief Reads the mouse status.
 *
 * @param st Pointer to store the status read from the mouse.
 * @return 0 upon success, non-zero otherwise.
 */
int read_mouse_status(uint8_t *st);

/**
 * @brief Reads the output buffer of the mouse.
 *
 * @param out Pointer to store the data read from the output buffer.
 * @return 0 upon success, non-zero otherwise.
 */
int read_output_buffer(uint8_t *out);

/**
 * @brief Writes a command to the mouse.
 *
 * @param port The port to write the command to.
 * @param cmd The command to be written.
 * @return 0 upon success, non-zero otherwise.
 */
int mouse_write(int port, uint8_t cmd);


#endif /* __KBC_H */
