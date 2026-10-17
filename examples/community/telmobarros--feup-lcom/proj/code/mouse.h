/**
 * This module contains all functions to manage mouse
 * Jorge Vale: 50%
 * Telmo Barros: 50%
 * Relative Weight: 4%
 */
#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdint.h>

/** @defgroup Mouse Mouse
 * @{
 * Functions to manage mouse
 */

/** @name Mouse_t structure */
/** @{
 *
 * Mouse structure
 */
typedef struct{
	unsigned int x, y;	// mouse position
	unsigned int left_btn ,middle_btn, right_btn;	// flags to store three buttons state of press or release
	int deltax;	// mouse movement in x axys
	int deltay;	// mouse movement in y axys
	unsigned int width,height;	// mouse sprite dimensions
	uint16_t * map;	//mouse sprite image
} Mouse_t;
/** @} end of Mouse_t structure */

/**
 * @brief Subscribes the mouse interrupts
 *
 * @return Return mouse ID to verify the interrupts and -1 upon failure
 */
int mouse_subscribe_int();

/**
 * @brief Unsubscribes the mouse interrupts
 *
 * @return Return 0 upon success and 1 upon failure
 */
int mouse_unsubscribe_int();

/**
 * @brief Reads from the output buffer of the mouse
 *
 * @return Return data of buffer and -1 upon failure
 */
int mouse_output();

#endif /* __MOUSE_H */
