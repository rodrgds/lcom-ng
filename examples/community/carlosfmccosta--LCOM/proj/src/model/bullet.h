/**
 * @file bullet.h
 * @brief Interface for bullet entity management.
 *
 * This header provides the structure and functions for creating, updating, drawing,
 * and destroying bullets, as well as checking their bounds in the game.
 */
#ifndef __BULLET_H
#define __BULLET_H

#include <stdint.h>
#include <stdbool.h>
#include "sprite.h"     
#include "position.h"
#include "assets/bullet_asset.h"

/** @def Bullet's speed if they're shot from a player*/
#define BULLET_SPEED_PLAYER 5 

/** @def Bullet's speed if they're shot from a enemy*/
#define BULLET_SPEED_ENEMY 2

/**
 * @brief Structure representing a bullet entity
 */
typedef struct {
    sprite_t *sprite;            /**< Sprite for bullet visualization */
    int16_t vx;                  /**< Velocity in x direction */
    int16_t vy;                  /**< Velocity in y direction */
    bool active;                 /**< Whether the bullet is active */
    bool is_from_player;         /**< Whether the bullet was fired by player (true) or enemy (false) */  
} bullet_t;

/**
 * @brief Initializes a new bullet.
 *
 * @param start_x Starting x-coordinate (typically player's center x).
 * @param start_y Starting y-coordinate (typically player's center y).
 * @param target_x Target x-coordinate (typically cursor's center x).
 * @param target_y Target y-coordinate (typically cursor's center y).
 * @return bullet_t* Pointer to the created bullet, or NULL on failure.
 */
bullet_t* bullet_init(int16_t start_x, int16_t start_y, int16_t target_x, int16_t target_y, bool is_from_pl);

/**
 * @brief Destroys a bullet and frees its resources.
 *
 * @param bullet Pointer to the bullet to destroy.
 */
void destroy_bullet(bullet_t *bullet);

/**
 * @brief Moves the bullet based on its velocity.
 *
 * @param bullet Pointer to the bullet to move.
 */
void bullet_move(bullet_t *bullet);

/**
 * @brief Draws the bullet on screen.
 *
 * @param bullet Pointer to the bullet to draw.
 * @return int 0 if successful, 1 on error (consistent with player_draw).
 */
int bullet_draw(const bullet_t *bullet);

/**
 * @brief Checks if the bullet is out of screen bounds and deactivates it if so.
 *
 * @param bullet Pointer to the bullet.
 * @param screen_width Width of the screen.
 * @param screen_height Height of the screen.
 */
void bullet_check_bounds_deactivate(bullet_t *bullet, unsigned int screen_width, unsigned int screen_height);

#endif //__BULLET_H
