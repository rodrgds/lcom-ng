#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_


/** @defgroup keyboard keyboard
 * @{
 *
 * Functions to manage keyboard abstraction (getting interrupts and processing input)
 */

#include "ev_queue.h"

int kbd_get_key(unsigned short stopCode);

/**
 * @brief Subscribe kbd interrupts
 *
 * @return hookID on success, -1 otherwise
 */
int kbd_subscribe_int();

/**
 * @brief Unsubscribe kbd interrupts
 *
 * @return 0
 */
int kbd_unsubscribe_int();


int kbd_int_handler(unsigned short);

/**
 * @brief Returns the scan code of a pressed key
 *
 * @return scanCode or 0 if none
 */
unsigned long kbd_get_pressed_key();

void processOutputCode(unsigned short scancode);
void processDualOutputCode(unsigned short firstByte, unsigned short scancode);

/**
 * @brief Genereate an event associated with the scan code given
 *
 * @param events - event queue
 * @param scancode - scancode of the key pressed
 * @return none
 */
void keyboardHandler(ev_queue_st* events, unsigned long scancode);

#endif /* __KEYBOARD_H */

/** @} end of keyboard*/
