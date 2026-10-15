#pragma once

#include "../model/Button.h"
#include "../input/MouseInterpreter.h"

/**
 * @brief Updates a button's hover and click state from the current mouse state.
 * Sets ButtonState::hovered when the cursor is inside the button bounds,
 * and ButtonState::clicked on the frame the primary button is pressed while hovered.
 * @param button ButtonState to update.
 * @param mouse Current mouse state providing cursor position and click events.
 */
void buttonControllerUpdate(ButtonState *button, MouseState *mouse);
