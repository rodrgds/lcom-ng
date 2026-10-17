#pragma once


/** @defgroup Button Button
 * @{
 * Struct containing the boundaries of the button
 */
typedef struct{
	int x1,y1,x2,y2;
}Box;

/**
 * @brief creates a new button (boundaries)
 *
 * @param x coordinate of left top corner
 * @param y coordinate of left top corner
 * @param x coordinate of right bottom corner
 * @param y coordinate of left bottom corner
 * @return Non NULL pointer to the new box
 */
Box* newBox(int x1, int y1, int x2, int y2);

/**
 * @brief deletes the box
 *
 * @param box to be deleted
 */
void deleteBox(Box* box);

/**@}*/
