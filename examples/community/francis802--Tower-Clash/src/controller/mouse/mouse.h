/**
 * @file mouse.h
 * 
 * @brief This file contains the definition of the MouseInfo struct and the
 *       function to configure and interact with the mouse.
*/

#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#define MOUSE_MASK BIT(2)

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"
#include "controller/video/graphics.h"

/**
 * @brief This struct contains the information about the mouse to ease
 * the implementation of some of the game's features.
*/
typedef struct
{
    uint8_t right_click;
    uint8_t left_click;
    int16_t x;
    int16_t y;
} MouseInfo;

/**
 * @brief This function subscribes mouse interrupts, using the sys_irqsetpolicy() function.
*/
int (mouse_subscribe_interrupts)();

/**
 * @brief This function unsubscribes mouse interrupts, using the sys_irqrmpolicy() function
 * and the hook_id_mouse variable, fetched from the mouse_subscribe_interrupts() function.
*/
int (mouse_unsubscribe_interrupts)();

/**
 * @brief This function handles mouse interrupts, using the read_KBC_output() function to
 * get the current byte information.
*/
void (mouse_ih)();

/**
 * @brief Synchronizes mouse bytes
 * 
 * This function synchronizes the mouse bytes, using the mouse_bytes[3] array and the
 * the current_byte variable, fetched with the mouse_ih() function.
*/
void (mouse_sync_bytes)();

/**
 * @brief This function updates the mouse information, stored in the mouse_info struct, using
 * the information stored in the mouse_bytes[3] array.
*/
void (mouse_sync_info)();

/**
 * @brief This function writes a command to the mouse, using the write_KBC_command() function.
 * It tries to write the command a programable number of times, defined by the MAX_ATTEMPS
 * macro, until it receives an ACK response from the mouse.
 * 
 * @param command Command to be written
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (mouse_write)(uint8_t command);

#endif
