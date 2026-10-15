/**
 * @file enemy.h
 * @brief Defines structures and functions for enemy characters.
 *
 * This file includes the definition of the `Character` struct (used for both players and enemies),
 * the `CharacterPos` struct, constants for enemy management, and function prototypes
 * for initializing, creating, adding, updating, and drawing enemy units.
 */

#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_SPAWN_DELAY 180 /**< Delay in timer ticks before attempting to spawn a new enemy. */
#define MAX_ENEMIES 20  /**< Maximum number of active enemies allowed at once. */

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "controllers/video/graphics.h"


typedef struct {
    uint16_t x_pos, y_pos;
    uint16_t width, height;
    uint8_t *sprite;

    int health;
    int damage;
    int attack_range;
    int pixels_per_sec;

    bool enemy;
    unsigned int attack_cooldown;

    // Animation
    bool is_attacking;
    unsigned int attack_anim_timer;
    uint8_t *default_sprite;
    uint16_t default_width, default_height;
    uint8_t *attack_sprite;
    uint16_t attack_width, attack_height;
    uint8_t *attack_sprite2;
    uint16_t attack2_width, attack2_height;
} Character;


typedef struct {
    unsigned int x;
    unsigned int y;
} CharacterPos;

extern Character *active_enemies[MAX_ENEMIES];
extern int num_active_enemies;
extern unsigned int enemy_spawn_tick_counter;
extern unsigned int timer_frequency_hz;

/**
 * @brief Creates a new enemy character instance.
 * @param pos The position (CharacterPos) where the enemy will be created.
 * @param enemy_type The type of enemy to create (determines stats and sprites).
 * @return Pointer to the created Character, or NULL on failure.
 */
Character *createEnemy(CharacterPos pos, int enemy_type);

/**
 * @brief Adds an enemy unit to the game.
 * This function typically calls createEnemy and then adds the unit to the active list.
 * @param pos The position (CharacterPos) where the enemy will be spawned.
 * @param enemy_type The type of enemy to add.
 */
void add_enemy_to_game(CharacterPos pos, int enemy_type);

/**
 * @brief Updates the state of all active enemy units and handles spawning.
 * This includes movement, AI logic (like attacking), managing animations, and periodic spawning of new enemies.
 */
void update_and_spawn_enemies(void);

/**
 * @brief Draws all active enemy units to the screen.
 */
void draw_enemies(void);

/**
 * @brief Initializes enemy resources, such as loading sprites.
 * @return True if initialization is successful, false otherwise.
 */
bool init_enemies(void);

/**
 * @brief Sets the timer frequency used for enemy character updates.
 * @param hz The frequency in Hertz.
 */
void set_timer_frequency_for_enemies(unsigned int hz);

#endif // ENEMY_H
