#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "i8042.h"


/**
 * @brief Subscribes keyboard interrupts
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes keyboard interrupts
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_unsubscribe_int)();
/**
 * @brief Mask function of sys_inb(port_t port, uint32_t *byte)
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_sys_inb)(int port, uint8_t *value);
/**
 * @brief Processes the scancode. Checks if it is a make or a break code, gets it's size in bytes and gets an array with size elements, with the scancode bytes

 * 
 * @param scancode 
 */
void processKeyScancode(uint8_t scancode);
/**
 * @brief Reads from the output buffer 0x60
 * 
 * @param command_byte 
 * @return Return 0 upon success and non-zero otherwise
 */
int readCommandByte(uint8_t command_byte);
/**
 * @brief Writes to the output buffer 0x60
 * 
 * @param command_byte 
 * @return Return 0 upon success and non-zero otherwise
 */
int writeCommandByte(uint8_t command_byte);
/**
 * @brief Checks status register and if inputBuffer is full
 * 
 * @param command_byte 
 * @return Return 0 upon success and non-zero otherwise
 */
int processCommandByte(uint8_t command_byte);
/**
 * @brief Enables Minix interrupts
 * @return Return 0 upon success and non-zero otherwise
 */
void enableInterrupts();

#endif
/* __KEYBOARD_H */
