#pragma once

#include "stdbool.h"

/**
 * @brief Axis-aligned bounding box used for collision detection.
 */
typedef struct {
    int x; /**< Left edge in pixels. */
    int y; /**< Top edge in pixels. */
    int w; /**< Width in pixels. */
    int h; /**< Height in pixels. */
} Box;

/**
 * @brief Tests whether two axis-aligned bounding boxes overlap.
 * @param a First bounding box.
 * @param b Second bounding box.
 * @return true if the boxes intersect, false otherwise.
 */
bool boxesOverlap(Box a, Box b);
