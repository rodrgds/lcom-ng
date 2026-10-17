#ifndef _DYNAMITE_H_
#define _DYNAMITE_H_

#include <lcom/lcf.h>

#include "position.h"

#define DYNAMITE_X 106
#define DYNAMITE_Y 109


/**
 * @struct Dynamite
 * @brief Represents a dynamite object in the game.
 *
 * The Dynamite struct contains information about the position, activity status,
 * direction, and explosion frame of a dynamite object.
 */
typedef struct {
    Position pos;           /** The position of the dynamite */
    bool active;            /** Indicates if the dynamite is active */
    Direction dir;          /** The direction of the dynamite */
    int explosionFrame;     /** The current explosion frame of the dynamite */
} Dynamite;

/**
 * @brief Creates a Dynamite object with the specified coordinates and direction.
 *
 * This function creates a Dynamite object with the given x and y coordinates and sets its direction.
 *
 * @param x The x-coordinate of the Dynamite object.
 * @param y The y-coordinate of the Dynamite object.
 * @param dir The direction of the Dynamite object.
 *
 * @return A pointer to the created Dynamite object.
 */
Dynamite* (createDynamite)(int x, int y, Direction dir);

/**
 * @brief Retrieves the x-coordinate of a Dynamite object.
 *
 * This function returns the x-coordinate of the specified Dynamite object.
 *
 * @param dynamite A pointer to the Dynamite object.
 *
 * @return The x-coordinate of the Dynamite object.
 */
int16_t (getDynamiteX)(Dynamite* dynamite);

/**
 * @brief Retrieves the y-coordinate of a Dynamite object.
 *
 * This function returns the y-coordinate of the specified Dynamite object.
 *
 * @param dynamite A pointer to the Dynamite object.
 *
 * @return The y-coordinate of the Dynamite object.
 */
int16_t (getDynamiteY)(Dynamite* dynamite);

/**
 * @brief Destroys a Dynamite object.
 *
 * This function frees the memory allocated for the specified Dynamite object.
 *
 * @param dynamite A pointer to the Dynamite object.
 */
void (destroyDynamite)(Dynamite* dynamite);

#endif
