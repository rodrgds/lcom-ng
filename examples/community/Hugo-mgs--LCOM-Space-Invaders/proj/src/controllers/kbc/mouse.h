#ifndef _MOUSE_H
#define _MOUSE_H

#include <minix/syslib.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"

/**
 * Subscribes mouse interrupts.
 *
 * @param bit_no Pointer to variable where the bit number to be set in the mask will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);
 
/**
 * Unsubscribes mouse interrupts.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * Issues a command to the mouse.
 *
 * @param cmd The command byte to send to the mouse.
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_issue_cmd)(uint8_t cmd);
 
/**
 * Mouse interrupt handler. Reads and processes mouse data from the output buffer.
 */
void (mouse_ih)(void);

/**
 * Places a byte from the mouse into the appropriate buffer or variable.
 */
void (mouse_place_byte)(void);
 
/**
 * Creates a mouse packet from the bytes received from the mouse.
 */
void (mouse_create_packet)(void);

#endif
