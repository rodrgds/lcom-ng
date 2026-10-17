#ifndef MOUSE_H
#define MOUSE_H

#include "drivers/keyboard/kbc.h"

extern int32_t kbd_subscription_id; /**< Keyboard subscription ID */

/**
 * @brief Subscribes and enables Mouse interrupts.
 *
 * @param bit_no Will hold the bit (BIT(hook_id)) to be set in the mask returned upon an interrupt.
 * @return Return 0 upon success and non-zero otherwise.
 */
int(mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Mouse interrupts.
 *
 * @return Return 0 upon success and non-zero otherwise.
 */
int(mouse_unsubscribe_int)(void);

/**
 * @brief Mouse interrupt handler.
 *
 * Tries to read a packet byte from the kbc, if any error occurs tries again until READ_ATTEMPT_LIMIT.
 */
void(mouse_int_handler)(void);

/**
 * @brief Wrapper of mouse_int_handler.
 *
 * Used to pass tests, should not be used so that IH names are coherent.
 */
void(mouse_ih)(void);

/**
 * @brief Reads packets from the Mouse.
 *
 * @return Return 0 upon success and non-zero otherwise.
 */
int(mouse_read_packet)(void);

/**
 * @brief Reads packets from the Mouse.
 *
 * @param packet Packet array to be synced.
 * @param packet_idx Index in the packet array.
 * @param packet_byte Last packet byte read from the mouse.
 * @return Return 0 upon success and non-zero otherwise.
 */
int(mouse_sync_packets)(uint8_t *packet, uint8_t *packet_idx, uint8_t packet_byte);

/**
 * @brief Builds packets read from the Mouse.
 *
 * @param packet array that stores packet bytes.
 * @param packet_idx index in the packet array.
 * @param pp packet returned.
 * @return Return 0 upon success and non-zero otherwise.
 */
int(mouse_build_packet)(uint8_t *packet, uint8_t *packet_idx, struct packet *pp);

/**
 * @brief Sets/Unsets data reporting.
 *
 * @param b set/unset data reporting.
 * @return Return 0 upon success and non-zero otherwise.
 */
int(mouse_set_data_reporting)(bool b);

#endif /* __MOUSE_H */
