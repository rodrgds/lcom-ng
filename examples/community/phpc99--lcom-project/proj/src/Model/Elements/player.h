#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdint.h>
#include <stdbool.h>

#include "src/include.h"
#include "src/Model/Position.h"
#include "src/Model/Elements/Backpack.h"

/**
 * @file Player.h
 * @brief Declares the Player structure and related functions for controlling the main character.
 *
 * Defines the player entity, state, and associated backpack inventory.
 */

/**
 * @enum PlayerState
 * @brief Defines the possible states of the player.
 */

typedef enum {
    PLAYER_IDLE,
    PLAYER_WALKING,
    PLAYER_JUMPING
} PlayerState;

/**
 * @struct Player
 * @brief Represents the player entity in the game.
 *
 * Stores player's position, size, velocity, ground status, state,
 * and pointer to their inventory (backpack).
 */

typedef struct {
    Position position;
    int16_t width, height;
    int16_t vy;
    bool on_ground;
    PlayerState state;
    Backpack *backpack;
} Player;

/**
 * @brief Creates and initializes a new Player.
 *
 * @param position Initial position of the player.
 * @param on_ground Whether the player starts on the ground.
 * @return Pointer to the newly created Player, or NULL on failure.
 */

Player *new_Player(Position position, bool on_ground);

/**
 * @brief Frees memory associated with a Player.
 *
 * Also deletes the backpack if it exists.
 *
 * @param p Pointer to the Player to delete.
 */

void delete_Player(Player *p);

/**
 * @brief Gets the current position of the Player.
 *
 * @param p Pointer to the Player.
 * @return The player's position.
 */

Position get_Player_Position(Player *p);

#endif
