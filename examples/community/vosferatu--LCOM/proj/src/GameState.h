#ifndef __GAMESTATE_H
#define __GAMESTATE_H

#include "bitmap.h"
#include "rectangle.h"
#include "cowboy.h"
#include "target.h"
#include "shield.h"
#include "bullet.h"
#include "table.h"

/** @defgroup GameState GameState
 * @{
 *
 * Functions for controlling GameState
 */

/*! Represents the GameState */
typedef struct{
	int done;
	Bitmap* background;
	Bitmap* board;
	Bitmap* gameWon;

	Cowboy* playerCowboy;
	Cowboy* pcCowboy;

	Rectangle* fireButton;
	Shield* shield;
	int level;
	int ticks;
	Bullet* playerBullet;
	Bullet* pcBullet;

	Table* scoretable;
	Table* countdown;

} GameState;


/**
 * @brief Creates a new GameState; ; allocates it
 * @return a pointer to the struct GameState created
 */
GameState* newGameState();

/**
 * @brief updates elements of the GameState
 * @param state a pointer to GameState
 * @param scancode key pressed by user (if any)
 */
void updateGameState(GameState* state, unsigned long scancode);

/**
 * @brief draws bitmaps of the GameState and other struct elements
 * @param state a pointer to GameState
 */
void drawGameState(GameState* state);

/**
 * @brief deletes Gamestate passed and frees from memory
 * @param state a pointer to GameState
 */
void deleteGameState(GameState* state);

/**
 * @brief updates the player position/fire through scancode/mouse (able to exit)
 * @param state a pointer to GameState
 * @param scancode key pressed by user (if any)
 */
void updatePlayer(GameState* state, unsigned long scancode);

/**
 * @brief check player's bullet colisions (if there is a bullet)
 * @param state a pointer to GameState
 */
void checkPlayerCollisions(GameState* state);

/**
 * @brief creates a bullet form the players input
 * @param state a pointer to GameState
 */
void playerFireGun(GameState* state);

/**
 * @brief updates "AI" player position/fire through gamestate data
 * @param state a pointer to GameState
 */
void updatePC(GameState* state);

/**
 * @brief fires "AI" player gun through gamestate data
 * @param state a pointer to GameState
 */
void pcFire(GameState* state);

/**
 * @brief checks "AI" player's bullet collisions
 * @param state a pointer to GameState
 */
void pcCollisions(GameState* state);

/** @} */

#endif /* __GAMESTATE_H */
