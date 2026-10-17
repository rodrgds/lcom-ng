#pragma once


#include <lcom/lcf.h>
#include <stdint.h>


/**
 * @brief Issues a command to the keyboard controller (KBC).
 *
 * This function sends a command to the KBC and retrieves an optional return argument.
 * 
 * @param command The command byte to be sent to the KBC.
 * @param ret_arg Pointer to a variable where the returned argument will be stored (if any).
 * @return 0 on success, one otherwise.
 */
int(kbc_issue_command)(uint8_t command, uint8_t *ret_arg); 
/**
 * @brief Inserts a command into the keyboard controller (KBC).
 *
 * This function sends a command to the KBC without expecting any return argument.
 * 
 * @param command The command byte to be sent to the KBC.
 * @return 0 on success, one otherwise.
 */
int(kbc_insert_command)(uint8_t command);

/**
 * @brief Writes arguments to the KBC.
 *
 * This function sends a series of arguments to the KBC.
 * 
 * @param args Pointer to an array of arguments to be sent.
 * @return 0 on success, one otherwise.
 */
int(kbc_write_args)(uint8_t *args);

/**
 * @brief Reads data from the output buffer.
 *
 * This function retrieves data from the KBC's output buffer.
 * 
 * @param data Pointer to a variable where the retrieved data will be stored.
 * @return 0 on success, one otherwise.
 */
int read_outputBuffer(uint8_t *data); 
