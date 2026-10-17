/**
 * @file keyboard.h
 * 
 * @brief Functions for using the Keyboard
*/


#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "KBC.h"

/** @defgroup keyboard keyboard
 * @{
 *
 * @brief  keyboard related functions
 * 
 */

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions for using the Keyboard
 */



/**
 * @brief Subscribes Keyboard interrupts and disables default IH
 *
 * @param irq_set_mask address of memory to be initialized with mask with bit number keyboard_hook_id set
 * @return Return 0 upon success and non-zero otherwise
 */
int(keyboard_subscribe_int)(uint8_t *irq_set_mask);



/**
 * @brief Unsubscribes Keyboard interrupts
 * @return Return 0 upon success and non-zero otherwise
 */
int(keyboard_unsubscribe_int)();



/**
 * @brief Configure KBC to enable interrupts on OBF from Keyboard
 * @return Return 0 upon success and non-zero otherwise
 */
int (keyboard_int_enabler)();




/**
 * @brief Configure KBC to disable interrupts on OBF from Keyboard
 * @return Return 0 upon success and non-zero otherwise
 */
int (keyboard_int_disabler)();


/**
 * @brief Reads a byte coming from the keyboard and tries to build a scancode
 */
void (kbc_ih)();

/** @} end of keyboard */


#endif /* __KEYBOARD_H */





