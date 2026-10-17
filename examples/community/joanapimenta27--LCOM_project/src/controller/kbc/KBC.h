/**
 * @file KBC.h
 * @brief Header file for the Keyboard Controller (KBC) module.
 */

#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"

/**
 * @brief Reads the status of the keyboard controller.
 * 
 * @param status Pointer to a variable where the status will be stored.
 * @return Returns 0 on success and non-zero otherwise.
 */
int (read_status)(uint8_t* status);

/**
 * @brief Reads the output of the keyboard controller.
 * 
 * @param port The port to read from.
 * @param output Pointer to a variable where the output will be stored.
 * @param mouse Boolean indicating whether the data is expected to be from the mouse.
 * @return Returns 0 on success and non-zero otherwise.
 */
int (read_KBC_out)(uint8_t port, uint8_t *output, bool mouse);

/**
 * @brief Writes a command byte to the keyboard controller.
 * 
 * @param port The port to write to.
 * @param commandByte The command byte to write.
 * @return Returns 0 on success and non-zero otherwise.
 */
int (write_KBC)(uint8_t port, uint8_t commandByte);

#endif /* _LCOM_KBC_H_ */
