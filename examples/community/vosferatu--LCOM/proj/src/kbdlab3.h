#ifndef __KBDLAB3_H
#define __KBDLAB3_H


/** @defgroup kbdlab3 kbdlab3
 * @{
 *
 * Functions for dealing with keyboard
 */


#define LED_CMD 0xED



/**
 * @brief assembly function to read from kbc
 * @return scancode
 */
unsigned long kbd_handler();

/**
 * @brief Subscribes and enables keyboard exclusive interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int keyboard_subscribe_int(void);

/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int keyboard_unsubscribe_int();

/**
 * @brief Keyboard interrupt handler
 *
 * return break/make code we want to print
 */
unsigned long keyboard_c_handler();


/**
 * @brief Writes a command to kbd
 *
 * @param command: command we want to send
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_write(unsigned long command);


/**
 * @brief Reads from outbuff
 *
 * @param *data: data to receive placed here
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_read(unsigned long *data);

/** @} */

#endif /* __KBDLAB3_H */
