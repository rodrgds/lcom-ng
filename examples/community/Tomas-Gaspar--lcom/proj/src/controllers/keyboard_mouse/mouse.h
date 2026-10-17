#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <stdint.h>

/**
 * @ingroup Device_Drivers
 * @{
 * 
 * @ingroup kb_mouse
 * @{
*/

/**
 * @brief Subscribes to mouse interrupts.
 *
 * @param bit_no Pointer to store the mouse hook id.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from mouse interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler.
 *
 * This function is the interrupt handler for the mouse interrupts.
 * If the read operation is successful,it sets `ret_val` to 0, otherwise, it sets `ret_val` to 1.
 */
void (mouse_ih)();

/**
 * @brief Issues a write command to the mouse.
 *
 * This function issues a write command to the specified register.
 *
 * @param reg The register to write the command to.
 * @param cmd The command to be written.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (mouse_issue_write_cmd)(uint8_t reg, uint8_t cmd);


/**
 * @brief Reads from the keyboard controller output buffer
 *
 * @param byte Pointer to store the read data.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (mouse_read_data)(uint8_t *byte);

/**
 * @brief Enable or disable mouse data reporting.
 *
 * @param cmd The command to enable or disable data reporting.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (mouse_data_reporting)(uint8_t cmd);

/**
 * @brief Parses a mouse packet and extracts relevant information.
 *
 * @param packet An array of three bytes representing a mouse packet.
 * @return A `struct packet` object containing the parsed information.
 */
struct packet (mouse_parse_packet)(uint8_t packet[3]);

/**
 *  }@
 * }@
*/

#endif
