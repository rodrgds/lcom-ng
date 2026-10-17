#ifndef __FINALSTATE_H
#define __FINALSTATE_H

#include <stdio.h>
#include "Button.h"
#include "Bitmap.h"
#include "fonts.h"

/** @defgroup FinalState FinalState
 * @{
 * Struct containing all the information of the final state
 */
typedef struct{

	Fonts* f;
	Bitmap* background;
	Bitmap* HighScoresBackground;
	int score;
	int play;
	int scoreTable;
	FILE *file;
	char first[30];
	char second[30];
	char third[30];
	char firstName[30];
	char secondName[30];
	char thirdName[30];
	char firstDate[30];
	char secondDate[30];
	char thirdDate[30];
	char newHighScore[30];
	char newHighScoreName[30];
	char newHighScoreDate[30];
	int flag;

}FinalState;

/**
 * @brief Creates a new final state
 *
 * @return Non NULL pointer final state
 */

FinalState* newFinalState();

/**
 * @brief updates the final state
 *
 * @param state to update
 * @param keyboard scancode
 * @param score
 * @param variable containing the final score of the player
 * @return int telling if the player wants to close the game
 */
int updateFinalState(FinalState* state, unsigned long scancode, int score, int scoreAux);

/**
 * @brief reads from a file the highest scores achivied before
 *
 * @param state to update
 */
void readHighScores(FinalState* state);
/**
 * @brief prints the highest scores
 *
 * @param state to update
 */
void printHighScores(FinalState* state);
/**
 * @brief defines a new high score and saves it to a text file
 *
 * @param state to update
 */
void newHighScore(FinalState* state);

/**
 * @brief verifies if the state there is a need to change to another state
 *
 * @param state to update
 */
int verifyFinalStateChange(FinalState* state);
/**
 * @brief draws the state
 *
 * @param state to update
 */
void drawFinalMenu(FinalState* state);
/**
 * @brief deletes the state
 *
 * @param state to be deleted
 */
void deleteFinalState(FinalState* state);

/**@}*/

#endif
