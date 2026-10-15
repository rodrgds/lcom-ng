#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

/**
 * @file Sprite.h
 * @brief Definition of the generic Sprite structure.
 *
 * A Sprite holds position, size, velocity, rotation, and bitmap data
 * for any moving or animated object. It is used by cube_sprite,
 * letter sprites, and progress bars throughout the game.
 */

/**
 * @struct Sprite
 * @brief Encapsulates all state needed to draw and animate a sprite.
 *
 * - x, y: Top-left coordinates on screen.
 * - width, height: Dimensions in pixels.
 * - xspeed, yspeed: Movement speed per frame in pixels.
 * - map: Pointer to pixel data (e.g., XPM raw buffer or custom map).
 * - angle: Current rotation angle in radians.
 * - rotation_speed: Rotation speed in radians per frame.
 * - index: General-purpose frame or state index for animations.
 * - cap: Maximum or “capacity” value associated with this sprite.
 */
typedef struct {
    int       x;               /**< Top-left X coordinate */
    int       y;               /**< Top-left Y coordinate */
    int       width;           /**< Width in pixels */
    int       height;          /**< Height in pixels */
    int       xspeed;          /**< Horizontal speed (pixels/frame) */
    int       yspeed;          /**< Vertical speed (pixels/frame) */
    uint8_t  *map;             /*< Pointer to raw bitmap data */
    double   angle;           /**< Current rotation angle (radians) */
    double   rotation_speed;  /**< Rotation speed (radians/frame) */
    int       index;           /**< Animation or state index */
    int       cap;             /**< Maximum capacity or limit */
} Sprite;

#endif


