/** 
 * @file kbc.h
 * @brief Header file for @ref kbc.c
 * 
 * This file declares all the functions to interact with the kbc.
 */


#ifndef _KBC_H_
#define _KBC_H_

#include <lcom/lcf.h>
#include "i8042.h"


/**
 * @brief Writes a command to the kbc
 * 
 * @param command The command to write
 * @return 0 in case of success, 1 otherwise
 */
int (kbc_write_command)(uint8_t command);

/**
 * @brief Writes a command argument to the kbc
 * 
 * @param arg The argument to write
 * @return 0 in case of success, 1 otherwise
 */
int (kbc_write_command_arg)(uint8_t arg);

/**
 * @brief Reads a command from the kbc
 * 
 * @param command The pointer where the read command will be stored
 * @return 0 in case of success, 1 otherwise
 */
int (kbc_read_command)(uint8_t* command);

/**
 * @brief Validates the data on the kbc's output buffer
 * 
 * @param aux True if the data needs to be mouse data, false is keyboard data
 * @return true if valid, false otherwise
 */
bool (kbc_valid_output)(bool aux);

/**
 * @brief Discards the data on the kbc's output buffer
 * 
 * Calls util_sys_inb() and does nothing with the data obtained.
 */
void (kbc_discard_output)();

#endif /* _KBC_H */
