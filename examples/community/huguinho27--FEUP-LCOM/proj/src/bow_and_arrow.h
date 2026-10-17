#ifndef BOW_AND_ARROW_H
#define BOW_AND_ARROW_H
#include "hero.h"

/** @defgroup Bow and Arrow Bow and Arrow
 * @{
 * Functions for manipulating game loops
 */

/**
 * @brief Show an image indicating the game is now in second stage
 */
void showSecondLevelScreen();

/**
 * @brief If the hero wins, will show a winning screen
 */
void showHeroWinScreen();

/**
 * @brief If the hero loses, will show a losing screen
 */
void showHeroLoseScreen();

/**
 * @brief Makes the bitmap associated with arrow number show on screen
 */
void showArrowNumber();

/**
 * @brief Makes the bitmap associated with the score show on screen
 */
void showScore();

/**
 * @brief Checks if there are any intact balloons flying around
 * in the first stage of the game
 * @return 1 on true, 0 otherwise
 */
int checkIfAnyBalloonIntact_firstlevel();

/**
 * @brief Checks if there are any intact balloons flying around
 * in the second stage of the game
 * @return 1 on true, 0 otherwise
 */
int checkIfAnyBalloonIntact_secondlevel();

/**
 * @brief Makes all balloons go blown on the first level.
 * This function is for testing purposes only
 */
void blowAllBalloonsFirstLevel();

/**
 * @brief Makes all balloons go blown on the second level.
 * This function is for testing purposes only
 */
void blowAllBalloonsSecondLevel();

/**
 * @brief Blows balloons whether the stage is first or second
 */
void blowCurrentLevelBalloons();

/**
 * @brief Checks if all balloons are destroyed on the first stage
 * @return 1 on true, 0 otherwise
 */
int checkAllBalloonsDestroyed_firstlevel();

/**
 * @brief Checks if all balloons are destroyed on the second stage
 * @return 1 on true, 0 otherwise
 */
int checkAllBalloonsDestroyed_secondlevel();

/**
 * @brief Allocates memory and create
 * balloons for the first stage of the game
 */
void initializeBalloons_firstLevel();

/**
 * @brief Allocates memory and create
 * balloons for the second stage of the game
 */
void initializeBalloons_secondLevel();

/**
 * @brief Frees the memory from all balloons
 */
void freeMemoryFromBalloons();

/**
 * @brief Frees the memory from all arrows
 */
void freeMemoryFromArrows();

/**
 * @brief This fucntion handles collision detection
 * @return 1(true) if the point of the arrow is inside
 * the red area of the balloon, 0 otherwise
 */
int isAnArrowInsideBalloonArea(int areaX, int areaY, int pointX, int pointY);

/**
 * @brief Checks if there are any arrows inside the screen
 * on the first stage of the game
 * @return 1 on true, 0 otherwise
 */
int checkIfAnyArrowInsideScreen_firstlevel();

/**
 * @brief Checks if there are any arrows inside the screen
 * on the second stage of the game
 * @return 1 on true, 0 otherwise
 */
int checkIfAnyArrowInsideScreen_secondlevel();

/**
 * @brief Adds a new arrow to the array of arrows
 * @param h The hero
 */
void addNewArrowToArray(hero_t *h);

/**
 * @brief Checks the number of arrows available on the hero
 * and prints it on screen
 * @param h The hero
 */
void updateArrowNumber(hero_t *h);

/**
 * @brief Handles the collision detection of arrows and balloons
 * @param h The hero
 */
void collisionDetection(hero_t *h);

/**
 * @brief Checks the score of the hero
 * and prints it on screen
 * @param h The hero
 */
void updateScore(hero_t *h);

/**
 * @brief Checks if the game is lost
 * @return 1 on true, 0 otherwise
 * @param h the hero
 */
int checkIfLoseGame(hero_t *h);

/**
 * @brief Checks if we need to change
 * to second stage of the game
 * @param h The hero
 */
void checkNextLevel(hero_t *h);

/**
 * @brief Handles all events related to mouse movement
 * @param h The hero
 */
void mouseMovement(hero_t *h);

/**
 * @brief Handles all events related to updating screen
 * every interruption of the timer
 * @param h The hero
 */
void updateScreen(hero_t *h);

/**
 * @brief Handles all events related to keyboard inputs
 * @param h The hero
 */
int keyboardUpdate(hero_t *h);

/**
 * @brief Resets all global variables
 * and prepares values for the game
 */
void initializeGameSettings();

/**
 * @brief Main function for the game.
 * Controls all interrupts of the peripherals and calls auxiliary functions
 * @return Always returns 0
 */
int gameLoop();

#endif /**@}*/
