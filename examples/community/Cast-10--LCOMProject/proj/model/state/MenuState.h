#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "../Button.h"

/**
 * @brief State for the main menu screen.
 */
typedef struct {
    ButtonState *playButton; /**< The "Play" button. */
} MenuState;

/**
 * @brief Allocates and initialises a MenuState with a positioned play button.
 * @return Pointer to the new MenuState, or NULL on allocation failure.
 */
MenuState *menuStateCreate();

/**
 * @brief Frees a MenuState and its button.
 * @param menu MenuState to destroy.
 */
void menuStateDestroy(MenuState *menu);

/**
 * @brief Returns true if the play button was clicked this frame.
 * @param menu MenuState to query.
 * @return true when the game should start.
 */
bool menuStateShouldStart(MenuState *menu);

/**
 * @brief Clears the click state on the play button.
 * @param menu MenuState to reset.
 */
void menuStateReset(MenuState *menu);
