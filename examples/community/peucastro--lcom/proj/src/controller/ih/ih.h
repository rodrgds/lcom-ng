#ifndef __PROJ_IH_H
#define __PROJ_IH_H

#include "controller/ev/ev.h"
#include "controller/input/input.h"
#include "controller/kbc/kbd.h"
#include "controller/kbc/mouse.h"
#include "controller/rtc/rtc.h"
#include "controller/timer/timer.h"
#include "model/game/game.h"
#include "view/view.h"

/** @defgroup ih ih
 * @{
 *
 * Functions for using the interrupt handlers
 */

/**
 * @brief Subscribes for all the devices interrups.
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int(subscribe_interrupts)(void);

/**
 * @brief Unsubscribes for all the devices interrups.
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int(unsubscribe_interrupts)(void);

/**
 * @brief Handles timer interrupts
 *
 * Calls the timer interrupt handler and processes any related events.
 *
 * @param game Pointer to the game instance to be updated
 */
void(timer_handler)(Game *game);

/**
 * @brief Handles keyboard interrupts
 *
 * Reads scan codes from the keyboard controller and processes keyboard events.
 *
 * @param game Pointer to the game instance to be updated
 */
void(kbd_handler)(Game *game);

/**
 * @brief Handles mouse interrupts
 *
 * Processes mouse data packets and handles mouse-related events.
 *
 * @param game Pointer to the game instance to be updated
 */
void(mouse_handler)(Game *game);

/**
 * @brief Handles RTC interrupts.
 *
 * Calls the respective event handler
 *
 * @param game Pointer to the game instance to be updated
 */
void(rtc_handler)(Game *game);

/**
 * @brief Processes device interrupts based on the interrupt bitmask.
 *
 * This function checks the interrupt bitmask to determine which device
 * generated an interrupt and calls the appropriate handler (timer, keyboard, or mouse).
 *
 * @param irq_mask Bitmask containing the pending interrupt flags
 * @param game Pointer to the game instance to be updated
 */
void(process_interrupts)(uint32_t irq_mask, Game *game);

/**@}*/

#endif /* __PROJ_IH_H */
