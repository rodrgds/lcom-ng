#pragma once

#include "ButtonController.h"

#include "../model/state/MenuState.h"

#include "../input/KeyboardInterpreter.h"
#include "../input/MouseInterpreter.h"

/**
 * @brief Handles menu input for a single frame.
 * Triggers a game start when the play button is clicked or the player-1 up key is pressed.
 * @param menu MenuState containing the play button to update.
 * @param kbd Current keyboard state.
 * @param mouse Current mouse state.
 */
void menuControllerUpdate(MenuState *menu, KeyboardState *kbd, MouseState *mouse);
