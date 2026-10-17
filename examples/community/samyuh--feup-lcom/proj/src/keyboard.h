#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>
/** @defgroup keyboard keyboard
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

/**
 * @brief Keyboard handler
 */
void (kbc_ih)(void);

/**
 * @brief 
 *
 * @param byte More significant byte of scancode
 * 
 * @return Return true or false wether its a make or break code
 */
bool (keyboard_makecode)(uint8_t byte);

/**
 * @brief Subscribes and enables Keyboard interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int(keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(keyboard_unsubscribe_int)();

/**
 * @brief Reenable keyboard interruptions after polling
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (keyboard_reenable)();


#endif /* __TIMER_H */
