#ifndef _SUN_H_
#define _SUN_H_

#include <stdint.h>
#include <stdbool.h>

#include "src/include.h"
#include "src/Model/Position.h"

/**
 * @file Sun.h
 * @brief Sun structure and related functions.
 *
 * Declaration of the Sun structure, representing
 * the sun in the game, with functions for
 * creating, destroying, and accessing its position.
 */

 /**
 * @struct Sun
 * @brief Represents the sun object.
 *
 * Stores the position and dimensions of the sun.
 */

typedef struct {
    Position position;
    int16_t width, height;
} Sun;

/**
 * @brief Creates a new Sun instance.
 *
 * @param position Initial position of the sun.
 * @param width Width of the sun.
 * @param height Height of the sun.
 * @return Pointer to a newly allocated Sun, or NULL on failure.
 */

Sun *new_Sun(Position position, int16_t width, int16_t height);

/**
 * @brief Frees memory associated with a Sun instance.
 *
 * @param sn Pointer to the Sun to be deleted.
 */

void delete_Sun(Sun *sn);

/**
 * @brief Retrieves current position of the Sun.
 *
 * @param sn Pointer to the Sun instance.
 * @return Current position of the sun.
 */

Position get_Sun_Position(Sun *sn);

#endif
