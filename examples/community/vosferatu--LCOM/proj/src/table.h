#ifndef __TABLE_H
#define __TABLE_H

#include "bitmap.h"
#include "graphics.h"

/** @defgroup table table
 * @{
 * 	table functions
 */

/*! Represents the table */
typedef struct {
	int x, y, w, h;

	Bitmap* number0;
	Bitmap* number1;
	Bitmap* number2;
	Bitmap* number3;
	Bitmap* number4;
	Bitmap* number5;
	Bitmap* number6;
	Bitmap* number7;
	Bitmap* number8;
	Bitmap* number9;
	Bitmap* minus;


	int playerScore, PCScore;
	int opponent;

} Table;

/**
 * @brief creates a new table
 * @param opponent if singleplayer (non-zero) else is 1
 * @return a pointer to a table type
 */
Table* newTable(unsigned int opponent);

/**
 * @brief resets the table(resets scores to 0)
 * @param table a pointer to a table type
 */
void resetTable(Table* table);

/**
 * @brief creates a table according to singleplayer or not
 * @param table a pointer to a table type
 */
void drawTable(Table* table);

/**
 * @brief draws the score on the screen, at x and y position
 * @param table a pointer to a table type
 * @param number number to be drawn
 * @param x coordinate X
 * @param y coordinate Y
 */
void drawNumber(Table* table, int number, int x, int y);

/**
 * @brief frees a table from memory
 * @param table a pointer to a table type
 */
void deleteTable(Table* table);

/**@}*/

#endif /* _TABLE_H */
