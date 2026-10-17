#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

/**
 * @ingroup Device_Drivers
 * @{
 * 
 * @ingroup kb_mouse
 * @{
*/

/**
 * @brief Subscribes to keyboard interrupts.
 *
 * @param bit_no Pointer to store the keyboard hook id.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (kb_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from keyboard interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (kb_unsubscribe_int)();

/**
 * @brief Reads from the keyboard controller output buffer
 * 
 * @return Return 0 upon successm non-zero otherwise.
*/
int (kbc_read)();

/**
 * @brief Keyboard controller interrupt handler
 * 
 * Reads from the keyboard controller output buffer
*/
void (kbc_ih)();

/** }@
 * }@
 */

#endif
