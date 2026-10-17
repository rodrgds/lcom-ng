#ifndef SCORE_H
#define SCORE_H

#include "rtc.h"

/** @defgroup Score Score
 * @{
 */

typedef  struct score_t{
  char letters[3];
  int value;
  Date data;
} Score;

#define SCOREX          50
#define SCOREY          225
#define SCOREDISPSIZE   75

Score highscores[3];

/*
@brief Creates a Score variable with a value of val

@param val    Score value

@return Score variablr
*/
Score score(int val);

/*
@brief Returns the biggest of two Scores compairing their value

@param s1     ptr to first score to be compared

@param s2     ptr to second score to be compared

@return Highest Score of the two
*/
Score maxScore(Score *s1, Score *s2);

/*
@brief Increases the value of letter[0] in the given score

@param s1   ptr to Score to change
*/
void upLetterZero(Score *s1);

/*
@brief Decreases the value of letter[0] in the given score

@param s1   ptr to Score to change
*/
void downLetterZero(Score *s1);

/*
@brief Increases the value of letter[1] in the given score

@param s1   ptr to Score to change
*/
void upLetterOne(Score *s1);

/*
@brief Decreases the value of letter[1] in the given score

@param s1   ptr to Score to change
*/
void downLetterOne(Score *s1);

/*
@brief Increases the value of letter[2] in the given score

@param s1   ptr to Score to change
*/
void upLetterTwo(Score * s1);

/*
@brief Decreases the value of letter[2] in the given score

@param s1   ptr to Score to change
*/
void downLetterTwo(Score *s1);

/*
@brief Sets the values in s1 to the other parameters

@param s1 score to be set

@param l1 new letter[0] of score

@param l2 new letter[1] of score

@param l3 new letter[2] of score

@param value new value
*/
void setScore(Score *s1, char l1, char l2, char l3, int value);


/*
@brief Copies the values of one score to the other

@param s1   ptr to destiny score

@param s2   ptr to src score
*/
void copy(Score *s1, Score *s2);

/*
@brief Gets the HighScores from the file
*/
void readHighScoresFromFile();

/*
@brief Writes the HighScores to the file
*/
void writeHighScoresToFile();

/*
@brief Checks if a new HS is set

@param s1   New Score

@return True if so
*/
bool newHS(Score * s1);

/*
@brief Given a Score updates the HighScores Array accordingly

@param s1   Given Score
*/
void updateHighScores(Score *s1);

#endif
