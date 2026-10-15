#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include "KBC.h"
/**
 * @brief Subscribes to mouse interrupts.
 *
 * @param bit_no Pointer to store the bit number of the mouse interrupt mask.
 * @return 0 on success or 1 if pointer is invalid.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from mouse interrupts.
 * 
 * @return 0 on success.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Writes a command directly to mouse.
 * 
 * @param command byte to send to mouse.
 * @return 0 on success or 1 if it wasn't successeful on any of the attempts.
 */
int (mouse_write_command)(uint8_t command);

/**
 * @brief Mouse interrupt handler.
 */
void (mouse_ih)();

/**
 * @brief Updates position of mouse within screen boundaries.
 * 
 * @param x int with current x coordinate of mouse
 * @param y int with current y coordinate of mouse
 * @param max_x int with maximum value of x allowed within the screen.
 * @param max_y int with maximum value of y allowed within the screen.
 * @param min_x int with minimum value of x allowed within the screen.
 * @param min_y int with minimum value of y allowed within the screen.
 * @param mouse_packet Mouse event packet containing mouse button states.
 * @return 0 on success.
 */
int (calculate_placement)(int* x, int* y, int max_x, int max_y, int min_x, int min_y, struct packet mouse_packet);

#endif /*_LCOM_MOUSE_H_*/
