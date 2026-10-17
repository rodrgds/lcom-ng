
#ifndef __GAME_H
#define __GAME_H

/** @file game.h */

/**
 *  @brief Initiates the state machine and calls the menu function.
 *  @param file Path to the scores' file.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int init_game(char *file);

/**
 *  @brief Displays the game menu.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int game_menu();

/**
 *  @brief Starts the game on easy mode.
 *  @returns Returns 0 if successful and 1 otherwise.
 */
int game_mode_easy();

/**
 *  @brief Starts the game on hard mode.
 *  @returns Returns 0 if successful and 1 otherwise.
 */
int game_mode_hard();

#endif
