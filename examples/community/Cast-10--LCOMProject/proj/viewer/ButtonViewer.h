#pragma once

#include <lcom/lcf.h>
#include <stdlib.h>

#include "sprite/Sprite.h"

#include "../model/Button.h"

/**
 * @brief Renderer for an interactive button with a normal and a hover sprite.
 */
typedef struct {
    Sprite *normal; /**< Sprite drawn when the button is not hovered. */
    Sprite *hover;  /**< Sprite drawn when the cursor is over the button. */
} ButtonViewer;

/**
 * @brief Allocates a ButtonViewer and loads both sprites from XPM maps.
 * @param normal_xpm XPM map for the default (non-hovered) sprite.
 * @param hover_xpm XPM map for the hovered sprite.
 * @return Pointer to the new ButtonViewer, or NULL on failure.
 */
ButtonViewer *buttonViewerCreate(xpm_map_t normal_xpm, xpm_map_t hover_xpm);

/**
 * @brief Frees a ButtonViewer and its sprites.
 * @param viewer ButtonViewer to destroy.
 */
void buttonViewerDestroy(ButtonViewer *viewer);

/**
 * @brief Draws the button at the given position, updating its state from the sprite dimensions.
 * Uses the hover sprite when ButtonState::hovered is true, otherwise the normal sprite.
 * @param viewer ButtonViewer supplying the sprites.
 * @param button ButtonState to update and render.
 * @param x Screen X coordinate of the button's top-left corner.
 * @param y Screen Y coordinate of the button's top-left corner.
 */
void buttonViewerDraw(ButtonViewer *viewer, ButtonState *button, int x, int y);
