#pragma once

#include "bitmap.h"

#define numMaxOfAcceptedDirections 5

/**
 * @brief The Levels Possible Instructions
 */
typedef enum {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NOT_LEFT,
	NOT_RIGHT,
	NOT_UP,
	NOT_DOWN,
	NOT_NOT_LEFT,
	NOT_NOT_RIGHT,
	NOT_NOT_UP,
	NOT_NOT_DOWN,
	NOTHING,
	NOT_NOTHING,
	NOT_NOT_NOTHING

} Intruction;


/**
 * @struct Level
 * @brief  This structure holds information about the Level
 */

typedef struct {
	short* acceptedDirections;
	short numAcceptedDirections;

	short instruction;

	short levelID;

	Bitmap* instructionBitmap;

} Level;

/**
 * @brief Displays the Level
 *
 * @param level The Game to be displayed
 */
void displayLevel(Level* level);

/**
 * @brief Checks if the direction is accepted by the level
 *
 * @param level		 The Level
 * @param direction	 The Direction
 *
 * @return 1 if true, 0 if false
 */
short isAcceptedDirection(Level* level, short direction);

/**
 * @brief Free the Games Levels and all the resources used by it
 *
 * @param level The Level
 */
void freeLevel(Level* level);
