#include "i8042.h"
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
/**
 * @brief Subscribes to mouse interrupts.
 *
 * @param bit_no Pointer to the variable that will store the bit mask of the subscribed interrupt.
 * @return Return 0 upon success and non-zero otherwise.
 */
int(mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from mouse interrupts.
 *
 * @return Return 0 upon success and non-zero otherwise.
 */
int(mouse_unsubscribe_int)();

/**
 * @brief Sends a command to the mouse.
 *
 * @param cmd The command to be sent.
 * @return Return 0 upon success and non-zero otherwise.
 */
int send_cmd_mouse(uint8_t cmd);

/**
 * @brief Converts bytes to a mouse packet.
 */
void bytes_to_packet();

/**
 * @brief Parses a mouse packet.
 */
void packet_parse();

/**
 * @brief Mouse interrupt handler.
 */
void(mouse_ih)();

/**
 * @brief Checks if the output buffer can be read from the mouse.
 *
 * @return Return 0 if the output buffer can be read and non-zero otherwise.
 */
int can_read_outbuf_mouse();

/**
 * @brief Changes the sample rate of the mouse.
 *
 * @param rate The new sample rate.
 * @return Return 0 upon success and non-zero otherwise.
 */
int change_sample_rate(uint8_t rate);
