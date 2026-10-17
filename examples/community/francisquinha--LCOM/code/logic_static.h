#ifndef __LOGIC_STATIC_H
#define __LOGIC_STATIC_H

/** @file logic_static.h
 * @brief file containing declarations of the static functions in logic.c
 */

/** @defgroup logic_static logic_static
 * @{
 *
 * @brief static functions to handle the game logic
 */

/**
 * @brief computes the specs of a game card given its index
 * first spec is the color: 0 - red, 1 - green, 2 - blue
 * second spec is the symbol: 0 - diamond, 1 - oval, 2 - squiggle
 * third spec is the number: 0 - one, 1 - two, 2 - three
 * fourth spec is the shadding: 0 - open, 1 - stripped, 2 - solid
 * @param index of the card (from 0 to 80)
 * @param specs array of unsigned shorts with the specs of a card given its index
 */
static void get_specs(unsigned short index, unsigned short specs[4]);

/**
 * @brief given the specs of three cards, checks if they form a set
 * @param specs0 specs of the first card
 * @param specs1 specs of the second card
 * @param specs2 specs of the third card
 * @return 0 if the three cards form a set, 1 otherwise
 */
static unsigned short check_specs(unsigned short specs0[4], unsigned short specs1[4], unsigned short specs2[4]);

/**
 * @brief computes the specs of all the cards that are displayed in the game
 * @param game the game to compute the specs
 * @param specs array with all the specs arrays
 */
static void get_all_specs(Game *game, unsigned short specs[21][4]);

/** @} end of logic_static */

#endif /* __LOGIC_STATIC_H */
