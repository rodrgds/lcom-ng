#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_
#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "controller/KBC/KBC.h"

/** @defgroup mouse mouse
 * @{
 *
 * Functions for using the mouse
 */

/**

* @brief Mouse information structure.
* This structure holds information related to mouse events and cursor position.
*/

typedef struct {
    uint8_t right_click;
    uint8_t left_click;
    uint8_t middle_click;
    int16_t x;
    int16_t y;
} MouseInfo;  

/**
 * @brief Subscribes mouse interrupts
 * @return 0 if it is successful, not 0 if otherwise
 */

int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 * @return 0 if it is successful, not 0 if otherwise
 */

int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler
 */

void (mouse_ih)();

/**
 * @brief Reads the mouse output buffer
 */

void mouse_sync_bytes();

/**
 * @brief Converts the mouse bytes to a mouse struct
 */

void (mouse_bytes_to_packet)();

/**
 * @brief Writes a command to the mouse and waits for acknowledgement.
 * @param command The command to be sent to the mouse.
 * @return 0 if the command is successfully sent and acknowledged by the mouse, 1 otherwise.
 */

int (mouse_write)(uint8_t command);

/**
 * @brief Reads the mouse output buffer
 * @return 0 if the data is successfully read, 1 otherwise.
 */

int(mouse_read_data)();

int getX();

int getY();



#endif
