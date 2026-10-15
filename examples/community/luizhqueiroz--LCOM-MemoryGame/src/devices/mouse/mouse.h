/**
 * @file mouse
 * @brief Functions for using the mouse
 */

#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

/** @defgroup Mouse Mouse
 * @{
 *
 * Functions for using the mouse
 */

#include <stdint.h>
#include "i8042_mouse.h"

/**
 * @brief Reads the mouse configuration
 *
 * @param st The value of the mouse configuration
 * @return Return 0 upon success and 1 otherwise
 */
int (mouse_read_config)(uint8_t *st);

/**
 * @brief Reads the mouse output buffer 
 *
 * @param output The data in the output buffer
 * @return Return 0 upon success and 1 otherwise
 */
int (mouse_read_out_buf)(uint8_t *output);

/**
 * @brief Subscribes mouse interrupt
 *
 * @param bit_no Bit number of mouse IRQ
 * @return Return 0 upon success and 1 otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupt
 * @return Return 0 upon success and 1 otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Enables the mouse stream mode
 * @return Return 0 upon success and 1 otherwise
 */
int (mouse_enable_stream_mode)();

/**
 * @brief Enables the mouse data reporting
 * @return Return 0 upon success and 1 otherwise
 */
int (mouse_enable_dt_reporting)();

/**
 * @brief Disables the mouse data reporting
 * @return Return 0 upon success and 1 otherwise
 */
int (mouse_disable_dt_reporting)();

/**
 * @brief Performs action in the mouse with the command byte
 *
 * @param cmd Mouse command
 * @param cmdb  Second argument for the command byte
 * @return Return 0 upon success and 1 otherwise
 */
int (mouse_cmd)(uint8_t cmd, uint8_t cmdb);

/**
 * @brief Parse the packet obtained in the mouse interrupts
 */
void (mouse_parse_packet)();

/**
 * @brief Mouse interrupt handler
 */
void (mouse_ih)();

/**
 * @brief Updates the mouse location based on the screen size and the parsed packets from the mouse
 */
void (mouse_update_position)();

/**@}*/

#endif
