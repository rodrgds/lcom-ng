#ifndef _MOUSE_H_
#define _MOUSE_H_

/**
 * @file mouse.h
 * @brief Mouse interface functions.
 */

#include <lcom/lcf.h>
#include "../KBC/i8042.h"
#include "../KBC/KBC.h"



/** @defgroup mouse mouse
 * @{
 */

/**
 * @brief Subscribes to mouse interrupts.
 *
 * @param bit_no Pointer to a variable where the mouse IRQ bit mask will be stored.
 * @return 0 on success, non-zero value on failure.
 */
int (mouse_subscribe_int)(uint32_t *mouse_irq);
/**
 * @brief Unsubscribes from mouse interrupts.
 *
 * @return 0 on success, non-zero value on failure.
 */
int (mouse_unsubscribe_int)();
/**
 * @brief Mouse interrupt handler.
 */
void (mouse_ih)();
/**
 * @brief Enables data reporting for the mouse.
 *
 * @return 0 on success, non-zero value on failure.
 */
int (enable_data_reporting)();
/**
 * @brief Disables data reporting for the mouse.
 *
 * @return 0 on success, non-zero value on failure.
 */
int (disable_data_reporting)();
/**
 * @brief Parses the mouse packet.
 */
void (parse_packet)();
/**
 * @brief Synchronizes mouse packets.
 */
void (sync_packet)();

/** @} */ // end of Mouse group

#endif

