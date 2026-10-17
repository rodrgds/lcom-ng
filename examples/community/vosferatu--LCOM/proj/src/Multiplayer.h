#ifndef __MULTIPLAYER_H
#define __MULTIPLAYER_H

#include "bitmap.h"
#include "rectangle.h"
#include "cowboy.h"
#include "target.h"
#include "shield.h"
#include "bullet.h"
#include "table.h"

/** @defgroup Multiplayer Multiplayer
 * @{
 *
 * Functions for controlling Multiplayer state
 */

/*! Represents the Multyplayer State */
typedef struct{
	int done;
	Bitmap* background;
	Bitmap* board;

	Cowboy* player1Cowboy;
	Cowboy* player2Cowboy;

	int level;

	Shield* shield;
	Bullet* player1Bullet;
	Bullet* player2Bullet;

	Table* scoretable;

} Multiplayer;


/**
 * @brief Creates a new Multiplayer state; ; allocates it
 * @return a pointer to the struct Multiplayer created
 */
Multiplayer* newMultiplayer();

/**
 * @brief updates elements of the Multiplayer State
 * @param state a pointer to Multiplayer  state
 * @param scancode key pressed by users (if any)
 */
void updateMultiplayer(Multiplayer* state, unsigned long scancode);

/**
 * @brief draws bitmaps of the Multiplayer and other struct elements
 * @param state a pointer to Multiplayer state
 */
void drawMultiplayer(Multiplayer* state);

/**
 * @brief deletes Multiplayer  state passed and frees from memory
 * @param state a pointer to Multiplayer state
 */
void deleteMultiplayer(Multiplayer* state);


/**
 * @brief check player 1's bullet colisions (if there is a bullet)
 * @param state a pointer to Multiplayer state
 */
void player1Collisions(Multiplayer* state);

/**
 * @brief creates a bullet form the player 1's input
 * @param state a pointer to Multiplayer state
 */
void player1Fire(Multiplayer* state, int direction);

/**
 * @brief creates a bullet form the player 2's input
 * @param state a pointer to Multiplayer state
 */
void player2Fire(Multiplayer* state, int direction);

/**
 * @brief check player 2's bullet colisions (if there is a bullet)
 * @param state a pointer to Multiplayer state
 */
void player2Collisions(Multiplayer* state);

/** @} */

#endif /* __MULTIPLAYER_H */
