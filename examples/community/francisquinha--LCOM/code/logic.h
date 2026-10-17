#ifndef __LOGIC_H
#define __LOGIC_H

/** @file logic.h
 * @brief file containing declarations of the functions that deal with the game logic
 */

/** @defgroup logic logic
 * @{
 *
 * @brief functions to handle the game logic
 */

#include "game.h"

/**
 * @brief checks if the three cards selected in the game form a set
 * @param game to check for a set
 * @return 1 if it is not a set, 0 otherwise
 */
unsigned short check_set(Game *game);

/**
 * @brief checks if there are no sets in display in the game
 * @param game to check for no sets
 * @return 1 if there are sets, 0 otherwise
 */
unsigned short check_no_sets(Game *game);

/**
 * @brief function to obtain a hint for a set in display
 * @param game to look for a set
 * @return 1 if there is no set, 0 otherwise
 */
unsigned short get_set(Game *game);

/** @} end of logic */

#endif /* __LOGIC_H */
