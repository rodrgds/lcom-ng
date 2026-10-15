#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Subscribes mouse IRQ notifications with IRQ_EXCLUSIVE policy.
 * @param bit_no Pointer to store the hook bitmask.
 * @return 0 on success, 1 on failure.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse IRQ notifications.
 * @return 0 on success, 1 on failure.
 */
int (mouse_unsubscribe_int)(void);

/**
 * @brief Mouse interrupt handler. Reads one byte from the KBC output buffer.
 * Sets an internal error flag if the read fails.
 */
void (mouse_ih)(void);

/**
 * @brief Accumulates bytes from mouse_ih() and builds a complete 3-byte packet.
 *
 * Synchronises on the first byte (BIT(3) must be set). Returns true once the
 * third byte is received and the internal packet struct is ready.
 *
 * @return true when a complete, validated packet is available; false otherwise.
 */
bool (mouse_sync_and_build_packet)(void);

/**
 * @brief Returns a pointer to the last complete mouse packet.
 * Valid only after mouse_sync_and_build_packet() returns true.
 */
struct packet *(mouse_get_packet)(void);

/**
 * @brief Sends a command byte to the mouse and checks the acknowledgment.
 *
 * Retries up to 3 times on NACK (0xFE). Returns immediately on ACK (0xFA)
 * or fatal error (0xFC).
 *
 * @param cmd Command byte to send (e.g. MOUSE_EN_DATA = 0xF4).
 * @return 0 on ACK, 1 on error.
 */
int (mouse_write_cmd)(uint8_t cmd);

#endif /* _LCOM_MOUSE_H_ */
