#ifndef GAME_CONTROLER_H
#define GAME_CONTROLER_H

#include "game_config.h"

//-------------------- DECK FUNCTIONS --------------------//
/** @brief Generates a new 52-card deck
 *  @return 0 on success, 1 on failure
 */
int game_controller_generate_deck();

/** @brief Shuffles the current deck using Fisher-Yates algorithm
 *  @return 0 on success, 1 on failure
 */
int game_controller_shuffle_deck();

/** @brief Resets deck to initial unshuffled state
 *  @return 0 on success, 1 on failure
 */
int game_controller_reset_deck();
//--------------------------------------------------------//

//-------------------- PLAYER ACTION FUNCTIONS --------------------//
/** @brief Splits a player's hand into two hands
 *  @param player_id The ID of the player
 *  @param hand_index Index of hand to split
 *  @return 0 on success, 1 on failure
 */
int game_controller_split(int player_id, int hand_index);

/** @brief Player stays with current hand
 *  @param player_id The ID of the player
 *  @param hand_index Index of hand
 *  @return 0 on success, 1 on failure
 */
int game_controller_stay(int player_id, int hand_index);

/** @brief Doubles player's bet and deals one more card
 *  @param player_id The ID of the player
 *  @param hand_index Index of hand
 *  @return 0 on success, 1 on failure
 */
int game_controller_double(int player_id, int hand_index);

/** @brief Deals one additional card to player's hand
 *  @param player_id The ID of the player
 *  @param hand_index Index of hand
 *  @return 0 on success, 1 on failure
 */
int game_controller_hit(int player_id, int hand_index);
//-----------------------------------------------------------------//

//--------------------- AUXILIARY FUNCTIONS ---------------------//
/** @brief Initializes game state and allocates memory
 *  @return 0 on success, 1 on failure
 */
int game_controller_init();

/** @brief Places a bet for a specific player
 *  @param player_id The ID of the player placing the bet
 *  @param bet The amount to bet
 *  @param place_on_table Position on table (1-8)
 *  @return 0 on success, 1 on failure
 */
int game_controller_place_bet(int player_id, int bet, int place_on_table);

/** @brief Frees allocated memory and cleans up game state
 *  @return 0 on success, 1 on failure
 */
int game_controller_free();

int game_controller_add_hand(int player_id);

/**
 * @brief Calculates the total value of a hand, handling Aces as 1 or 11.
 * @return The hand value, or -1 on error
 */
int game_controller_calculate_hand_value(hand_t hand);
//---------------------------------------------------------------//

//-------------------- GAME AND TURN ACTION STATE HANDLER -----------------//
/** @brief Handles player action states (bet, hit, stay, etc)
 *  @param player_id The ID of the player
 *  @return 0 on success, 1 on failure
 */
int game_controller_turn_state_handler(int player_id, int hand);

/** @brief Handles the current game state based.
 *  @return 0 on success, 1 on failure
 */
int game_controller_game_state_handler();

int game_controller_Dealer_Turn();
//-------------------------------------------------------------------------//

/** @brief Deals a card to a player or dealer
 *  @return 0 on success, 1 on failure
 */
int game_controller_deal_card(int player_id, int hand_index);

/**
 * @brief Handles payout logic at the end of a round.
 * @return 0 on success, 1 on failure
 */
int game_controller_payout(int player_id);

int game_controller_wining_hand(hand_t *hand);

int game_controller_deal_initial_cards();
int game_controller_redy_for_new_round();

#endif // GAME_CONTROLER_H
