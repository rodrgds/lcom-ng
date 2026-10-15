#ifndef HITBOX_H
#define HITBOX_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Axis-aligned rectangle used for game collision checks.
 */
typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
} Hitbox;

/**
 * @brief Creates a hitbox from its top-left corner and dimensions.
 *
 * @param x Top-left x coordinate.
 * @param y Top-left y coordinate.
 * @param width Hitbox width in pixels.
 * @param height Hitbox height in pixels.
 * @return Hitbox value with the given position and dimensions.
 */
Hitbox hitbox_create(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Updates the top-left position of an existing hitbox.
 *
 * @param hitbox Hitbox to move. NULL is accepted.
 * @param x New top-left x coordinate.
 * @param y New top-left y coordinate.
 */
void hitbox_set_position(Hitbox *hitbox, uint16_t x, uint16_t y);

/**
 * @brief Checks whether a hitbox has no collision area.
 *
 * @param hitbox Hitbox to test. NULL is treated as empty.
 * @return true if the hitbox is NULL or has zero width or height, false otherwise.
 */
bool hitbox_is_empty(const Hitbox *hitbox);

/**
 * @brief Checks whether a hitbox starts outside the given bounds.
 *
 * @param hitbox Hitbox to test.
 * @param width Bounds width in pixels.
 * @param height Bounds height in pixels.
 * @return true if the hitbox top-left corner is outside the bounds, false
 * otherwise or when hitbox is NULL.
 */
bool hitbox_is_outside_bounds(const Hitbox *hitbox, uint16_t width, uint16_t height);

/**
 * @brief Checks whether two hitboxes overlap.
 *
 * @param a First hitbox.
 * @param b Second hitbox.
 * @return true if the two hitboxes intersect, false otherwise.
 */
bool hitbox_intersects(const Hitbox *a, const Hitbox *b);

/**
 * @brief Checks whether a point is inside a hitbox.
 *
 * @param hitbox Hitbox to test.
 * @param x Point x coordinate.
 * @param y Point y coordinate.
 * @return true if the point is inside the hitbox, false otherwise.
 */
bool hitbox_contains_point(const Hitbox *hitbox, uint16_t x, uint16_t y);

#endif /* HITBOX_H */
