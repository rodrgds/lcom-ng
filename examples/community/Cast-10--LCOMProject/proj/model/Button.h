#pragma once

#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief State of an interactive button.
 */
typedef struct {
    int x; /**< Left edge in pixels. */
    int y; /**< Top edge in pixels. */
    int width; /**< Width in pixels. */
    int height; /**< Height in pixels. */
    bool hovered; /**< True while the cursor is over the button. */
    bool clicked; /**< True on the frame the button was clicked. */
} ButtonState;

/**
 * @brief Allocates and initialises a ButtonState with zeroed fields.
 * @return Pointer to the new ButtonState, or NULL on allocation failure.
 */
ButtonState *buttonStateCreate();

/**
 * @brief Frees a ButtonState previously created with buttonStateCreate().
 * @param button Button to destroy.
 */
void buttonStateDestroy(ButtonState *button);

/**
 * @brief Sets the screen position of the button.
 * @param button Button to update.
 * @param x New left edge in pixels.
 * @param y New top edge in pixels.
 */
void buttonStateSetPosition(ButtonState *button, int x, int y);

/**
 * @brief Sets the dimensions of the button.
 * @param button Button to update.
 * @param width New width in pixels.
 * @param height New height in pixels.
 */
void buttonStateSetSize(ButtonState *button, int width, int height);

/**
 * @brief Tests whether a screen point lies within the button's bounds.
 * @param button Button to test.
 * @param x Screen x coordinate.
 * @param y Screen y coordinate.
 * @return true if (x, y) is inside the button rectangle.
 */
bool buttonStateContains(ButtonState *button, int x, int y);

/**
 * @brief Clears the one-frame click flag.
 * @param button Button to reset.
 */
void buttonStateResetClick(ButtonState *button);
