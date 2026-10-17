#ifndef CHARSET_H
#define CHARSET_H

/** @defgroup charset charset
 *  @{
 *
 * Constants and functions used for programming the charset
 */

#define NO_ADD_CHAR 0

/**
 * @brief
 * @param scancode the array containing the scancodes to be processed
 * @param scancode_size the size of the array
 * @param scancode_type indicates whether the scancode is a breakcode or a makecode
 *
 */
void(process_scancode)(uint8_t *scancode, int scancode_size, int scancode_type);

/**@}*/

#endif
