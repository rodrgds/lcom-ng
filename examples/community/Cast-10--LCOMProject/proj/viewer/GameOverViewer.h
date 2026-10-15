#pragma once

#include <stdlib.h>

#include "ButtonViewer.h"

#include "../assets/game_over/retry_button_xpm.h"
#include "../assets/game_over/retry_button_hover_xpm.h"
#include "../assets/game_over/quit_button_xpm.h"
#include "../assets/game_over/quit_button_hover_xpm.h"

#include "../model/state/State.h"

/**
 * @brief Renderer for the game-over screen.
 */
typedef struct {
    ButtonViewer *restartButtonViewer; /**< Viewer for the restart button. */
    ButtonViewer *quitButtonViewer;    /**< Viewer for the quit button. */
} GameOverViewer;

/**
 * @brief Allocates and initialises a GameOverViewer, loading button sprites.
 * @return Pointer to the new GameOverViewer, or NULL on failure.
 */
GameOverViewer *gameOverViewerCreate();

/**
 * @brief Frees a GameOverViewer and its button viewers.
 * @param viewer GameOverViewer to destroy.
 */
void gameOverViewerDestroy(GameOverViewer *viewer);

/**
 * @brief Draws the game-over screen including the winner text and action buttons.
 * @param viewer GameOverViewer supplying the button sprites.
 * @param state Application state providing the winner and button hover/click state.
 */
void gameOverViewerDraw(GameOverViewer *viewer, State *state);
