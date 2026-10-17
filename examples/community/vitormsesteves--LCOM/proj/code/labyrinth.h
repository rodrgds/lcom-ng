#pragma once

#include "rectangle.h"

/** @defgroup labyrinth labyrinth
 * @{
 * Functions to manipulate a labyrinth
 */

/// Labyrinth definition
typedef struct {
	Rectangle* rec1;
	Rectangle* rec2;
	Rectangle* rec3;
	Rectangle* rec4;
	Rectangle* rec5;
	Rectangle* rec6;
	Rectangle* finish;
} Labyrinth;

/**
 * @brief Creates a new Labyrinth
 *
 * @return Non NULL pointer to the labyrinth
 */
Labyrinth* newLabyrinth();

/**
 * @brief Draws a Labyrinth
 *
 * @param lab Pointer to Labyrinth
 */
void drawLabyrinth(Labyrinth* lab);

/**
 * @brief Verifies if rectangle is inside of the Labyrinth
 *
 * @param state Pointer to Labyrinth
 * @param rect Pointer to Rectangle
 * @return Validation value
 */
int insideLab(Labyrinth* lab, Rectangle* rect);

/**
 * @brief Deletes a Labyrinth
 *
 * @param state Pointer to Labyrinth
 */
void deleteLabyrinth(Labyrinth* lab);
