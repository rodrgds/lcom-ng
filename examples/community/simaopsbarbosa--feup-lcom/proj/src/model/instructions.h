/**
 * @file instructions.h
 * @brief Interface for instructions screen logic and state.
 */

#ifndef _LCOM_INSTRUCTIONS_H_
#define _LCOM_INSTRUCTIONS_H_

#include "controllers/KBC/keyboard.h"
#include "controllers/KBC/mouse.h"
#include "controllers/video/video.h"
#include "view/instructions_view.h"
#include "model/state.h"
#include "model/modes.h"

/**
 * @brief Initializes the instructions screen.
 * 
 * Draws the initial frame of the instructions view.
 */
void instructions_init();

/**
 * @brief Keyboard interrupt handler for the instructions screen.
 * 
 * Allows returning to the previous game mode by pressing ENTER.
 */
void keyboard_instructions();

/**
 * @brief Timer interrupt handler for the instructions screen.
 * 
 * Redraws the instructions view frame.
 */
void instructions_timer_ih();

/**
 * @brief Mouse interrupt handler for the instructions screen.
 * 
 * Checks for hover and click on the 'go back' button to return to the previous screen.
 */
void mouse_instructions();

#endif /* _LCOM_INSTRUCTIONS_H_ */
