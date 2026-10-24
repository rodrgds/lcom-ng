/**
 * @file asteroid.h
 * @brief Asteroid entity management.
 */
#ifndef ASTEROID_H
#define ASTEROID_H

#include "game.h"
#include "../graphics/sprite.h"

/**
 * @brief Spawns a new asteroid in the game.
 * @param game Pointer to the Game state.
 * @param type The type of the enemy to spawn.
 */
void asteroid_spawn(Game *game, EnemyType type);

/**
 * @brief Updates all active asteroids.
 * @param game Pointer to the Game state.
 */
void asteroid_update_all(Game *game);

/**
 * @brief Checks if a formation wave is currently stopped on screen.
 *
 * While this is true, other enemy spawns can be paused so the formation remains
 * the focus of the round.
 *
 * @param game Pointer to the Game state.
 * @return true if at least one formation enemy is holding position.
 */
bool asteroid_has_holding_formation(const Game *game);

/**
 * @brief Draws all active asteroids.
 * @param game Pointer to the Game state.
 * @param basic_sprite Pointer to the sprite for basic asteroids.
 * @param zigzag_sprite Pointer to the sprite for zigzag asteroids.
 * @param formation_sprite Pointer to the sprite for formation asteroids.
 * @param shooter_sprite Pointer to the sprite for shooter asteroids.
 */
void asteroid_draw_all(const Game *game,
                       const Sprite *basic_sprite,
                       const Sprite *zigzag_sprite,
                       const Sprite *formation_sprite,
                       const Sprite *shooter_sprite);

#endif
