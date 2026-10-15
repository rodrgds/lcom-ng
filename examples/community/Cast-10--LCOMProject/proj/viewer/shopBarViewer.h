#pragma once

#include <stdlib.h>

#include "../model/state/shopBarState.h"

#include "../assets/pause/buy_buttons.h"

#include "ButtonViewer.h"

#include "video.h"

/**
 * @brief Renderer for a single upgradeable bar in the shop.
 */
typedef struct {
    uint32_t border_color;       /**< Colour of the bar border. */
    uint32_t empty_color;        /**< Colour of the unfilled portion of the bar. */
    uint32_t fill_color;         /**< Colour of the filled (purchased) portion. */
    uint32_t buy_hover_color;    /**< Buy button colour when the cursor is over it. */
    uint32_t buy_disabled_color; /**< Buy button colour when the bar is at max level. */
    ButtonViewer *buttonViewer;  /**< Viewer used to render the buy button. */
} ShopBarViewer;

/**
 * @brief Allocates and initialises a ShopBarViewer.
 * @return Pointer to the new ShopBarViewer, or NULL on failure.
 */
ShopBarViewer *shopBarViewerCreate();

/**
 * @brief Frees a ShopBarViewer.
 * @param viewer ShopBarViewer to destroy.
 */
void shopBarViewerDestroy(ShopBarViewer *viewer);

/**
 * @brief Draws the upgrade bar and its buy button.
 * Filled segments reflect the current upgrade level; the buy button is greyed out at max level.
 * @param viewer ShopBarViewer (currently stateless).
 * @param bar ShopBarState providing position, level, and buy-button state.
 */
void shopBarViewerDraw(ShopBarViewer *viewer, ShopBarState *bar);
