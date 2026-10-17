#ifndef __KB_H
#define __KB_H

/** @defgroup kb kb

 * @{
 *
 * Functions for using the kbd code
 */

/**
 * @brief Subscribes and enables keyboard interrupts
 */
int kb_subscribe_int();

/**
 * @brief Unsubscribes and disables keyboard interrupts
 */
int kb_unsubscribe_int();

/**
 * @brief Returns a key code when a key is pressed
 * @return Returns a key code unsigned long
 */
unsigned long get_kb();

#endif /* __KB_H */
