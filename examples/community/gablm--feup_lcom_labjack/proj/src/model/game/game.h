#ifndef _LCOM_GAME_H_
#define _LCOM_GAME_H_

#include <lcom/lcf.h>

#include "../../data_structures/queue/queue.h"
#include "../player/player.h"
#include "../card/card.h"
#include "../../assets/cards/back_red_1.xpm"
#include "../animation/animation.h"

#define GAME_DECK_SIZE 52
#define GAME_DECK_AMOUNT 2
#define GAME_DECK_DRAW_AMOUNT 4
#define GAME_DECK_DRAW_X 815
#define GAME_DECK_DRAW_Y 250

/**
 * @defgroup game Game
 * @ingroup models
 * @{
 * 
 * @brief Game module for handling the main game logic and state.
 */

/**
 * @struct game_t
 * @brief Structure to represent the state of the game.
 */
typedef struct {
  sprite_t *card_back;               /**< Sprite for the back of the cards. */
  queue_t *cards;                    /**< Queue of cards for the game deck. */
  queue_t *dealer;                   /**< Queue of cards for the dealer. */
  animation_t *curr_anim;            /**< Pointer to the current animation. */
  uint32_t dealer_value;             /**< Value of the dealer's cards. */
  uint8_t input_select;              /**< Input select flag. */
  uint8_t dealer_turn;               /**< Flag indicating if it's the dealer's turn. */
  uint8_t dealer_ignore_last;        /**< Flag to ignore the dealer's last card. */
  player_t main_player;              /**< Main player of the game. */
  player_t other_player;             /**< Other player in the game. */
} game_t;

/**
 * @brief Initializes the game.
 *
 * @param game Pointer to the game structure to initialize.
 * @return 0 on success, 1 on failure.
 */
int game_init(game_t *game);

/**
 * @brief Destroys the game, freeing all allocated resources.
 *
 * @param game Pointer to the game structure to destroy.
 */
void game_destroy(game_t *game);

/**
 * @brief Adds a deck of cards to the game, optionally shuffling it.
 *
 * @param cards Pointer to the queue where the deck will be added.
 * @param shuffle Flag indicating whether to shuffle the deck.
 * @return 0 on success, 1 on failure.
 */
int game_add_deck(queue_t *cards, bool shuffle);

/**
 * @brief Draws the game deck on the screen.
 *
 * @param game Pointer to the game structure.
 * @return 0 on success, 1 on failure.
 */
int game_draw_deck(game_t *game);

/**
 * @brief Draws the dealer's cards on the screen.
 *
 * @param game Pointer to the game structure.
 * @param font Pointer to the font structure used for rendering text.
 * @return 0 on success, 1 on failure.
 */
int game_draw_dealer(game_t *game, font_t *font);

/**
 * @brief Gives a card from the deck to a receiver queue.
 *
 * @param deck Pointer to the deck queue.
 * @param receiver Pointer to the receiver queue.
 * @return 0 on success, 1 on failure.
 */
int game_give_card(queue_t *deck, queue_t *receiver);

/**
 * @brief Calculates the total value of the cards in a queue.
 *
 * @param cards Pointer to the queue of cards.
 * @return The total value of the cards.
 */
uint32_t game_get_cards_value(queue_t *cards);

/** @} */

#endif /* _LCOM_EV_GAME_H_ */
