/**
 * @file kbc.h
 * @brief KBC interface
 */


#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/**
 * @brief Reads the status byte from the KBC status register.
 * @param status Pointer to variable where the status byte will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int (read_KBC_status)(uint8_t* status);

/**
 * @brief Reads the output byte from the specified KBC port.
 * @param port Port to read from (0x60 for keyboard, 0x64 for mouse).
 * @param output Pointer to variable where the output byte will be stored.
 * @param mouse 1 if reading from mouse, 0 if from keyboard.
 * @return 0 on success, non-zero otherwise.
 */
int (read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Writes a command byte to the specified KBC port.
 * @param port Port to write to.
 * @param commandByte Command byte to write.
 * @return 0 on success, non-zero otherwise.
 */
int (write_KBC_command)(uint8_t port, uint8_t commandByte);

#endif
