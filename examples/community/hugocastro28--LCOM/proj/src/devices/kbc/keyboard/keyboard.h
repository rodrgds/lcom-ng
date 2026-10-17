#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

/**
 * @brief Subscribes the keyboard interruptions.
 * 
 */
int (keyboard_subscribe_int)(uint8_t* bit_no);

/**
 * @brief Unsubscribes the keyboard interruptions.
 * 
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Inserts the new scan byte to the array of scan bytes.
 * 
 */
void (keyboard_int_handler)();

/**
 * @brief 
 * 
 */
int (kbc_enable_int)();

#endif
