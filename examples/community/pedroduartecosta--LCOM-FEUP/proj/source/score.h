#ifndef __SCORE_H
#define __SCORE_H

#include "Bitmap.h"


/** @defgroup Score Score
 * @{
 * functions regarding the score
 */

typedef struct{
	unsigned int score;
	Bitmap* score1;
	Bitmap* score2;
	Bitmap* score3;
	Bitmap* score4;
	Bitmap* score5;
	Bitmap* score6;
	Bitmap* score7;
	Bitmap* score8;
	Bitmap* score9;
	Bitmap* score0;
	Bitmap* scoreBoard;
}Score;

/**
 * @brief creates a new score and loads it's bitmaps
 *
 * @return Null pointer to the new score
 */

Score* newScore();

/**
 * @brief updates the score
 *
 * @param score
 * @param int telling if the player caught a pokeball
 */
void updateScore(Score* s,int hitPoke);//hitPoke=0 se nao atingiu e hitPoke=1 atingiu pokeball

/**
 * @brief draws score on the screen
 *
 * @param score
 */
void drawScore(Score* s);

/**
 * @brief returns the score value
 *
 * @param score
 * @return int containing the final score
 */
unsigned int returnScore(Score* s);

/**
 * @brief deletes a player score
 *
 * @param score to be deleted
 */
void deleteScore(Score* s);

/**@}*/
#endif
