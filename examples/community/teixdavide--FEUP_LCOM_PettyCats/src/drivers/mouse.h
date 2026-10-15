/**
 * @file mouse.h
 * @brief This file contains the necessary functions to process interrupts, parse
 * mouse packets and sending of commands to the KBC
 * @date 2024-06-02
 * 
 */

#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
/**
 * @brief Subscribes mouse interrupts using irq_reenable and irq_exclusive policy
 * 
 * @param bit_no hook id bit position of mouse
 * @return 0 in case of success, 1 otherwise
 * 
 */
int (mouse_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubcribes mouse interrupts
 * 
 * @return 0 upon success, 1 otherwise
 * 
 */
int (mouse_unsubscribe_int)();
/**
 * @brief Reads status register when an interrupt occurs and checks for timeout and parity errors, if none are found, it calls a function to read a byte to be placed in the mouse packet struct
 * 
 */
void (mouse_ih)();
/**
 * @brief Disables data reporting from the mouse
 * 
 * @return 0 upon success, 1 otherwise
 */
int kbc_restore_mouse();
/**
 * @brief First the 0xD4 command is issued to the status port of the kbc, then command cmd is sent to the kbc output buffer, then 10 attempts are made to read the KBC's output buffer to check if the command was acknowledged(ACK)
 * 
 * @param cmd command to be sent
 * @return 0 upon acknowledgement, 1 otherwise
 */
int issue_cmd_to_mouse(uint8_t cmd);
/**
 * @brief Reads the KBC's status byte to check if KBC'S output buffer is full, if so, its contents are placed in 'data' parameter and checked for errors and correspondance to mouse data
 * 
 * @param data 
 * @return int 
 */
int read_data_from_KBC_mouse(uint8_t *data);
/**
 * @brief Upon an interrupt this function is called, it reads a byte from the KBC and then places it in the mouse packet struct
 * @return 0 upon success, 1 otherwise 
 */
int read_byte_to_mouse_packet();
/**
 * @brief Buids the mouse packet
 * 
 */
void array_to_packet();
/**
 * @brief Reads the KBC status bit and checks if output buffer is empty, if so this function issues a command 'cmd' to port 'port'
 * 
 * @param port port taht will receive the command
 * @param cmd command to be sent
 * @return 0 upon success, 1 otherwise 
 */
int issue_cmd_to_KBC(uint8_t port, uint8_t cmd);

#endif
