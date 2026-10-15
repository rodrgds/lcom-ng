/**
 * @file mouse.h
 * @brief Interface for mouse controller functions and interrupt handling.
 *
 * Provides functions to subscribe/unsubscribe mouse interrupts, send commands, synchronize packets, and check button states.
 */

#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include "KBC.h"
#include "i8042.h"


/**
 * @brief Mouse interrupt handler. Reads data from the mouse output buffer.
 *
 * This function should be called on each mouse interrupt to process the latest mouse byte.
 */
void(mouse_ih)();

/**
 * @brief Subscribes mouse interrupts.
 *
 * @param bit_no Pointer to variable where the hook id will be stored (for use with sys_irqsetpolicy).
 * @return 0 on success, non-zero otherwise.
 */
int mouse_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts.
 *
 * @return 0 on success, non-zero otherwise.
 */
int mouse_unsubscribe_int();

/**
 * @brief Writes a command byte to the mouse.
 *
 * @param command The command byte to send to the mouse.
 * @return 0 on success, non-zero otherwise.
 */
int mouse_write_command(uint8_t command);

/**
 * @brief Synchronizes the mouse packet bytes.
 *
 * This function ensures that the mouse packet bytes are properly aligned before further processing.
 */
void mouse_sync();

/**
 * @brief Generates a mouse packet from the latest bytes read.
 *
 * This function processes the 3 raw bytes from the mouse and fills the packet struct.
 */
void generate_packet();

/**
 * @brief Checks if the left mouse button is currently pressed.
 *
 * @return true if pressed, false otherwise.
 */
bool isLeftButtonPressed();

/**
 * @brief Checks if the right mouse button is currently pressed.
 *
 * @return true if pressed, false otherwise.
 */
bool isRightButtonPressed();
extern int cursor_x;
extern int cursor_y;


#endif
