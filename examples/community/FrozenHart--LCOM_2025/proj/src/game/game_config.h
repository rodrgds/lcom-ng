#ifndef _LCOM_GAME_CONFIG_H
#define _LCOM_GAME_CONFIG_H

#include <lcom/lcf.h>

/** @defgroup game_config game_config
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define GAME_TICKS_PER_SECOND 60  /** @brief Game ticks per second */
#define PLAYER_INITIAL_MONEY 1000 /** @brief Initial money for each player */
#define FPS 30
#define PPS 60 // time it processes the state per second
#define GRAPHIC_MODE 0x14C // 1152x864  Direct color	 32 ((8:)8:8:8)
#define MAX_BUTTONS 4

typedef enum {
  WAITING_PLAYER_ACTION_GAME,
  START,     /** @brief Start action */
  DEAL,      /** @brief Deal action */
  BET,       /** @brief Bet action */
  DECISIONS, /** @brief Decision action */
  PAYOUT,    /** @brief Payout action */
  END_GAME   /** @brief End action */
} game_action_state_t;

typedef enum {
  WAITING_PLAYER_ACTION, /** @brief Waiting Player Action */
  STAY,                  /** @brief Stay action */
  DOUBLE,                /** @brief Double action */
  HIT,                   /** @brief Hit action */
  SPLIT,                 /** @brief Split action */
  END_TURN               /** @brief End action */
} turn_action_state_t;

typedef enum {
  MAIN_MENU,     /** @brief Main menu state */
  SINGLE_PLAYER, /** @brief Single player state */
  MULTI_PLAYER,  /** @brief Multi-player state */
  EXIT           /** @brief Exit state */
} application_state_t;

/**
 * @brief Represents a single playing card.
 * @param number Card number (1-13), where 1 is Ace, 11 is Jack, 12 is Queen, 13 is King.
 * @param letter Card suit ('H' = Hearts, 'D' = Diamonds, 'C' = Clubs, 'S' = Spades).
 */
typedef struct {
  int number;           /** @brief Card number (1-13) */
  unsigned char letter; /** @brief Card suit (H,D,C,S) */
} card_t;               /** @brief Card structure */

/**
 * @brief Represents a hand of cards for a player or dealer.
 * @param cards Pointer to array of cards in the hand.
 * @param num_cards Number of cards currently in the hand.
 * @param place_on_table Position of the hand on the table (1-8).
 * @param current_bet Current bet placed on this hand.
 */
typedef struct {
  card_t *cards;      /** @brief Array of cards in hand */
  int num_cards;      /** @brief Number of cards in hand */
  int place_on_table; /** @brief Position on table (1-8) */
  int current_bet;    /** @brief Current bet amount */
  bool wining_hand;
} hand_t;

/**
 * @brief Represents a player's data in the game.
 * @param money Amount of money the player has.
 * @param ID Unique player identifier.
 * @param action Current action state of the player.
 * @param hands Pointer to array of hands (for splits).
 * @param hands_count Number of hands the player currently has.
 */
typedef struct {
  int money;                  /** @brief Money */
  int ID;                     /** @brief ID */
  turn_action_state_t action; /** @brief Action state */
  hand_t *hands;              /** @brief Player hands */
  int hands_count;            /** @brief Number of hands */
} Player_data_t;

/**
 * @brief Represents a deck of cards.
 * @param cards Pointer to array of cards in the deck.
 * @param num_cards Number of cards remaining in the deck.
 * @param total_cards Total number of cards in the deck.
 */
typedef struct {
  card_t *cards;   /** @brief Array of cards in deck */
  int num_cards;   /** @brief Number of cards remaining in deck */
  int total_cards; /** @brief Total number of cards in deck */
} deck_t;          /** @brief Deck structure */

/**@}*/

#endif /* _LCOM_GAME_CONFIG_H */
