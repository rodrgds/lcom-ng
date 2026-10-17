#ifndef __KBC_H
#define __KBC_H

#include <stdbool.h>
#include <stdint.h>

/** @defgroup kbc kbc
 *  @{
 *
 * Functions used for the programming of the KBC
 */

#define DELAY_US 20000

/**
 * @brief subscribes to keyboard interrupts and puts the given bit in bit_no
 *
 * @param[in] bit_no Bit number of the keyboard interrupt messages
 */
int(kbc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief unsubscribes mouse interrupts
 */
int(kbc_unsubscribe_int)();

/**
 * @brief Check if keyboard output buffer is full
 *
 * @return true if keyboard output buffer is full, false otherwise
 */
int(kbc_outbuf_full)();

/**
 * @brief Reenable keyboard interrupts
 *
 */
void(kbc_enable_int)();

/**
 * @brief Checks if the keyboard scancode is ready to process
 *
 * @return true if keyboard is ready to process, false otherwise
 */
bool(keyboard_ready)();

/**
 * @brief Marks the current keyboard scancode as processed
 *
 */
void(mark_scancode_processed)();

/**
 * @brief Returns the current keyboard scancode size
 *
 * @return keyboard scancode size
 */
int(get_scancode_size)();

/**
 * @brief Returns the current keyboard scancode type
 *
 * @return keyboard scancode type(MAKE/BREAK)
 */
int(get_scancode_type)();

/**@}*/

#endif
