#pragma once

#include <math.h>

/**
 * @brief 2D floating-point position.
 */
typedef struct {
    float x; /**< Horizontal coordinate. */
    float y; /**< Vertical coordinate. */
} Position;

/**
 * @brief Moves a position by a direction vector at the given speed.
 * Diagonal movement is normalized so its magnitude equals axis-aligned movement.
 * @param pos Position to update.
 * @param dx Horizontal direction (-1, 0, or 1).
 * @param dy Vertical direction (-1, 0, or 1).
 * @param speed Movement speed in pixels per tick.
 */
void position_move(Position *pos, int dx, int dy, float speed);

/**
 * @brief Returns the x coordinate rounded to the nearest integer.
 * @param pos Source position.
 * @return Rounded x coordinate in pixels.
 */
int position_px(Position pos);

/**
 * @brief Returns the y coordinate rounded to the nearest integer.
 * @param pos Source position.
 * @return Rounded y coordinate in pixels.
 */
int position_py(Position pos);
