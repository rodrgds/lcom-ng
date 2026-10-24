/**
 * @file bullet.h
 * @brief Bullet entity management.
 */
#ifndef BULLET_H
#define BULLET_H

#include "game.h"
#include "../graphics/sprite.h"

/**
 * @brief Spawns a new bullet.
 * @param game Pointer to the Game state.
 */
void bullet_spawn(Game *game);

/**
 * @brief Spawns an enemy bullet moving approximately toward a target point.
 *
 * @param game Pointer to the Game state.
 * @param x Initial bullet x coordinate.
 * @param y Initial bullet y coordinate.
 * @param target_x Target x coordinate.
 * @param target_y Target y coordinate.
 */
void enemy_bullet_spawn(Game *game, int x, int y, int target_x, int target_y);

/**
 * @brief Updates all active bullets.
 * @param game Pointer to the Game state.
 */
void bullet_update_all(Game *game);

/**
 * @brief Draws all active bullets.
 * @param game Pointer to the Game state.
 * @param sprite Pointer to the bullet Sprite.
 */
void bullet_draw_all(const Game *game, const Sprite *sprite);

/**
 * @brief Draws all active enemy bullets.
 * @param game Pointer to the Game state.
 * @param sprite Pointer to the enemy bullet Sprite.
 */
void enemy_bullet_draw_all(const Game *game, const Sprite *sprite);

#endif
