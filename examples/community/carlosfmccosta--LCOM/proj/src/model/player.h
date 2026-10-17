/**
 * @file player.h
 * @brief Interface for player entity management.
 *
 * This header provides the structure and functions for creating, updating, drawing,
 * and destroying the player, as well as handling player actions, movement, health,
 * ammo, and sprite management.
 */
#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdint.h>
#include <stdbool.h>
#include "sprite.h"
#include "assets/player/walk.h"
#include "assets/player/idle.h"
#include "assets/player/shoot.h"

struct player;
typedef struct player player_t;

/** @def Player movement speed */
#define PLAYER_SPEED 8 

/** @def Maximum number of bullets the player can shoot */
#define MAX_BULLETS 50 

/** @def Maximum ammo capacity in a magazine */
#define MAX_AMMO_IN_MAGAZINE 3 

/**
 * @enum action
 * @brief Enumeration of player possible actions.
 */
typedef enum {
    A_WALK,  /**< Player walking action */
    A_SHOOT, /**< Player shooting action */
    A_IDLE   /**< Player idle action */
} action;

/**
 * @brief Initializes a player at the given starting position.
 * 
 * @param start_x Initial X coordinate of the player.
 * @param start_y Initial Y coordinate of the player.
 * @return Pointer to the initialized player_t struct or NULL on failure.
 */
player_t* player_init(int16_t start_x, int16_t start_y);

/**
 * @brief Frees all allocated resources for the player.
 *
 * @param player Pointer to the player to destroy.
 */
void destroy_player(player_t *player);

/**
 * @brief Moves the player by the specified delta values.
 *
 * @param player Pointer to the player.
 * @param dx Change in X position.
 * @param dy Change in Y position.
 */
void player_move(player_t *player, int16_t dx, int16_t dy);

/**
 * @brief Draws the player on the screen according to the current action.
 *
 * @param player Pointer to the player.
 * @return 0 if successful, 1 otherwise.
 */
int player_draw(player_t *player);

/**
 * @brief Draws the given sprite, animating it if the delay allows.
 *
 * @param sp Pointer to the sprite to draw.
 * @return 0 if successful, 1 otherwise.
 */
int draw_player_sprite(sprite_t *sp);

/**
 * @brief Applies damage to the player by reducing health by 1.
 *
 * @param player Pointer to the player.
 */
void player_take_damage(player_t *player);

/**
 * @brief Ensures the player remains within the screen boundaries, adjusting the position if he's out-of-bounds.
 *
 * @param player Pointer to the player.
 * @param screen_width Width of the game screen in pixels.
 * @param screen_height Height of the game screen in pixels.
 */
void player_check_bounds(player_t *player, unsigned int screen_width, unsigned int screen_height);

/**
 * @brief Gets the width of the player's sprite according to the current action.
 *
 * @param player Pointer to the player.
 * @return Width of the sprite in pixels.
 */
uint16_t get_player_width(player_t *player);

/**
 * @brief Gets the height of the player's sprite according to the current action.
 *
 * @param player Pointer to the player.
 * @return Height of the sprite in pixels.
 */
uint16_t get_player_height(player_t *player);

/**
 * @brief Returns a pointer to the player's current position.
 *
 * @param player Pointer to the player.
 * @return Pointer to the position_t struct.
 */
position_t * get_player_pos(player_t *player);

/**
 * @brief Returns the current health of the player.
 *
 * @param player Pointer to the player.
 * @return Current health value.
 */
unsigned int get_player_health(player_t *player);

/**
 * @brief Gets the sprite associated with the player's current action.
 *
 * @param player Pointer to the player.
 * @return Pointer to the sprite_t.
 */
sprite_t* get_player_sprite(player_t *player);

/**
 * @brief Returns whether the player is currently reloading.
 *
 * @param player Pointer to the player.
 * @return true if reloading, false otherwise.
 */
bool get_player_is_reloading(player_t *player);

/**
 * @brief Sets the reloading state of the player.
 *
 * @param player Pointer to the player.
 * @param is_reloading New reloading state.
 */
void set_player_is_reloading(player_t *player, bool is_reloading);

/**
 * @brief Returns the amount of ammo currently in the player's magazine.
 *
 * @param player Pointer to the player.
 * @return Number of bullets in the magazine.
 */
unsigned int get_player_current_ammo_in_magazine(player_t *player);

/**
 * @brief Sets the current ammo count in the player's magazine.
 *
 * @param player Pointer to the player.
 * @param ammo New ammo count.
 */
void set_player_current_ammo_in_magazine(player_t *player, unsigned int ammo);

/**
 * @brief Sets the player's current action.
 *
 * @param player Pointer to the player.
 * @param act New action to set.
 */
void set_player_act(player_t *player, action act);

/**
 * @brief Sets sprite flip status (horizontal mirroring) for idle and shooting sprites.
 *
 * @param player Pointer to the player.
 * @param flip True to flip sprites horizontally, false to reset.
 */
void set_player_flip_on_shoot(player_t *player, bool flip);

/**
 * @brief Sets the player to idle if the current animation has finished playing.
 *
 * @param player Pointer to the player.
 */
void set_player_idle(player_t *player);

/**
 * @brief Sets the player's position for all associated sprites.
 *
 * @param player Pointer to the player.
 * @param x New X coordinate.
 * @param y New Y coordinate.
 */
void set_player_pos(player_t *player, uint16_t x, uint16_t y);

#endif //__PLAYER_H
