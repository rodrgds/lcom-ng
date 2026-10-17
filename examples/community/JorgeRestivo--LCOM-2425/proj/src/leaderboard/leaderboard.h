/**
 * @file leaderboard.h
 * @brief Leaderboard management functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include "proj.h"

/**
 * @brief Reads the leaderboard from file and updates the game structure.
 * @param game The Game structure to update with leaderboard values.
 */
void read_leaderboard(Game game);

/**
 * @brief Checks if the current score should be added to the leaderboard.
 * @param game The Game structure containing the current score and leaderboard.
 * @return 0 if the score was added to the leaderboard, 1 otherwise.
 */
int check_score(Game game);

/**
 * @brief Writes the current leaderboard to file.
 * @param game The Game structure containing the leaderboard to write.
 */
void write_leaderboard(Game game);
