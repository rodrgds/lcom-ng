#ifndef SCORE_H
#define SCORE_H

extern int survival_score;

/**
 * @brief Resets the survival score to zero
 */
void score_reset();

/**
 * @brief Increments the survival score by one
 */
void score_increment();

/**
 * @brief Retrieves the current survival score
 * 
 * @return (int) The current value of the survival score
 */
int get_score();

#endif
