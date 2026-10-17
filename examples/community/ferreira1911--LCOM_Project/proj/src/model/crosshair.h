#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include <lcom/lcf.h>

/**
 * @struct Crosshair
 * @brief Represents the crosshair cursor in the game.
 *
 * Stores the position and size of the crosshair.
 */
typedef struct {
    int16_t x, y;
    uint16_t width, height;
} Crosshair;

/**
 * @brief Initializes the global crosshair at the given position.
 *
 * Sets the initial x and y coordinates and default size of the crosshair.
 *
 * @param x Initial horizontal position on screen
 * @param y Initial vertical position on screen
 */
void (crosshair_init)(int16_t x, int16_t y);

/**
 * @brief Updates the position of the global crosshair by the given deltas.
 *
 * Moves the crosshair and clamps it within the screen boundaries.
 *
 * @param delta_x Amount to move horizontally
 * @param delta_y Amount to move vertically
 */
void (crosshair_update_position)(int16_t delta_x, int16_t delta_y);

#endif

