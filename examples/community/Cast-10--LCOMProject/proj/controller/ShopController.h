#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "../model/state/GameState.h"
#include "../model/state/ShopState.h"
#include "../input/MouseInterpreter.h"
#include "ShopBarController.h"

/**
 * @brief Controller for the upgrade shop screen.
 */
typedef struct {
    ShopBarController *barController; /**< Shared bar controller used for every upgrade bar. */
} ShopController;

/**
 * @brief Allocates and initialises a ShopController.
 * @return Pointer to the new ShopController, or NULL on failure.
 */
ShopController *shopControllerCreate();

/**
 * @brief Frees a ShopController and its bar controller.
 * @param controller ShopController to destroy.
 */
void shopControllerDestroy(ShopController *controller);

/**
 * @brief Processes mouse input for all upgrade bars of both players for one tick.
 * Delegates to shopBarControllerUpdate() for each bar.
 * @param controller ShopController providing the bar controller.
 * @param shop ShopState containing all upgrade bars.
 * @param mouse Current mouse state providing cursor position and click events.
 */
void shopControllerUpdate(ShopController *controller, ShopState *shop, MouseState *mouse, GameState *game);
