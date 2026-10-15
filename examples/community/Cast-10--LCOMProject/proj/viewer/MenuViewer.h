#pragma once

#include <stdlib.h>

#include "ButtonViewer.h"

#include "../assets/menu/play_buttons_xpms.h"

#include "../model/state/MenuState.h"

/**
 * @brief Renderer for the main menu screen.
 */
typedef struct {
    ButtonViewer *playButtonViewer; /**< Viewer for the play button. */
} MenuViewer;

/**
 * @brief Allocates and initialises a MenuViewer, loading the play button sprites.
 * @return Pointer to the new MenuViewer, or NULL on failure.
 */
MenuViewer *menuViewerCreate();

/**
 * @brief Frees a MenuViewer and its button viewer.
 * @param viewer MenuViewer to destroy.
 */
void menuViewerDestroy(MenuViewer *viewer);

/**
 * @brief Draws the menu screen contents onto the back buffer.
 * @param viewer MenuViewer supplying the sprites.
 * @param menu MenuState containing button hover/click state.
 */
void menuViewerDraw(MenuViewer *viewer, MenuState *menu);
