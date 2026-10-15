#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../macros/includes.h"


/**
 * @brief Subscribes the keyboard device, setting into the interrupt mask the respective keyboard hook id bit to 1
 * @return Returns the bit number that was set if successfull or 0xFF if unsuccessfull
 */
uint8_t (keyboard_subscribe)();
/**
 * @brief Unsubscribes the keyboard device, setting into the interrupt mask the respective keyboard hook id bit to 0
 * @return Returns 0 if successfull and 1 if otherwise
 */
int (keyboard_unsubscribe)();

/**
 * @brief Reads the value of input made by user from a port given by argument and stores result on variable command in argument
 * @param port Port from which value is being read
 * @param command Pointer to stored read value
 * @return Returns the read value or 0xFFFF if error
 */
int (kbc_read_output)(int port, uint32_t *command);
/**
 * @brief Writes to the keyboard port a certain command
 * @param port Port from which value is being written
 * @param command Command being written into port
 * @return Returns 0 if successfull and 0xFFFF if otherwise
 */
int (kbc_write_output)(int port, unsigned long command);
/**
 * @brief Restores the keyboard to it's original value
 * @return Returns 0 if successfull and 1 if otherwise
 */
int (keyboard_restore)();

#endif
