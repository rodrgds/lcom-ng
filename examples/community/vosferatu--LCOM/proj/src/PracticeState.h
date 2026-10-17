#ifndef __PRACTICESTATE_H
#define __PRACTICESTATE_H

#include "bitmap.h"
#include "rectangle.h"
#include "cowboy.h"
#include "target.h"
#include "shield.h"
#include "bullet.h"
#include "table.h"

/** @defgroup PracticeState PracticeState
 * @{
 *
 * Functions for controlling PracticeState
 */

/*! Represents the PracticeState */
typedef struct{
	int done;
	int score;

	int mouseOnReady;
	Rectangle* readyButton;
	Rectangle* fireButton;
	Bitmap* background;
	Bitmap* board;

	Cowboy* cowboy;
	Target* target;
	Shield* shield;
	Bullet* bullet;
	Table* scoretable;



} PracticeState;

/**
 * @brief Creates a new PracticeState; allocates it
 * @return a pointer to the struct PracticeState created
 */
PracticeState* newPracticeState();

/**
 * @brief updates elements of the PracticeState
 * @param state a pointer to PracticeState
 * @param scancode key pressed by user (if any)
 */
void updatePracticeState(PracticeState* state, unsigned long scancode);

/**
 * @brief draws bitmaps of the PracticeState and other struct elements
 * @param state a pointer to PracticeState
 */
void drawPracticeState(PracticeState* state);

/**
 * @brief deletes Practicestate passed and frees from memory
 * @param state a pointer to PracticeState
 */
void deletePracticeState(PracticeState* state);

/**
 * @brief makes a player fire a bullet
 * @param state a pointer to PracticeState
 */
void playerFire(PracticeState* state);

/**
 * @brief check collisions of a bullet (top screen, bottom screen, shield, targed, right screen)
 * @param state a pointer to PracticeState
 */
void checkCollisions(PracticeState* state);

/** @} */

#endif /* __PRACTICESTATE_H */
