#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

#include <lcom/lcf.h>

#include "devices/mouse.h"
#include "model/crosshair.h"
#include "view/crosshair_view.h"

/**
 * @brief Initializes the crosshair's position on the screen.
 * 
 * Sets the crosshair to a default starting position.
 */
void (crosshair_controller_init)();

/**
 * @brief Updates the crosshair based on mouse input.
 * 
 * Moves the crosshair using mouse deltas and handles left button clicks
 * to detect hits on targets and update scores accordingly.
 * 
 * @param pp Pointer to a parsed mouse packet.
 */
void (crosshair_controller_update)(struct packet *pp);

/**
 * @brief Draws the crosshair on the screen.
 * 
 * Uses the current position and size of the crosshair.
 */
void (crosshair_controller_draw)();

/**
 * @brief Resets the crosshair to its default initial position.
 */
void (crosshair_controller_reset)();

#endif

