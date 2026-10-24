/**
 * @file boss.h
 * @brief Boss enemy logic and rendering.
 */
#ifndef BOSS_H
#define BOSS_H

#include "game.h"

#define BOSS_SPRITE_SCALE 3
#define BOSS_WIDTH 288
#define BOSS_HEIGHT 96
#define BOSS_ROUND_5_HP 60
#define BOSS_ROUND_5_SCORE 1000
#define BOSS_ROUND_5_FORMATION_STOP_SCORE 23500
#define BOSS_ROUND_5_ENTRY_SCORE 25000
#define BOSS_ENTRY_Y 104
#define BOSS_ENTRY_SPEED 2
#define BOSS_ROUND_10_SCALE 4
#define BOSS_ROUND_10_WIDTH 384
#define BOSS_ROUND_10_HEIGHT 128
#define BOSS_ROUND_10_HP 150
#define BOSS_ROUND_10_SCORE 3000
#define BOSS_ROUND_10_ENTRY_SCORE 50000
#define BOSS_ROUND_10_ENTRY_Y 82
#define BOSS_MOVE_SPEED 2
#define BOSS_FIRE_MIN_INTERVAL_FRAMES 18
#define BOSS_FIRE_RANDOM_INTERVAL_FRAMES 30
#define BOSS_MESSAGE_FRAMES (TARGET_FPS * 3)
#define WIN_MESSAGE_FRAMES (TARGET_FPS * 4)
#define BOSS_MAX_LASERS 4
#define BOSS_LASER_WARNING_FRAMES (TARGET_FPS * 2)
#define BOSS_LASER_FIRE_FRAMES 42
#define BOSS_LASER_COOLDOWN_FRAMES (TARGET_FPS * 9)
#define BOSS_LASER_RANDOM_COOLDOWN_FRAMES (TARGET_FPS * 7)
#define BOSS_LASER_WIDTH 8
#define BOSS_LASER_WARNING_WIDTH 2

/**
 * @brief Initializes boss-related resources (e.g. sprites).
 */
void boss_init(void);

/**
 * @brief Destroys boss-related resources.
 */
void boss_destroy(void);

/**
 * @brief Resets the boss state for a new game.
 * @param game Pointer to the game state.
 */
void boss_reset(Game *game);

/**
 * @brief Spawns the level 5 boss.
 * @param game Pointer to the game state.
 */
void boss_spawn_round_5(Game *game);

/**
 * @brief Spawns the level 10 boss.
 * @param game Pointer to the game state.
 */
void boss_spawn_round_10(Game *game);

/**
 * @brief Updates the boss logic (movement, attacks).
 * @param game Pointer to the game state.
 */
void boss_update(Game *game);

/**
 * @brief Checks for collisions between player bullets and the boss.
 * @param game Pointer to the game state.
 */
void boss_check_bullet_collisions(Game *game);

/**
 * @brief Draws the boss lasers and warnings.
 * @param game Pointer to the game state.
 */
void boss_draw_lasers(const Game *game);

/**
 * @brief Draws the boss and its health bar.
 * @param game Pointer to the game state.
 */
void boss_draw(const Game *game);

/**
 * @brief Draws the boss entry message.
 * @param game Pointer to the game state.
 */
void boss_draw_message(const Game *game);

/**
 * @brief Defeats the boss, triggering explosions and granting rewards.
 * @param game Pointer to the game state.
 */
void boss_defeat(Game *game);

#endif
