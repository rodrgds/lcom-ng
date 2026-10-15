/**
 * @file guess_mode_view.h
 * @brief Interface for rendering the guess mode game view and related UI elements.
 */

#ifndef _LCOM_GUESS_MODE_VIEW_H_
#define _LCOM_GUESS_MODE_VIEW_H_

#include <lcom/lcf.h>
#include "view/view.h"

/**
 * @brief Draws the frame for Guess Mode.
 * 
 * Renders the screen for Guess Mode. It displays a countdown during the initial 3 seconds,
 * followed by instructions for the player(s) to stop a timer after guessing the target time.
 * 
 * @param elapsed_seconds Number of seconds that have passed since the mode started.
 * @param target_seconds Number of seconds the player(s) should try to guess.
 */
void draw_guess_mode_frame(int elapsed_seconds, int target_seconds);

#endif /* _LCOM_GUESS_MODE_VIEW_H_ */
