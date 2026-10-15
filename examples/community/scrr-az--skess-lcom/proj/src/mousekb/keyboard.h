#ifndef _LCOM_KB_H_
#define _LCOM_KB_H_

#include <lcom/lcf.h>
#include "KBC.h"

#define MAX_STR_LEN 13

/**
 * @brief Subscribes to keyboard interrupts.
 * 
 * @param bit_no Adress to store IRQ bit mask.
 * @return return 0 on success.
 */
int keyboard_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes from keyboard interrupts.
 * 
 * @return 0 on success.
 */
int keyboard_unsubscribe_int();

/**
 * @brief Keyboard interrupt handler 
 * 
 * @param string buffer where typed characters are stored.
 * @param size_str pointer to current lenght of input string.
 * @param ESC_key bool value that is set to true when ESC key is pressed.
 * @param submit bool value that is set to true when ENTER key is pressed.
 */
void kbc_ih1(char string[], int *size_str, bool *ESC_key, bool *submit);

#endif /*_LCOM_KB_H_*/
