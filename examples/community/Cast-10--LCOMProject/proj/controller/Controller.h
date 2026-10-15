#pragma once

#include <lcom/lcf.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../model/state/State.h"
#include "../viewer/Viewer.h"

#include "../input/KeyboardInterpreter.h"
#include "../input/MouseInterpreter.h"

#include "GameController.h"
#include "MenuController.h"
#include "ShopController.h"

#include "vbe.h"

/**
 * @brief Top-level application controller that owns all game state, input, and rendering.
 */
typedef struct {
    State state; /**< Full application state (menu, game, shop, game-over). */
    KeyboardState kbd; /**< Aggregated keyboard input state. */
    MouseState mouse; /**< Aggregated mouse input state. */
    GameController *gameCtrl; /**< In-match game logic controller. */
    Viewer *viewer; /**< Top-level renderer. */
    ShopController *shopCtrl; /**< Shop upgrade controller. */
} Controller;

/**
 * @brief Allocates and initialises a Controller, creating all sub-systems.
 * @return Pointer to the new Controller, or NULL on failure.
 */
Controller *controllerCreate();

/**
 * @brief Frees a Controller and all sub-systems it owns.
 * @param ctrl Controller to destroy.
 */
void controllerDestroy(Controller *ctrl);

/**
 * @brief Runs one tick of top-level game logic, routing to the active state handler.
 * Handles global inputs (ESC to quit, P to pause/resume) and delegates to
 * menuControllerUpdate(), gameControllerUpdate(), shopControllerUpdate(), or the
 * game-over button logic. Checks win conditions (tower HP reaching zero) after
 * each in-match update.
 * @param ctrl Controller to update.
 * @param second_elapsed True if a full second has passed since the last update.
 */
void controllerUpdate(Controller *ctrl, bool second_elapsed);

/**
 * @brief Renders the current game state to the back buffer and swaps buffers.
 * @param ctrl Controller providing the viewer and current state.
 */
void controllerDraw(Controller *ctrl);
