/**
 * @file enemy.h
 * @brief Interface for enemy entity management.
 *
 * This header provides the structure and functions for creating, updating, drawing,
 * and destroying enemies, as well as handling their actions and interactions in the game.
 */
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <stdint.h>
#include <stdbool.h>
#include "sprite.h"
#include "player.h"
#include "wall.h"
#include "assets/enemy/walk.h"
#include "assets/enemy/idle.h"
#include "assets/enemy/shoot.h"

struct enemy;

/**
 * @brief Enumeration of possible enemy actions/states
 */
typedef enum {
    E_WALK,     /**< Enemy is walking */
    E_SHOOT,    /**< Enemy is shooting */
    E_IDLE      /**< Enemy is idle */
} enemy_action;

/**
 * @brief Structure representing an enemy entity
 */
typedef struct enemy { 
    enemy_action act;                /**< Current action/state of the enemy */
    sprite_t *walk;                  /**< Sprite for walking animation */
    sprite_t *idle;                  /**< Sprite for idle animation */
    sprite_t *shoot;                 /**< Sprite for shooting animation */
    bool active;                     /**< Whether the enemy is active */
    int shoot_cooldown_timer;        /**< Ticks until next shot */
    int direction;                   /**< Movement direction (0:left, 1:down, 2:right, 3:up) */     
} enemy_t;

/** @def Enemy movement speed */
#define ENEMY_SPEED 5

/** @def Initial number of enemies */
#define INITIAL_ENEMIES 5

/** @def Number of enemies to add at certain points */
#define ADD_ENEMIES 3

/** @def Maximum number of enemies allowed */
#define MAX_ENEMIES 150

/** @def Minimum spawn distance from player */
#define SPAWN_DISTANCE 100

/** @def Base cooldown ticks between enemy shots */
#define ENEMY_SHOOT_COOLDOWN_BASE_TICKS 180

/** @def Random variation in shot cooldown ticks */
#define ENEMY_SHOOT_COOLDOWN_RANDOM_TICKS 60

/** @brief Enemy sprite XPM */
extern const char* enemy_xpm[];

/**
 * @brief Initialize a new enemy at specified position
 * 
 * @param start_x Initial x position
 * @param start_y Initial y position
 * @return Pointer to created enemy, NULL on failure
 */
enemy_t* enemy_init(int16_t start_x, int16_t start_y);

/**
 * @brief Destroy an enemy and free its resources
 * 
 * @param enemy Enemy to destroy
 */
void destroy_enemy(enemy_t *enemy);

/**
 * @brief Move an enemy according to its current state
 * 
 * @param enemy Enemy to move
 * @param wall_arr Array of walls for collision detection
 * @param player Player entity for interaction
 * @param enemy_arr Array of enemies for collision detection
 */
void enemy_move(enemy_t *enemy, wall_array_t *wall_arr, player_t *player, enemy_t **enemy_arr);

/**
 * @brief Draw an enemy based on its current state
 * 
 * @param enemy Enemy to draw
 * @return 0 on success, non-zero on failure
 */
int enemy_draw(enemy_t *enemy);

/**
 * @brief Draw an enemy sprite with animation delay
 * 
 * @param sp Sprite to draw
 * @return 0 on success and 1 on failure
 */
int draw_enemy_sprite(sprite_t *sp); 

/**
 * @brief Check and enforce screen boundaries for an enemy
 * 
 * @param enemy Enemy to check
 * @param screen_width Width of game screen
 * @param screen_height Height of game screen
 */
void enemy_check_bounds(enemy_t *enemy, unsigned int screen_width, unsigned int screen_height);

/**
 * @brief Get width of enemy based on current state
 * 
 * @param enemy Enemy to measure
 * @return Width in pixels
 */
uint16_t get_enemy_width(enemy_t *enemy);

/**
 * @brief Get height of enemy based on current state
 * 
 * @param enemy Enemy to measure
 * @return Height in pixels
 */
uint16_t get_enemy_height(enemy_t *enemy);

/**
 * @brief Get current position of enemy
 * 
 * @param enemy Enemy to check
 * @return Pointer to position structure
 */
position_t * get_enemy_pos(enemy_t *enemy);

/**
 * @brief Create multiple enemies with proper spawning conditions
 * 
 * @param enemy_arr Array to store created enemies
 * @param num Number of enemies to create
 * @param wall_arr Array of walls for collision detection
 * @param player_pos Player position for spawn distance check
 * @param vres Vertical screen resolution
 * @param hres Horizontal screen resolution
 * @return 0 on success and 1 on failure
 */
int create_enemies(enemy_t** enemy_arr, int num, wall_array_t *wall_arr, position_t* player_pos,unsigned vres,unsigned hres);

/**
 * @brief Destroy all enemies in an array
 * 
 * @param enemy_arr Array of enemies to destroy
 */
void destroy_enemies(enemy_t** enemy_arr);

/**
 * @brief Draw all active enemies
 * 
 * @param enemy_arr Array of enemies to draw
 * @return 0 on success and 1 on failure
 */
int draw_enemies(enemy_t** enemy_arr);

/**
 * @brief Move all active enemies
 * 
 * @param enemy_arr Array of enemies to move
 * @param screen_width Width of game screen
 * @param screen_height Height of game screen
 * @param wall_arr Array of walls for collision detection
 * @param player Player entity for interaction
 */
void move_enemies(enemy_t** enemy_arr,unsigned int screen_width, unsigned int screen_height, wall_array_t *wall_arr, player_t *player);

/**
 * @brief Get the current sprite of an enemy based on its state
 * 
 * @param enemy Enemy to check
 * @return Pointer to current sprite
 */
sprite_t* get_enemy_sprite(enemy_t *enemy);

/**
 * @brief Set enemy's current action
 * 
 * @param enemy Enemy to modify
 * @param act New action
 */
void set_enemy_act(enemy_t *enemy, enemy_action act);

/**
 * @brief Set sprite flip state for shooting animation
 * 
 * @param enemy Enemy to modify
 * @param flip Whether to flip the sprite or not
 */
void set_enemy_flip_on_shoot(enemy_t *enemy, bool flip);

/**
 * @brief Set position of an enemy
 * 
 * @param enemy Enemy to position
 * @param x X coordinate
 * @param y Y coordinate
 */
void set_enemy_pos(enemy_t *enemy, uint16_t x, uint16_t y);

#endif // _ENEMY_H_
