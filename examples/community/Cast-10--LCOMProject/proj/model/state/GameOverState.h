#pragma once

#include <stdlib.h>
#include "../Button.h"

/**
 * @brief State for the game-over screen.
 */
typedef struct {
    ButtonState *restartButton; /**< Button that restarts the match. */
    ButtonState *quitButton; /**< Button that exits the application. */
} GameOverState;

/**
 * @brief Allocates and initialises a GameOverState with positioned buttons.
 * @return Pointer to the new GameOverState, or NULL on allocation failure.
 */
GameOverState *gameOverStateCreate();

/**
 * @brief Frees a GameOverState and its buttons.
 * @param state GameOverState to destroy.
 */
void gameOverStateDestroy(GameOverState *state);
