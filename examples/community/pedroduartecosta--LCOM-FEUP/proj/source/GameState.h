#ifndef __GAMESTATE_H
#define __GAMESTATE_H

#include "Bitmap.h"
#include "Player.h"
#include "Collisions.h"
#include "score.h"
#include "Pokeball.h"

#define OBS_SIZE      3
#define POKE_SIZE     3

/** @defgroup GameState GameState
 * @{
 * Struct containing all the information of the gamestate
 */

typedef struct{
	int y, complete,vel;
	Bitmap* background;
	Player* player;
	Score* s;
	Obstacles* obs_l[OBS_SIZE];
	Obstacles* obs_c[OBS_SIZE];
	Obstacles* obs_r[OBS_SIZE];
	Pokeball* poke_l[POKE_SIZE];
	Pokeball* poke_c[POKE_SIZE];
	Pokeball* poke_r[POKE_SIZE];
	unsigned counter; //conta as interrupções
	unsigned sec; //conta os segundos de jogo
} GameState;

/**
 * @brief Creates a new game state
 *
 * @return Non NULL pointer to the new game state
 */


GameState* newGameState();

/**
 * @brief updates the game state
 *
 * @param game state to update
 *
 * @return int telling if the player wants to close the game state countaining the score of the game
 */
int updateGameState(GameState* g,unsigned long scancode,int counter);
/**
 * @brief draws the game state
 *
 * @param game state to be drawn
 */
void drawGameState(GameState* g);

/**
 * @brief deletes the game state
 *
 * @param game state to be deleted
 */
void deleteGameState(GameState* g);

/**
 * @brief moves the background of the game
 *
 * @param game state
 */
void moveBackGround(GameState* g);

/**
 * @brief checks for collisions
 *
 * @param game state
 */
int checkEndGame(GameState *g);
/**
 * @brief moves and creates obstacles
 *
 * @param game state
 */
void updateAllObstacles(GameState* g,int incSpeed);
/**
 * @brief increses the game counter
 *
 * @param game state
 */
void updateCounterGame(GameState* g);
/**
 * @brief moves and creates pokeballs
 *
 * @param game state
 */
void updateAllPokeballs(GameState* g,int incSpeed);
/**
 * @brief checks collisions with pokeballs
 *
 * @param game state
 */
int checkPokeballsCollision(GameState* g);



/**@}*/

#endif
