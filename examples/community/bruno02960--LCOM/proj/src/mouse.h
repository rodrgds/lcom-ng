/**
 * @file mouse.h
 * @author Bruno Pinto and Vitor Magalhaes
 * @brief Contains mouse functions
 */

#ifndef _MOUSE_H_
#define _MOUSE_H_

/** @defgroup mouse mouse
 * @{
 *
 * Functions related to mouse
 */

/**
 * @brief Checks if the given byte is a mouse byte
 *
 * @return 0 if it is a keyboard byte, 1 if it is a mouse one
 */
int is_mouse_byte();

/**
 * @brief Subscribes and enables Mouse interruptss
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int mouse_subscribe(void);

/**
 * @brief Unsubscribes Mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe(void);

/**
 * @brief Reads packets from mouse
 *
 * @return Returns packet read
 */
char mouse_scan();

/**
 * @brief Sends a command to the mouse
 *
 * @param cmd command to send
 */
void mouse_command(unsigned long cmd);

/** @} end of mouse */

#endif
