/**
 * @file guess_mode.h
 * @brief Interface for guess mode game logic and state.
 */

#ifndef _LCOM_GUESS_MODE_H_
#define _LCOM_GUESS_MODE_H_

#include <lcom/lcf.h>
#include "model/modes.h"
#include "model/state.h"
#include "view/guess_mode_view.h"
#include "utils/random.h"

/**
 * @brief Initializes the Guess mode.
 * 
 * Resets relevant variables and sets up the random guess timer.
 */
void guess_mode_init();

/**
 * @brief Keyboard interrupt handler for Guess mode.
 * 
 * Detects Player 1's guess reaction using SPACE key.
 */
void guess_mode_keyboard_ih();

/**
 * @brief Mouse interrupt handler for Guess mode.
 * 
 * Detects Player 2's guess reaction using the left mouse button.
 */
void guess_mode_mouse_ih();

/**
 * @brief Timer interrupt handler for Guess mode.
 * 
 * Manages the countdown, reaction window, and game-over conditions.
 */
void guess_mode_timer_ih();

#endif /* _LCOM_GUESS_MODE_H_ */
