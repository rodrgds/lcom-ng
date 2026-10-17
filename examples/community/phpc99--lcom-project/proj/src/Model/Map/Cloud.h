#ifndef _CLOUD_H_
#define _CLOUD_H_

#include <stdint.h>
#include <stdbool.h>

#include "src/include.h"
#include "src/Model/Position.h"

/**
 * @file Cloud.h
 * @brief Cloud structure and its related functions.
 *
 * Defines the Cloud structure,
 * along with functions to create, destroy and retrieve their position.
 */

/**
 * @struct Cloud
 * @brief Represents a cloud object.
 *
 * Contains position and size information for rendering the cloud.
 */

typedef struct {
    Position position;
    int16_t width, height;
} Cloud;

/**
 * @brief Creates a new Cloud instance.
 *
 * @param position Initial position of the cloud.
 * @param width Width of the cloud.
 * @param height Height of the cloud.
 * @return Pointer to the newly created Cloud, or NULL on failure.
 */

Cloud *new_Cloud(Position position, int16_t width, int16_t height);

/**
 * @brief Frees memory associated with a Cloud instance.
 *
 * @param cl Pointer to the Cloud to be deleted.
 */

 void delete_Cloud(Cloud *cl);

/**
 * @brief Gets the current position of a Cloud.
 *
 * @param cl Pointer to the Cloud instance.
 * @return The position of the cloud.
 */

Position get_Cloud_Position(Cloud *cl);

#endif
