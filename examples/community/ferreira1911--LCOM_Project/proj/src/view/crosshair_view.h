#ifndef CROSSHAIR_VIEW_H
#define CROSSHAIR_VIEW_H

#include <lcom/lcf.h>

#define CROSSHAIR_COLOR 0xFF0000 /**< @brief Color used to draw the crosshair (red) */

/**
 * @brief Draws the global crosshair on screen.
 *
 * Uses horizontal and vertical lines centered on the crosshair's position.
 * Relies on the global Crosshair model for coordinates and dimensions.
 */
void (draw_crosshair)();

#endif
