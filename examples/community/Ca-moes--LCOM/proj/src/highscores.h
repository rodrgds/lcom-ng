#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <lcom/lcf.h>

#include "game.h"

/**
 * @brief Load top 5 highscores from file. If no file exists create one
 * 
 * @param game pointer 
*/
int get_highscores(Game * game);


/**
 * @brief Check if the new score is better that the ones in the highscoresr
 * 
 * @param game pointer
 * @return index to insert the new highscore or NUMBEROFSCORES if it is not an highscore
 */
int new_highscore(Game * game, int speed);

/**
 * @brief Save the highscores in a file
 * 
 * @param game pointer
 */
void set_highscores(Game * game);


/**
 * @brief Add new highscore to highscores
 * 
 * @param game pointer
 * @param pos index of the highscores array to insert new highscore
 * @param speed The number of letters written per second in the last game
 */
void add_new_highscore(Game ** game, unsigned int pos,int speed,realTime * dt);

#endif /* HIGHSCORES_H */
