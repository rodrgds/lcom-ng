#ifndef __CARD_H_
#define __CARD_H_

#include <lcom/lcf.h>
#include "./sprites/sprite.h"

/**
 * @defgroup card Card
 * @{
 *
 * Functions and structures for working with playing cards and poker hands
 */

#define DECK_SIZE 52 /**< @brief Size of a standard deck of cards */

/**
 * @brief Enumeration representing the suits of the cards
 */
typedef enum {
    CLUBS,    /**< Clubs suit */
    SPADES,   /**< Spades suit */
    HEARTS,   /**< Hearts suit */
    DIAMONDS, /**< Diamonds suit */
} suit_enum;

/**
 * @brief Enumeration representing the worth of the cards
 */
typedef enum {
    TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
} worth_enum;

/**
 * @brief Structure representing a card
 */
typedef struct Card {
    suit_enum suit;   /**< Suit of the card */
    worth_enum worth; /**< Worth of the card */
} Card;

/**
 * @brief Enumeration representing the rank of hands in a poker game
 */
typedef enum hand_rank_enum {
	HIGH, /**< High card */
    PAIR, /**< One pair */
    FLUSH, /**< Flush */
    STRAIGHT, /**< Straight */
    THREE_OF_A_KIND, /**< Three of a kind */
    STRAIGHT_FLUSH /**< Straight flush */
} hand_rank_enum;

/**
 * @brief Structure representing a hand type in a poker game
 */
typedef struct HandType {
    hand_rank_enum rank; /**< Rank of the hand */
    int values[3]; /**< Values of the hand */
} HandType;

/**
 * @brief Enumeration representing the outcome of a poker game
 */
typedef enum Outcome { 
    WIN, /**< Win */
    LOSE, /**< Lose */
    DRAW /**< Draw */
} Outcome;

/**
 * @brief Creates a standard deck of cards
 *
 * @param deck Pointer to the array to store the deck of cards
 */
void (create_deck)(Card *deck);

/**
 * @brief Shuffles a deck of cards
 *
 * @param deck Pointer to the deck of cards to shuffle
 */
void (shuffle_deck)(Card *deck);

/**
 * @brief Prints the details of a card
 *
 * @param card Pointer to the card to print
 */
void (print_card)(const Card *card);

/**
 * @brief Prints the details of a deck of cards
 *
 * @param deck Pointer to the deck of cards to print
 */
void (print_deck)(const Card *deck);

/**
 * @brief Finds the sprite associated with a card
 *
 * @param card Pointer to the card
 * @return Pointer to the sprite associated with the card
 */
Sprite *(find_card_sprite)(const Card *card);

/**
 * @brief Draws a card at the specified coordinates
 *
 * @param card Pointer to the card to draw
 * @param x X coordinate of the card
 * @param y Y coordinate of the card
 */
void (draw_card)(const Card *card, uint32_t x, uint32_t y);

/**
 * @brief Determines the rank of a hand of cards
 *
 * @param cards Array of cards representing the hand
 * @param hand Pointer to the structure to store the hand rank
 */
void (determine_hand)(Card cards[3], HandType *hand);

/**
 * @brief Evaluates the type of hand based on its characteristics
 *
 * @param hand Pointer to the hand to evaluate
 * @param values Array of values representing the hand
 * @param flush Indicates whether the hand is a flush
 * @param straight Indicates whether the hand is a straight
 * @param duplicates Number of duplicate cards in the hand
 * @param duplicated Value of the duplicated card
 */
void (evaluate_hand_type)(HandType *hand, int values[3], bool flush, bool straight, int duplicates, int duplicated);

/**
 * @brief Creates a hand type based on the specified parameters
 *
 * @param hand Pointer to the hand type structure to create
 * @param rank Rank of the hand
 * @param values Array of values representing the hand
 * @param index Index of the hand type
 */
void (create_hand_type)(HandType *hand, hand_rank_enum rank, int values[3], int index);

/**
 * @brief Creates a hand type for a pair
 *
 * @param values Array of values representing the hand
 * @return The created hand type
 */
struct HandType (create_pair_type)(int values[3]);

/**
 * @brief Determines the outcome of a poker game based on the hands of the player and the house
 *
 * @param playerHand Hand of the player
 * @param houseHand Hand of the house
 * @return The outcome of the game
 */
Outcome (winning_hand)(HandType playerHand, HandType houseHand);

/**
 * @brief Calculates the score of a hand based on its rank
 *
 * @param hand Pointer to the hand type structure
 * @return The score of the hand
 */
int (hand_rank_score)(const HandType *hand);

/**
 * @brief Converts a hand type to a string representation
 *
 * @param hand Hand type to convert
 * @return Pointer to the string representation of the hand type
 */
char *(handtype_to_string)(HandType hand);

/** @} */

#endif /* __CARD_H_ */
