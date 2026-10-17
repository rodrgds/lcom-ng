#include <stdint.h>
/** @defgroup game game.h
 * @{
 *
 * This file contains the main game functions.
 * 
 */

/**
 * @brief Contains the driver_receive loop, and handles most of the actual game
 * 
 */
void startGame();

/**
 * @brief Exits graphic mode, saves the data, and frees memory
 * 
 */
void endGame();

/**
 * @brief Loads all sprites necessary for the game
 * 
 */
void initStartGame();

/**
 * @brief Saves the data to file.txt
 * 
 */
void saveData();

/**
 * @brief Is called after a player scores a point. Handles the points and checks if is game over
 * 
 * @param playerWhoWon is the player who scored a point
 * @return Returns 1 when one of the players reaches 5 points, and 0 otherwise.
 */
int handlePoints(int playerWhoWon);

/**
 * @brief Reads the max score from a file.
 * 
 */
void readData();
/**@}*/
