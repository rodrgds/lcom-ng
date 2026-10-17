#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include <stdio.h>

/**
 * @brief function that sets up the game
 * 
 * @return int 0 if success, 1 if failure
 */
int before();

/**
 * @brief game loop
 * 
 */
void game();

/**
 * @brief function that cleans up the game
 * 
 * @return int 0 if success, 1 if failure
 */
int after();

#endif
