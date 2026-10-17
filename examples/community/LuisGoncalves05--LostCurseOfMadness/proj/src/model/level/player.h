#ifndef PLAYER_H
#define PLAYER_H

#include <lcom/lcf.h>
#include <math.h>
#include <stdlib.h>

#include "assets/xpm/level/player/idle.xpm"
#include "assets/xpm/level/player/walking.xpm"
#include "config.h"
#include "drivers/video/gpu.h"
#include "model/utils/animated_sprite.h"
#include "model/utils/keys.h"
#include "model/utils/sprite.h"

#define PLAYER_HEALTH 3        /**< Health of the player */
#define PLAYER_DEFAULT_SPEED 3 /**< Default speed of the player */

/**
 * @brief Represents a single Player instance.
 *
 * The Player struct is opaque.
 */
typedef struct Player Player;

/**
 * @brief Possible states of the player for animations and logic.
 */
typedef enum {
    PLAYER_IDLE,     /**< Player is idle */
    PLAYER_WALKING,  /**< Player is walking */
    PLAYER_AIMING,   /**< Player is aiming */
    PLAYER_SHOOTING, /**< Player is shooting */
    PLAYER_DEAD,     /**< Player is dying */
    PLAYER_WIN       /**< Player is winning */
} PlayerState;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

/**
 * @brief Creates a new player instance.
 *
 * @return Pointer to the created Player structure.
 */
Player *create_player();

/**
 * @brief Frees the memory associated with the player.
 *
 * @param player Pointer to the Player structure to destroy.
 * @return 0 on success, 1 on failure.
 */
int destroy_player(Player *player);

/**
 * @brief Returns the Sprite associated with the player.
 *
 * @param player Pointer to the Player structure.
 * @return Pointer to the Sprite structure.
 */
Sprite *player_get_sprite(Player *player);

/**
 * @brief Returns the x-coordinate of the player.
 *
 * @param player Pointer to the Player structure.
 * @return The player's x-coordinate.
 */
uint16_t player_get_x(Player *player);

/**
 * @brief Sets the x of the player.
 *
 * @param player Pointer to the Player structure.
 * @param x New x value.
 */
void player_set_x(Player *player, uint16_t x);

/**
 * @brief Returns the y-coordinate of the player.
 *
 * @param player Pointer to the Player structure.
 * @return The player's y-coordinate.
 */
uint16_t player_get_y(Player *player);

/**
 * @brief Sets the yx of the player.
 *
 * @param player Pointer to the Player structure.
 * @param y New y value.
 */
void player_set_y(Player *player, uint16_t y);

/**
 * @brief Returns the x-speed of the player.
 *
 * @param player Pointer to the Player structure.
 * @return The player's x-speed.
 */
double player_get_xspeed(Player *player);

/**
 * @brief Sets the x-speed of the player.
 *
 * @param player Pointer to the Player structure.
 * @param xspeed New x-speed value.
 */
void player_set_xspeed(Player *player, double xspeed);

/**
 * @brief Returns the y-speed of the player.
 *
 * @param player Pointer to the Player structure.
 * @return The player's y-speed.
 */
double player_get_yspeed(Player *player);

/**
 * @brief Sets the y-speed of the player.
 *
 * @param player Pointer to the Player structure.
 * @param yspeed New y-speed value.
 */
void player_set_yspeed(Player *player, double yspeed);

/**
 * @brief Returns the width of the player.
 *
 * @param player Pointer to the Player structure.
 * @return The player's width.
 */
uint16_t player_get_width(Player *player);

/**
 * @brief Returns the height of the player.
 *
 * @param player Pointer to the Player structure.
 * @return The player's height.
 */
uint16_t player_get_height(Player *player);

/**
 * @brief Returns the AnimatedSprite associated with the player.
 *
 * @param player Pointer to the Player structure.
 * @return Pointer to the AnimatedSprite structure.
 */
AnimatedSprite *player_get_animated_sprite(Player *player);

/**
 * @brief Returns the current Direction of the player.
 *
 * @param player Pointer to the Player structure.
 * @return Current player Direction value.
 */
Direction player_get_direction(Player *player);

/**
 * @brief Returns the current state of the player.
 *
 * @param player Pointer to the Player structure.
 * @return Current player state.
 */
PlayerState player_get_state(Player *player);

/**
 * @brief Sets the current state of the player.
 *
 * @param player Pointer to the Player structure.
 * @param health New state value.
 */
void player_set_state(Player *player, PlayerState state);

/**
 * @brief Updates the player's state based on the given mouse packet.
 *
 * @param player Pointer to the Player structure.
 * @param pp Mouse packet used to determine action.
 * @return 0 on success, 1 on failure.
 */
int player_update_state(Player *player, struct packet pp);

/**
 * @brief Updates the player's speed based on keyboard input.
 *
 * @param player Pointer to the Player structure.
 * @param scan_code Keyboard scan code.
 * @return 0 on success, 1 on failure.
 */
int player_update_speed(Player *player, uint8_t scan_code);

/**
 * @brief Decreases the player's health by one.
 *
 * @param player Pointer to the Player structure.
 * @return 0 on success, 1 on failure.
 */
int player_lose_health(Player *player);

/**
 * @brief Renders the player on the screen.
 *
 * @param player Pointer to the Player structure.
 * @param delta Direction angle in radians.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, 1 on failure.
 */
int draw_player(Player *player, double delta, uint8_t *frame_buffer);

#endif /* PLAYER_H */
