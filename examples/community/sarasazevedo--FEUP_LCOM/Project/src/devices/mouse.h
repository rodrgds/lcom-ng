/**
 * @file mouse.h
 * 
 * @brief Functions for using the Mouse
*/

#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "KBC.h"

/** @defgroup mouse mouse
 * @{
 *
 * @brief  mouse related functions
 * 
 */

/**
 * @brief Reads a byte and tries to build a mouse packet
*/
void (mouse_ih)();


/**
 * @brief Organizes the mouse packet information in a packet struct
*/
void (mouse_parse_packet)();


/**
 * @brief Subscribes and enables Mouse Interrupts
 *
 * @param irq_set_mask  Mask with the mouse_hook_id bit set
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_subscribe_int)(uint8_t *irq_set_mask);


/**
 * @brief Unsubscribes Mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_unsubscribe_int)();



/**
 * @brief Sends Mouse Command Byte to the Mouse
 * 
 * @param byte The byte to be written
 * @return Return 0 upon success and non-zero otherwise
*/
int (mouse_write_byte)(uint8_t byte);



/**
 * @brief Disable data reporting (in Stream Mode)
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int (mouse_disable_data_report)();



/**
 * @brief Enable data reporting (in Stream Mode)
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int (mouse_enable_data_report)();


/**
 * @brief Set Mouse to operate in Stream Mode
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int (mouse_set_stream_mode)();



/**
 * @brief Set Mouse to operate in Remote Mode
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int (mouse_set_remote_mode)();

/** @} end of mouse */


#endif /* __MOUSE_H */
