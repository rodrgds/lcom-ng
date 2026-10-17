/**
 * @file types.h
 * @brief Type definitions for game entities, positions, and speeds.
 */

#ifndef VEC_H
#define VEC_H

#include "constants.h"

/**
 * @brief Structure representing a 2D speed vector (floating point).
 */
typedef struct
{
    float x; /**< X component of speed. */
    float y; /**< Y component of speed. */
} Vec_speed;

/**
 * @brief Structure representing a 2D position vector (integer).
 */
typedef struct
{
    int x; /**< X coordinate. */
    int y; /**< Y coordinate. */
} Vec_pos;

/**
 * @brief Structure representing a generic game entity (e.g., enemy).
 */
typedef struct
{
    Vec_pos pos;      /**< Position of the entity. */
    Vec_speed speed;  /**< Speed of the entity. */
    bool active;      /**< Whether the entity is active. */
    int hp;           /**< Hit points (health) of the entity. */
} Entity;

/**
 * @brief Structure representing a bullet in the game.
 */
typedef struct {
    Vec_pos pos;      /**< Position of the bullet. */
    Vec_speed speed;  /**< Speed of the bullet. */
    bool active;      /**< Whether the bullet is active. */
    int damage;       /**< Damage dealt by the bullet. */
} Bullet;

/**
 * @brief Structure representing the player.
 */
typedef struct {
    Vec_pos pos;      /**< Position of the player. */
    Vec_speed speed;  /**< Speed of the player. */
    int health;       /**< Health of the player. */
    int score;        /**< Score of the player. */
} Player;

/**
 * @brief Global player instance.
 */
extern Player player;

/**
 * @brief Array of enemy entities.
 */
extern Entity enemies[MAX_ENEMIES];

/**
 * @brief Array of bullets.
 */
extern Bullet bullets[MAX_BULLETS];

#endif
