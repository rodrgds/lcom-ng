/**
 * @file react_mode.h
 * @brief Interface for react mode game logic and state.
 */

#ifndef _LCOM_REACT_MODE_H_
#define _LCOM_REACT_MODE_H_

#include <lcom/lcf.h>
#include "controllers/KBC/mouse.h"
#include "model/modes.h"
#include "model/state.h"
#include "view/view.h"
#include "utils/random.h"

/**
 * @brief Initializes the Reaction mode.
 * 
 * Resets relevant variables and sets up the random reaction timer.
 */
void react_mode_init(void);

/**
 * @brief Keyboard interrupt handler for Reaction mode.
 * 
 * Checks if Player 1 reacts by pressing SPACE.
 */
void react_mode_keyboard_ih(void);

/**
 * @brief Mouse interrupt handler for Reaction mode.
 * 
 * Checks if Player 2 reacts by clicking the left mouse button.
 */
void react_mode_mouse_ih(void);

/**
 * @brief Timer interrupt handler for Reaction mode.
 * 
 * Updates elapsed time and checks game-over conditions.
 */
void react_mode_timer_ih(void);

#endif /* _LCOM_REACT_MODE_H_ */
