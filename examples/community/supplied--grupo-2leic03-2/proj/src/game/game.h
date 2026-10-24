/**
 * @file game.h
 * @brief Main game state and logic.
 */
#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#include "../config.h"
#include "../input/input.h"
#include "../graphics/sprite.h"
#include "state.h"
#include "entity.h"

#define MAX_ASTEROIDS 32
#define MAX_BULLETS 64
#define MAX_ENEMY_BULLETS 64
#define MAX_EXPLOSIONS 32
#define MAX_POWERUPS 4
#define MAX_FLOATING_TEXTS 8

#define ASTEROID_SIZE 48
#define BULLET_SPEED 12
#define ENEMY_BULLET_SPEED 5
#define ASTEROID_SPEED 2
#define ASTEROID_MAX_SPEED 5
#define ASTEROID_SPEEDUP_KILLS 15
#define ASTEROID_FORMATION_START_FRAMES (TARGET_FPS * 12)
#define ASTEROID_FORMATION_HOLD_FRAMES (TARGET_FPS * 6)
#define SHOOT_COOLDOWN_FRAMES 15
#define EXPLOSION_DURATION 18
#define MENU_NAV_COOLDOWN_FRAMES 12
#define SCORE_TIME_INTERVAL_FRAMES 10
#define SCORE_PER_KILL 200
#define SCORE_PER_FORMATION_KILL 100
#define SCORE_LOST_PER_MISSED_ENEMY 200

/**
 * @brief Structure representing the player's ship.
 */
typedef struct {
  Entity base;
  int speed;
  int lives;
} Ship;

/**
 * @brief Enemy movement/visual variants.
 */
typedef enum {
  ENEMY_BASIC,
  ENEMY_ZIGZAG,
  ENEMY_FORMATION,
  ENEMY_SHOOTER
} EnemyType;

/**
 * @brief Structure representing an asteroid.
 */
typedef struct {
  Entity base;
  EnemyType type;
  int age;
  int score_value;
  int hold_y;
  int shoot_cooldown;
  bool costs_life;
  bool formation_released;
} Asteroid;

/**
 * @brief Structure representing a bullet.
 */
typedef struct {
  Entity base;
} Bullet;

/**
 * @brief Structure representing an explosion.
 */
typedef struct {
  Entity base;
  int age;
} Explosion;

/**
 * @brief Structure representing a boss enemy.
 */
typedef struct {
  Entity base;
  int hp;
  int max_hp;
  int shoot_cooldown;
  int laser_warning_timer;
  int laser_fire_timer;
  int laser_cooldown;
  int laser_count;
  int laser_x[4]; // 4 is BOSS_MAX_LASERS
  int shield_spawn_timer;
  int shields_spawned;
  int shield_spawn_limit;
  int round;
  int sprite_scale;
  int reward_score;
  int next_level;
  int entry_y;
  bool active;
  bool defeated;
  bool laser_hit_ship;
} Boss;

/**
 * @brief Powerup types.
 */
typedef enum {
  POWERUP_HEALTH,
  POWERUP_SHIELD,
  POWERUP_FIRE_RATE,
  POWERUP_MULTI_SHOT
} PowerupType;

/**
 * @brief Structure representing a powerup.
 */
typedef struct {
  Entity base;
  PowerupType type;
} Powerup;

/**
 * @brief Structure representing a floating text animation.
 */
typedef struct {
  int x;
  int y;
  int age;
  char text[32];
  uint32_t color;
  bool active;
} FloatingText;

/**
 * @brief Main game state structure.
 */
typedef struct Game {
  GameState state;
  Ship ship;
  Boss boss;
  Asteroid asteroids[MAX_ASTEROIDS];
  Bullet bullets[MAX_BULLETS];
  Bullet enemy_bullets[MAX_ENEMY_BULLETS];
  Explosion explosions[MAX_EXPLOSIONS];
  Powerup powerups[MAX_POWERUPS];
  FloatingText floating_texts[MAX_FLOATING_TEXTS];
  int score;
  int level;
  int round_message_timer;
  int basic_spawn_timer;
  int zigzag_spawn_timer;
  int formation_spawn_timer;
  int health_spawn_timer;
  int shield_spawn_timer;
  int weapon_powerup_spawn_timer;
  int shield_timer;
  bool health_powerup_spawned;
  bool shield_powerup_spawned;
  bool weapon_powerup_spawned;
  bool weapon_fire_rate_active;
  bool weapon_multi_shot_active;
  int shooter_spawn_timer;
  int frame_counter;
  int shoot_cooldown;
  int asteroids_destroyed;
  int boss_message_timer;
  int win_message_timer;
  bool round10_boss_defeated;
  char rtc_time_str[16];
  char rtc_date_str[16];
} Game;

/**
 * @brief Initializes the game state.
 * @param game Pointer to the Game state.
 */
void game_init(Game *game);

/**
 * @brief Destroys and frees game resources.
 */
void game_destroy(void);

/**
 * @brief Updates the game logic for one frame.
 * @param game Pointer to the Game state.
 * @param input Pointer to the current InputState.
 */
void game_update(Game *game, const InputState *input);

/**
 * @brief Draws the entire game state.
 * @param game Pointer to the Game state.
 * @param input Pointer to the current InputState.
 */
void game_draw(Game *game, const InputState *input);

/**
 * @brief Resets the gameplay state for a new session.
 * @param game Pointer to the Game state.
 */
void game_reset_play_state(Game *game);

extern Sprite cursor_sprite;
extern Sprite player_sprite;

#endif
