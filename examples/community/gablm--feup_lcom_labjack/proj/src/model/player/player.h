#ifndef _LCOM_PLAYER_H_
#define _LCOM_PLAYER_H_

#include <lcom/lcf.h>
#include "../../data_structures/queue/queue.h"
#include "../player/player.h"
#include "../card/card.h"
#include "../font/font.h"

#define PLAYER_STARTING_COINS 100
#define PLAYER_MAX_DECK_SIZE 15

/**
 * @defgroup player Player
 * @ingroup models
 * @{
 * 
 * @brief Player module for handling player-related operations and state.
 */

/**
 * @enum game_over_state_t
 * @brief Enumeration for the game over state.
 */
typedef enum {
	NOT_ENDED,  /**< Game is not finished. */
	PLAYER_WIN, /**< Player has won. */
	PLAYER_LOSS, /**< Player has lost. */
	PLAYER_DRAW, /**< Game is a draw. */
	PLAYER_SURRENDER /**< Player has surrended. */
} game_over_state_t;

/**
 * @struct player_t
 * @brief Structure to represent a player.
 */
typedef struct {
	uint32_t coins;                /**< Number of coins the player has. */
	uint32_t bet;                  /**< Current bet of the player. */
	uint32_t cards_value;          /**< Value of the player's cards. */
	uint32_t won_coins;            /**< Number of coins won by the player. */
	game_over_state_t game_over_state; /**< State of the game for the player. */
	queue_t *cards;                /**< Queue of cards the player has. */
} player_t;

/**
 * @brief Initializes the player.
 *
 * @param player Pointer to the player structure to initialize.
 * @return 0 on success, 1 on failure.
 */
int player_init(player_t *player);

/**
 * @brief Destroys the player, freeing all allocated resources.
 *
 * @param player Pointer to the player structure to destroy.
 */
void player_destroy(player_t *player);

/**
 * @brief Draws the player's cards on the screen.
 *
 * @param player Pointer to the player structure.
 */
void player_draw(player_t *player);

/** @} */

#endif /* _LCOM_PLAYER_H_ */
