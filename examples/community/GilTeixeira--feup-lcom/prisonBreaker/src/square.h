#pragma once

#include "bitmap.h"

/**
 * @brief Square Display Settings
 */

typedef enum {
	DISTANCE_TO_MOVE = 100,
	SQUARE_X_COORD = 366,
	SQUARE_Y_COORD = 268,
	SQUARE_MAX_MOVE = 120,
	SQUARE_DELTA_MOVE = 4,

} SquareDisplaySettings;

/**
 * @struct Square
 * @brief This structure holds the information about the game Main Object
 */
typedef struct {
	short direction;
	Bitmap* bitmap;

	short deslocX, deslocY;

} Square;

/**
 * @brief  Initializes the Square
 *
 * @return The new Square
 */
Square* initSquare();

/**
 * @brief  Updates the Square
 *
 * @param Square The Square to be updated
 */
void updateSquare(Square* square);

/**
 * @brief Check if square has return to its initial position
 *
 * @param square The square to be tested
 *
 * @return 1 if has finished, 0 if has not
 */
short hasFinishedMovement(Square* square);

/**
 * @brief Display the Square
 *
 * @param Square The Square to be displayed
 */
void displaySquare(Square* square);

/**
 * @brief  Free the Square and all the resources used by it
 *
 * @param square The Square to be freed
 */
void freeSquare(Square* square);
