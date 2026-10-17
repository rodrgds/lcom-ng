#ifndef COIN_MODEL_H
#define COIN_MODEL_H

#include "../position.h"

/**
 * @brief Small coins that are placed along the maze.
*/
typedef struct {
    Position pos;
    bool isCaptured;
} SmallCoin;

/**
 * @brief Special coins that increase the game time when captured.
 */
typedef struct {
    Position pos;
    bool isCaptured;
} BigCoin;

/**
 * @brief Set the initial position of all small coins.
 * 
 */
void (set_small_coins_positions)();


/**
 * @brief Set the initial position of all big coins.
 * 
 */
void (set_big_coins_position)();

#endif
