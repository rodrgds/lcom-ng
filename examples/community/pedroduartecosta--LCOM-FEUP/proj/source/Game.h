#ifndef __GAME_H
#define __GAME_H

#include <stdio.h>
#include "MainMenuState.h"

/** @defgroup Game Game
 * @{
 * Struct containing all the information of the game and its states
 */

typedef enum{
	MAIN_MENU_STATE, GAME_STATE, FINAL_STATE
}State;


typedef struct{

	int set_kb;
	int set_mouse;
	int set_timer;
	int counter;
	State currentState;
	void* state;
	unsigned long scancode;
	int mainScore;


}Game;

/**
 * @brief Creates a new game
 *
 * @return Non NULL pointer to the game
 */

Game* startGame();

/**
 * @brief updates the game
 *
 * @param game to update
 *
 * @return int telling if the player wants to close the game
 */
int updateGame(Game* game);
/**
 * @brief draws the game
 *
 * @param game to be drawn
 */
void drawGame(Game* game);
/**
 * @brief deletes one of the game states
 *
 * @param game
 */
void deleteState(Game* game);
/**
 * @brief deletes all game states
 *
 * @param game to be deleted
 */
void deleteGame(Game* game);

/**@}*/

#endif
