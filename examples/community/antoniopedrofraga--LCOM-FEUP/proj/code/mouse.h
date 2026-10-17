#ifndef __TEST4_H
#define __TEST4_H

/** @defgroup mouse mouse
 * @{
 *
 * Functions for using the mouse code
 */

/**
 * @brief Subscribes mouse
 */
int mouse_subscribe_int();

/**
 * @brief Unsubscribes mouse
 */
int mouse_unsubscribe_int();

/**
 * @brief Gets mouse packets
 * @return returns -1 in case of an error or returns mouse data
 */
unsigned short get_packets();
/**
 * @brief Activates mouse
 */
int activate_mouse();

/**
 * @brief Deactivates mouse
 */
int deactivate_mouse();

#endif
