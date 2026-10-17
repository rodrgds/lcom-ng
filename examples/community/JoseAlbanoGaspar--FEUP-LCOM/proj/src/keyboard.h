#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "macros/i8042.h"

/**
 * @brief subscribes keyboard's interrupts
 * @return Returns 0 upon success and non-zero otherwise
 */
int (kbd_subscribe_int)(uint8_t *bit_no);
/**
 * @brief unsubscribes keyboard's interrupts
 * @return Returns 0 upon success and non-zero otherwise
 */
int (kbd_unsubscribe_int)();
/**
 * @brief Reads the scancode byte from the output buffer;
 * @param *data Pointer to unsigned long variable that will store the data read from the out buffer
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_read_buffer)(uint16_t * data);
/**
 * @brief reads status byte from kbc controller
 * @param *status Pointer to unsigned long variable that will store new status byte
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_read_status)(uint8_t * status);
/**
 * @brief after read the status register, check if there was some communications errors
 * @return Return 0 upon success and non-zero otherwise
 */
int (isValidStatus)();
/**
 * @brief reads the scancodes from the KBC using interrupts
 */
void (kbc_ih)(void);
/**
 * @brief when pressed key is not ESC, it resets the scancode
 */
void (kbc_reset_scancode)(void);
/**
 * @brief reads the scancodes from the KBC using polling
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_poll)();
/**
 * @brief prints the scancode
 */
void (kbc_print)();


#endif /* _LCOM_KEYBOARD_H_ */
