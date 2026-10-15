#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "../model/champion/Champion.h"
#include "../model/state/shopBarState.h"

/**
 * @brief Controller for a single upgradeable shop bar (currently stateless).
 */
typedef struct {
    ButtonState *buyButton; /**< Buy button state owned by this controller. */
} ShopBarController;

/**
 * @brief Allocates and initialises a ShopBarController.
 * @return Pointer to the new ShopBarController, or NULL on failure.
 */
ShopBarController *shopBarControllerCreate();

/**
 * @brief Frees a ShopBarController.
 * @param controller ShopBarController to destroy.
 */
void shopBarControllerDestroy(ShopBarController *controller);

/**
 * @brief Updates the buy button hover state and purchases the next upgrade level on click.
 * Purchasing is skipped when the bar is already at SHOP_BAR_MAX_LEVEL.
 * @param controller ShopBarController (currently unused).
 * @param bar ShopBarState to update.
 * @param mouse_x Current cursor X coordinate in pixels.
 * @param mouse_y Current cursor Y coordinate in pixels.
 * @param mouse_pressed True if the primary mouse button was clicked this frame.
 * @return true if an upgrade was purchased this frame.
 */
bool shopBarControllerUpdate(ShopBarController *controller,
                             ShopBarState *bar,
                             int mouse_x,
                             int mouse_y,
                             bool mouse_pressed,
                             Champion *owner);
