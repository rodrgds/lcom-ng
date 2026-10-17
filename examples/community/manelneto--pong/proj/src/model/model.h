#ifndef _MODEL_H_
#define _MODEL_H_

/** @defgroup model model
 * @{
 *
 * Functions related to the model
 */

/**
 * @brief Enum for the states
 */
typedef enum {
  MENU,   /*!< menu state */
  LEVELS, /*!< levels state */
  GAME,   /*!< game state */
  END     /*!< end state */
} State;

/**
 * @brief Starts pong
 *
 * Starts the menu.
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int start_pong();

/**
 * @brief Handles an interrupt from the timer
 *
 * Swaps the buffers, handles the interrupt according to the current state and draws the new frame.
 */
void timer_interrupt_handler();

/**
 * @brief Handles an interrupt from the keyboard
 *
 * Handles the interrupt according to the current state, restores/resets the keyboard scancode and draws the new frame.
 */
void keyboard_interrupt_handler();

/**
 * @brief Handles an interrupt from the mouse
 *
 * Handles the interrupt according to the current state, restores/resets the mouse packet and draws the new frame.
 */
void mouse_interrupt_handler();

/**
 * @brief Ends pong
 *
 * Ends the game, the levels and the menu.
 */
void end_pong();

/**@}*/

#endif /* _MODEL_H_ */
