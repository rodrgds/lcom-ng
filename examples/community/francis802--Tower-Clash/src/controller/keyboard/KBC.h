/**
 * @file KBC.h
 * 
 * @brief This file contains the definition of the function to interact with the
 *      KBC.
*/

#ifndef _LCOM_KBC_KEY_H_
#define _LCOM_KBC_KEY_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @brief This function reads the KBC status, using the util_sys_inb() function.
 * 
 * @param status Pointer to the variable where the status will be stored
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (read_KBC_status)(uint8_t* status);

/**
 * @brief This function reads the KBC output, using the util_sys_inb() function.
 * It tries to read the output 10 times, with a 20ms delay between each try, 
 * in case of the KBC output buffer being full.
 * 
 * @param port Port to read from
 * @param output Pointer to the variable where the output will be stored
 * @param mouse Boolean value that indicates if the output is from the mouse
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief This function writes a command to the KBC, using the util_sys_inb() function.
 * It tries to write the command 10 times, with a 20ms delay between each try,
 * in case of the KBC input buffer being full.
 * 
 * @param port Port to write to
 * @param commandByte Command to write
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (write_KBC_command)(uint8_t port, uint8_t commandByte);

#endif
