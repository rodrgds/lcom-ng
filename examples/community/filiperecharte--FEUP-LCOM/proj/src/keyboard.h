#include "i8254.h"
#include <stdbool.h>
#include <stdint.h>

#include "utils.h"
#include "timer.h"
/**
* @brief Keyboard subscription
*
* Subscribes the keyboard interrupts
*
* @param bit_no - pointer to bit_no to verify keyboard interrupts 
* @return Returns 0 on success, 1 otherwise
*/
int keyboard_subscribe_int(uint8_t* bit_no);
/**
* @brief Keyboard unsubscription
*
* Unsubscribes the keyboard interrupts
*
* @return Returns 0 on success, 1 otherwise
*/
int keyboard_unsubscribe_int();
/**
* @brief Keyboard's interrupt handler
*
* Handles keyboard interrupts
*
* @return Returns nothing
*/
void (kbc_ih)(void);
/**
* @brief Gets info
*
* Auxiliary function to pass on function that prints scancodes
*
* @param is_makecode - pointer to a bool - true if the scancode is a makecode
* @param scancode_size - pointer to an unsigned 8 bit integer that represents scancode size
* @param scancode_bytes - array if bytes that make up the scancode
* @param one_byte - pointer to a bool - true if the scancode is of one byte (without 0xE0)
* @return Returns nothing
*/
void get_info(bool *is_makecode, uint8_t *scancode_size, uint8_t scancode_bytes[], bool *one_byte);
/**
* @brief Reenables keyboard
*
* Reenables keyboard by writing appropriate values to appropriate registers
* @return Returns 0 on a success, 1 otherwise
*/
int kbd_reenable();
