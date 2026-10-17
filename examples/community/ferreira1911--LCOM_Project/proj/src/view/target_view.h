#ifndef TARGET_VIEW_H
#define TARGET_VIEW_H

#include <lcom/lcf.h>

#include "model/target.h"

/**
 * @brief Draws a target on screen if it is visible.
 *
 * Renders the XPM image of the given Target at its position.
 * Does nothing if the pointer is NULL or if the target is not visible.
 *
 * @param target Pointer to the Target to draw
 */
void (draw_target)(const Target *target);

#endif
