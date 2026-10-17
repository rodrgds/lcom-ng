#ifndef _MOON_H_
#define _MOON_H_

#include <stdint.h>
#include <stdbool.h>

#include "src/include.h"
#include "src/Model/Position.h"

/**
 * @file Moon.h
 * @brief Moon structure and related functions.
 *
 * Defines Moon structure used to represent the moon,
 * as well as functions to create, delete, e get its position.
 */

/**
 * @struct Moon
 * @brief Represents the moon object in the graphical environment.
 *
 * Stores the position and size (width and height) of the moon.
 */

typedef struct {
    Position position;
    int16_t width, height;
} Moon;

/**
 * @brief Creates a new Moon instance.
 *
 * @param position Initial position of the moon.
 * @param width Width of the moon.
 * @param height Height of the moon.
 * @return Pointer to a dynamically allocated Moon object, or NULL if allocation fails.
 */

Moon *new_Moon(Position position, int16_t width, int16_t height);

/**
 * @brief Frees memory associated with a Moon instance.
 *
 * @param mn Pointer to the Moon be deleted.
 */

void delete_Moon(Moon *mn);

/**
 * @brief Gets current position of the Moon.
 *
 * @param mn Pointer to the Moon instance.
 * @return The position of the moon.
 */

Position get_Moon_Position(Moon *mn);

#endif
