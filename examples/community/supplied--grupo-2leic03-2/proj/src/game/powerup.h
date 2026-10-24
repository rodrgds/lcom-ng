/**
 * @file powerup.h
 * @brief Powerup entities.
 */
#ifndef POWERUP_H
#define POWERUP_H

#include "game.h"
#include "../graphics/sprite.h"

/**
 * @brief Spawns a new powerup.
 * @param game Pointer to the Game state.
 * @param type Type of the powerup.
 */
bool powerup_spawn(Game *game, PowerupType type);

/**
 * @brief Updates all active powerups.
 * @param game Pointer to the Game state.
 */
void powerup_update_all(Game *game);

/**
 * @brief Draws all active powerups.
 * @param game Pointer to the Game state.
 * @param health_sprite Pointer to the health powerup Sprite.
 * @param shield_sprite Pointer to the shield powerup Sprite.
 * @param fire_rate_sprite Pointer to the fire rate powerup Sprite.
 * @param multi_shot_sprite Pointer to the multi shot powerup Sprite.
 */
void powerup_draw_all(const Game *game, const Sprite *health_sprite, const Sprite *shield_sprite, const Sprite *fire_rate_sprite, const Sprite *multi_shot_sprite);

#endif
