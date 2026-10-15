#ifndef __PROJ_GAME_H
#define __PROJ_GAME_H

#include "model/board/board.h"
#include "model/entity/entity.h"
#include "model/resources/resources.h"
#include "model/sprite/anim_sprite.h"
#include "model/sprite/sprite.h"

/** @defgroup game game
 * @{
 *
 * Functions for managing the game state
 */

#define MAX_ENEMIES 10    /**< @brief Maximum number of enemies allowed in the game */
#define MAX_BRICKS 50     /**< @brief Maximum number of bricks allowed in the game */
#define MAX_WALLS 150     /**< @brief Maximum number of walls allowed in the game */
#define MAX_BOMBS 5       /**< @brief Maximum number of bombs that can exist simultaneously */
#define MAX_EXPLOSIONS 45 /**< @brief Maximum number of explosions that can exist simultaneously */

/**
 * @brief Enum representing the game state
 */
typedef enum {
  START, /**< @brief Game is in start menu */
  PAUSE, /**< @brief Game is in pause menu */
  GAME,  /**< @brief Game is being played */
  WIN,   /**< @brief The player won the game */
  LOSE,  /**< @brief The player lost the game */
  EXIT   /**< @brief Game is exiting */
} game_state_t;

/**
 * @brief Struct representing the game
 *
 * This struct contains the current state of the game, the game board,
 * and all game entities (player, enemies, bricks, walls, bombs).
 * All entities are stored in fixed-size arrays to avoid dynamic memory allocation.
 */
typedef struct Game {
  game_state_t state;                /**< @brief Current state of the game */
  uint8_t menu_option;               /**< @brief Current selected menu option */
  GameBoard board;                   /**< @brief Current game board */
  Entity player;                     /**< @brief The player entity */
  Entity enemies[MAX_ENEMIES];       /**< @brief Array of enemy entities */
  Entity bricks[MAX_BRICKS];         /**< @brief Array of brick entities */
  Entity walls[MAX_WALLS];           /**< @brief Array of wall entities */
  Entity bombs[MAX_BOMBS];           /**< @brief Array of bomb entities */
  Entity explosions[MAX_EXPLOSIONS]; /**< @brief Array of explosion entities */
  Entity powerup;                    /**< @brief The powerup entity */
  Entity door;                       /**< @brief The level exit door entity */
  uint8_t num_enemies;               /**< @brief Number of enemies in the game */
  uint8_t num_bricks;                /**< @brief Number of bricks in the game */
  uint8_t num_walls;                 /**< @brief Number of walls in the game */
  uint8_t num_bombs;                 /**< @brief Number of bombs in the game */
  uint8_t num_explosions;            /**< @brief Number of explosions in the game */
  uint8_t level;                     /**< @brief Current game level */
  uint8_t door_timer;                /**< @brief Timer for door interaction */
  uint8_t score;                     /**< @brief Player's current score */
  uint8_t level_timer;               /**< @brief Seconds remaining to complete level */
} Game;

/**
 * @brief Loads the next level of the game
 *
 * Increments the level counter, loads the corresponding board file,
 * and initializes all entities according to the board layout.
 * Preserves the player's remaining lives between levels.
 *
 * @param game Pointer to the current game state
 *
 * @return 0 upon success, non-zero otherwise
 */
int(load_next_level)(Game *game);

/**
 * @brief Initializes the game
 *
 * Sets up the game state and initializes the game board,
 * player, enemies, bricks, walls, and bombs based on the board layout.
 * Uses stack-allocated entities to avoid dynamic memory allocation.
 *
 * @param game Pointer to the game to be initialized
 *
 * @return 0 upon success, non-zero otherwise
 */
int(init_game)(Game *game);

/**
 * @brief Resets the game
 *
 * Resets all entities in the game to their default state and
 * clears the game board.
 *
 * @param game Pointer to the game to be reset
 *
 * @return 0 upon success, non-zero otherwise
 */
int(reset_game)(Game *game);

/**
 * @brief Updates door interaction timer and handles level progression
 *
 * Checks if player is standing on door after all bricks are destroyed.
 * Increments timer when conditions are met, loads next level after 3 seconds.
 *
 * @param game Pointer to the current game state
 */
void(update_door_timer)(Game *game);

/**
 * @brief Updates level timer and checks for timeout
 *
 * Decrements level timer and sets game state to LOSE if timer reaches 0.
 *
 * @param game Pointer to the current game state
 */
void(update_level_timer)(Game *game);

/**@}*/

#endif /* __PROJ_GAME_H */
