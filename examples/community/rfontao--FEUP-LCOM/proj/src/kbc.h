#ifndef _KBC_H_
#define _KBC_H_

#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

/** @defgroup KBC KBC
 * @{
 *
 * Functions of the programming of the KBC
 */

/**
 * @brief Subscribes the interruptions of the kbc
 * 
 * @param bit_no        Bit number of the IRQ line
 * @return              1 on failure
 */
int (kbc_subscribe_int)(int *bit_no);

/**
 * @brief Unsubscribes the interruptions of the kbc
 * 
 * @return              1 on failure
 */
int (kbc_unsubscribe_int)();

/**
 * @brief Checks if the outbuffer can be read and if it is reads its content
 * 
 * @return              1 on failure
 */
int (kbc_poll)();

/**
 * @brief Writes the cmd command to the kbc command register
 * 
 * @param cmd           Command to be issued
 * @return              1 on failure
 */
int (kbc_cmd)(uint8_t cmd);

/**
 * @brief Writes data to the kbc input buffer
 * 
 * @param data          Data to be written to the input buffer
 * @return              1 on failure
 */
int (kbc_write)(uint8_t data);

/**
 * @brief Reads the data on the outbuffer of the kbc and returns it to the data argument
 * 
 * @param data          Return value
 * @return              1 on failure
 */
int (kbc_read)(uint8_t *data);

/**
 * @brief Reenables the kbc interruptions of kbc after polling
 * 
 * @param command       ?
 * @return              1 on failure
 */
int (kbc_reenable_int)(uint8_t *command);

/**@}*/

#endif
