#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8024_kbc.h"

/**
 * @brief Subscribes to mouse interrupts with the IRQ_REENABLE and IRQ_EXCLUSIVE policies.
 * @param bit_no Bit number to be used in the mouse hook ID.
 * @return 0 if successful, non-zero otherwise.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes to mouse interrupts.
 * @return 0 if successful, non-zero otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Processes mouse interrupts by reading the KBC output buffer and arranging mouse packets in a struct.
 */
void (mouse_ih)();

/**
 * @brief Writes a command byte directly to the mouse.
 * @param command Command byte to be written.
 * @return 0 if successful, non-zero otherwise.
 */
int (writeCommand)(uint8_t command);

/**
 * @brief Writes a command byte to the KBC.
 * @param port KBC port address.
 * @param command Command byte to be written.
 * @return 0 if successful, non-zero otherwise.
 */
int (writeToKBC)(uint8_t port, uint8_t command);

#endif
