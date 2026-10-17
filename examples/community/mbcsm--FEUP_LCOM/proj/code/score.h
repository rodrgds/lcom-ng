/** @defgroup score score
 * @{
 *
 * utils for score
 */
#ifndef SCORE_H
#define SCORE_H

#include "rtc.h"
#include <stdio.h>


typedef struct 
{
    int scoreOne;
    char * scoreOneDate;
    int scoreTwo;
    char * scoreTwoDate;
    int scoreThree;
    char * scoreThreeDate;
    int scoreFour;
    char * scoreFourDate;
    int scoreFive;
    char * scoreFiveDate;

} Scores;

Scores s;

/**
 * @brief Loads scores from file to a struct, recording the best 5 scores
 */
void loadScores();

/**
 * @brief Saves scores to a file, recording the best 5 scores
 */
void saveScores();

/**
 * @brief adds new score if it is better than the currenctly saved ones
 */
void addNewScore(int score);



#endif

/**@}*/
