#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "../model/state/ShopState.h"

#include "shopBarViewer.h"

/**
 * @brief Renderer for the upgrade shop screen.
 */
typedef struct {
    ShopBarViewer *barViewer; /**< Shared viewer used to draw each upgrade bar. */
} ShopViewer;

/**
 * @brief Allocates and initialises a ShopViewer.
 * @return Pointer to the new ShopViewer, or NULL on failure.
 */
ShopViewer *shopViewerCreate();

/**
 * @brief Frees a ShopViewer and its bar viewer.
 * @param viewer ShopViewer to destroy.
 */
void shopViewerDestroy(ShopViewer *viewer);

/**
 * @brief Draws the shop UI for both players, including all upgrade bars and buy buttons.
 * @param viewer ShopViewer supplying the bar renderer.
 * @param shop ShopState containing the current upgrade levels and button states.
 */
void shopViewerDraw(ShopViewer *viewer, ShopState *shop, int p1_points, int p2_points);
