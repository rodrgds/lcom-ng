#ifndef _LCOM_STATES_H_
#define _LCOM_STATES_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "src/include/controllers/kbc/KBCInput.h"

/*!
 * @brief Initializes all game states.
 *
 * Allocates and initializes the necessary game states, including the menu, instructions, settings, and KBC states.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (makeStates)();

/*!
 * @brief Destroys all game states and frees their resources.
 *
 * Frees the memory allocated for all game states, including the menu, instructions, settings, and KBC states.
 */
void destroyStates();

/*!
 * @brief Advances the game to the next frame.
 *
 * Updates the game state based on the current game logic, draws the appropriate screen, and handles character movement.
 */
void (nextFrame)();

/*!
 * @brief Updates the KBC state based on the provided input.
 *
 * Processes the input from the keyboard or mouse and updates the game state accordingly.
 *
 * @param[in] input Pointer to the KBCInput object containing the input event.
 */
void (updateKBCState)(const KBCInput *input);

#endif
