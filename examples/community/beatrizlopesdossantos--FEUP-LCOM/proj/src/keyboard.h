#include <lcom/lcf.h>

/**
 * @defgroup keyboard
 * @{
 * @brief Functions of the Keyboard
 */

/**
* Subscribes and enables KBD 1 interrupts.
* @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
* @return Return 0 upon success and non-zero otherwise
*/
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
* Unsubscribes KBD 1 interrupts.
* @return Return 0 upon success and non-zero otherwise
*/
int (kbd_unsubscribe_int)();

/**
* Handles keyboard interrupts (C implementation)
*
* Reads the status register and the output buffer (OB).
* If there was some error, the byte read from the OB should be discarded.
*
* All communication with other code must be done via global variables, static if possible.
* @param IH'stake no arguments
* @return IH's return no values
*/
void (kbc_ih)();

/** @} end of keyboard */
