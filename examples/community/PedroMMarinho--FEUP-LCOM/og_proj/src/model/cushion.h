#pragma once
#include <lcom/lcf.h>
#include "vector.h"

struct Ball;
/**
 * @brief Struct that represents the linear cushion
*/
typedef struct LinearCushion{

  vector_t p1;
  vector_t p2;

  double lx;
  double ly;
  double l0;

  vector_t normal;

}LinearCushion;
/**
 * @brief Struct that represents the circular cushion
*/

typedef struct CircularCushion{

  vector_t position;
  double radius;

} CircularCushion;

/**
 * @brief Creates a new LinearCushion object.
 *
 * This function allocates memory for a new LinearCushion object and initializes it with the given points.
 *
 * @param p1 The first point defining the linear cushion.
 * @param p2 The second point defining the linear cushion.
 * @return LinearCushion* Pointer to the newly created LinearCushion object.
 */

LinearCushion* newLinearCushion(vector_t p1, vector_t p2);

/**
 * @brief Fixes the normal direction of the LinearCushion.
 *
 * This function adjusts the normal vector of the linear cushion to ensure it points in the correct direction relative to the ball's velocity.
 *
 * @param cushion Pointer to the LinearCushion object.
 * @param ball Pointer to the Ball object.
 */

void fixNormalDirection(LinearCushion* cushion,struct Ball* ball);

/**
 * @brief Creates a new CircularCushion object.
 *
 * This function allocates memory for a new CircularCushion object and initializes it with the given position and radius.
 *
 * @param pos The position of the circular cushion.
 * @param radius The radius of the circular cushion.
 * @return CircularCushion* Pointer to the newly created CircularCushion object.
 */

CircularCushion* newCircularCushion(vector_t pos, double radius);

/**
 * @brief Computes the normal vector for a CircularCushion at the point of contact with a Ball.
 *
 * This function calculates the normal vector from the ball's position to the circular cushion's position.
 *
 * @param cushion Pointer to the CircularCushion object.
 * @param ball Pointer to the Ball object.
 * @return vector_t The normalized vector representing the normal at the point of contact.
 */
vector_t CircularCushionNormal(CircularCushion* cushion,struct Ball* ball);
