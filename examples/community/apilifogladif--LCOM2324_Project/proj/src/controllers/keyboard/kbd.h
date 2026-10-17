#ifndef _LCOM_KBD_H_
#define _LCOM_KBD_H_

/**
 * @file kbd.h
 * 
 * @brief This file contains the keyboard functions.
*/


/**
 * @brief Subscribes to keyboard interrupts.
 * 
 * @param IRQ : IRQ.
 * @return int : 0 if sucess and 1 otherwise.
 */
int(kbd_subscribe_int)(uint8_t *IRQ);

/**
 * @brief Unsubscribes to keyboard interrupts.
 * 
 * @return int : 0 if sucess and 1 otherwise.
 */
int(kbd_unsubscribe_int)();

/**
 * @brief Reads the output from the specified port and checks for errors.
 * 
 * @param port : The I/O port to read from.
 * @param scancode : Variable where the read scancode will be stored.
 * @param mouse : Non-zero value if the data read should be from the mouse.
 * @return int : 0 if sucess and 1 otherwise.
 */
int (read_output)(uint8_t port, uint8_t *scancode, uint8_t mouse);

/**
 * @brief Restore the keyboard.
 * 
 * @return int : 0 if sucess and 1 otherwise.
 */
int (restoreKBD)();

#endif
