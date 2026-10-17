#ifndef DRIVER_H
#define DRIVER_H

/** @defgroup driver driver
 * @{
 *
 * Groups all the functions related to the different drivers.
 */

#include <lcom/lcf.h>
#include "keyboard.h"
#include "mouse.h"
#include "rtc.h"
#include "video.h"
#include "timer.h"
#include "event.h"
#include "sprite.h"

/**
 * @brief Subscribe interrupts: keyboard, mouse, timer.
 * @return int Returns zero upon success.
 */
int (subscribe_interrupts)(void);

/**
 * @brief Unsubscribe interrupts: keyboard, mouse, timer.
 * @return int Returns zero upon success.
 */
int (unsubscribe_interrupts)(void);

/**
 * @brief Checks for interrupts. If any occurred, creates a 
 * Event struct with all the necessary information.
 * 
 * @return int Returns 1 if any interrupt occurred and 0 otherwise.
 */
int (interrupt_handler)(void);

/**
 * @brief Get the last event that occurred
 * 
 * @return Event Event struct (supports all different interrupts)
 */
Event get_event();

/**@}*/

#endif
