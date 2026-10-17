#include <lcom/lcf.h>

#include <stdbool.h>

#include "../../utils/utils.h"
#include "PS2.h"
#include "i8042.h"

/** @defgroup mouse mouse
 *  @{
 *
 * Functions used for the programming of the mouse
 */

#define mouse_packet_t struct packet

/**
 * @brief subscribes to mouse interrupts and puts the given bit in bit_no
 *
 * @param[in] bit_no Bit number of the mouse interrupt messages
 */
int(mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief unsubscribes mouse interrupts
 */
int(mouse_unsubscribe_int)();

/**
 * @brief Check if mouse output buffer is full
 *
 * @return true if mouse output buffer is full, false otherwise
 */
bool(kbc_mouse_data)();

/**
 * @brief Check if mouse left button is pressed
 *
 * @return true if left button is pressed, false otherwise
 */
bool(lb_is_pressed)();

/**
 * @brief Get the current mouse packet
 *
 * @return the current mouse packet
 */
mouse_packet_t(get_mouse_packet)();

/**
 * @brief Resets the KBC to the default Minix state
 *
 */
void(reset_kbc)();

/**
 * @brief waits for the KBC input buffer to be empty
 *
 */
void(wait_for_inbuff_empty)();

/**
 * @brief Writes the given command to the mouse
 *
 * @param command The command to write
 */
void(write_command_to_mouse)(uint8_t command);

/**
 * @brief Enables KBC Mouse data reporting
 *
 */
void(kbc_enable_data_report)();

/**
 * @brief Get mouse command feedback
 *
 */
uint8_t(mouse_command_feedback)();

/**
 * @brief Convert an unsigned 8-bit value into a signed 16-bit value.
 *
 * @param value The unsigned 8-bit value
 * @param sign the sign of the resulting number
 *
 * @return The 2s-complement signed 16-bit equivalent of the 8-bit value passed in
 */
int16_t(uint8_to_int16)(uint8_t uint8_val, bool sign);

/**
 * @brief Checks if the mouse packet has been fully processed
 *
 * @return true if the packet has been fully processed
 */
bool(mouse_packet_ready)();

/**@}*/
