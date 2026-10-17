#ifndef BULLET_H
#define BULLET_H

#include <lcom/lcf.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "assets/xpm/level/bullet.xpm"
#include "drivers/video/gpu.h"
#include "model/utils/sprite.h"

#define BULLET_SPEED 10 /**< Speed of the bullet in pixels per tick */
#define MAX_BULLETS 50  /**< Maximum number of bullets that can exist simultaneously */

extern uint16_t x_res; /**< X resolution of the screen */
extern uint16_t y_res; /**< Y resolution of the screen */

/**
 * @brief Represents a single bullet instance.
 *
 * The Bullet struct is opaque.
 */
typedef struct Bullet Bullet;

/**
 * @brief Creates a new bullet at the specified coordinates moving in the given direction.
 *
 * @param x      The initial x-coordinate (in pixels).
 * @param y      The initial y-coordinate (in pixels).
 * @param angle  The movement direction in radians.
 *
 * @return Pointer to the created Bullet on success, or NULL on failure.
 */
Bullet *create_bullet(int x, int y, double angle);

/**
 * @brief Frees resources used by a bullet and destroys it.
 *
 * @param bullet Pointer to the Bullet to destroy.
 * @return 0 on success, 1 on failure.
 */
int destroy_bullet(Bullet *bullet);

/**
 * @brief Gets the sprite associated with the bullet.
 *
 * @param bullet Pointer to the Bullet.
 * @return Pointer to the Sprite.
 */
Sprite *bullet_get_sprite(Bullet *bullet);

/**
 * @brief Gets the x-coordinate of the bullet.
 *
 * @param bullet Pointer to the Bullet.
 * @return The x-coordinate of the bullet.
 */
int16_t bullet_get_x(Bullet *bullet);

/**
 * @brief Sets the x-coordinate of the bullet.
 *
 * @param bullet Pointer to the Bullet.
 * @param x The x-coordinate of the bullet.
 */
void bullet_set_x(Bullet *bullet, int16_t x);

/**
 * @brief Gets the y-coordinate of the bullet.
 *
 * @param bullet Pointer to the Bullet.
 * @return The y-coordinate of the bullet.
 */
int16_t bullet_get_y(Bullet *bullet);

/**
 * @brief Sets the y-coordinate of the bullet.
 *
 * @param bullet Pointer to the Bullet.
 * @param y The y-coordinate of the bullet.
 */
void bullet_set_y(Bullet *bullet, int16_t y);

/**
 * @brief Gets the horizontal velocity component of the bullet.
 *
 * @param bullet Pointer to the Bullet.
 * @return Velocity in the x-direction (pixels per tick).
 */
double bullet_get_xspeed(Bullet *bullet);

/**
 * @brief Gets the vertical velocity component of the bullet.
 *
 * @param bullet Pointer to the Bullet.
 * @return Velocity in the y-direction (pixels per tick).
 */
double bullet_get_yspeed(Bullet *bullet);

/**
 * @brief Checks whether the bullet is currently active.
 *
 * @param bullet Pointer to the Bullet.
 * @return true if active, false otherwise.
 */
bool bullet_get_active(Bullet *bullet);

/**
 * @brief Sets the bullet's active status.
 *
 * @param bullet Pointer to the Bullet.
 * @param active New active status to set.
 */
void bullet_set_active(Bullet *bullet, bool active);

/**
 * @brief Draws the bullet on the provided frame buffer.
 *
 * @param bullet Pointer to the Bullet to draw.
 * @param frame_buffer Pointer to the frame buffer where the bullet will be rendered.
 * @return 0 on success, 1 on failure.
 */
int draw_bullet(Bullet *bullet, uint8_t *frame_buffer);

#endif /* BULLET_H */
