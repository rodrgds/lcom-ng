#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

/**
 * @brief Checks if a scancode is a make code.
 * 
 * @param scancode Scancode to check.
 * 
 * @return true if the scancode is a make code, false otherwise.
 
*/
bool is_make_scancode(uint8_t scancode);

/**
 * @brief Creates an array with the scancode.
 * 
 * @param size Size of the array.
 * @param scancode Scancode to add to the array.
 * @param bytes Array to add the scancode to.
 * 
 * @return 0 upon success, one otherwise.
*/
int create_scancode_array(uint8_t size, uint8_t scancode, uint8_t bytes[]);

/**
 * @brief Reads a byte from a port.
 * 
 * @param port Port to read from.
 * @param value Address of memory to be initialized with the value read.
 * 
 * @return 0 upon success, one otherwise.
*/
int(util_sys_inb)(int port, uint8_t *value);

/**
 * @brief Gets the number of sys_inb calls.
 * 
 * @return The number of sys_inb calls.
*/
int get_no_sysinb();

/**
 * @brief Reads the value in the port and stores it in the value.
 * 
 * @param port Port to read from.
 * @param value Address of memory to be initialized with the value read.
 * 
 * @return 0 upon success, one otherwise.
*/

int(util_sys_inb)(int port, uint8_t *value);

/**
 * @brief Gets the most significant byte of a value.
 * 
 * @param val Value to get the most significant byte of.
 * @param msb Address of memory to be initialized with the most significant byte.
 * 
 * @return 0 upon success, one otherwise.
*/

int(util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Gets the least significant byte of a value.
 * 
 * @param val Value to get the least significant byte of.
 * @param lsb Address of memory to be initialized with the least significant byte.
 * 
 * @return 0 upon success, one otherwise.
*/
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

void (delay_milli_seconds)(unsigned milli_seconds);

uint8_t (bcd_binary)(uint8_t value);

#endif
