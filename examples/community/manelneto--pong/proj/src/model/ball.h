#ifndef _BALL_H_
#define _BALL_H_

#include <stdint.h>

#include "sprite.h"

#include "wall.h"

/** @defgroup ball ball
 * @{
 *
 * Functions related to the ball
 */

/**
 * @brief Struct for the ball
 */
typedef struct {
  int16_t x;      /*!< horizontal coordinate of the ball */
  int16_t y;      /*!< vertical coordinate of the ball */
  int8_t vx;      /*!< horizontal velocity of the ball */
  int8_t vy;      /*!< vertical velocity of the ball */
  Sprite *sprite; /*!< sprite of the ball*/
} Ball;

/**
 * @brief Ball constructor
 *
 * @param x horizontal coordinate of the ball
 * @param y vertical coordinate of the ball
 * @param vx horizontal velocity of the ball
 * @param vy vertical velocity of the ball
 * @return Constructed ball on success, NULL otherwise
 */
Ball *construct_ball(int16_t x, int16_t y, int8_t vx, int8_t vy);

/**
 * @brief Updates ball position and velocity
 *
 * @param ball pointer to the ball to be updated/moved
 * @param x_max maximum horizontal coordinate of the ball (screen limit/boundary)
 * @param y_max maximum vertical coordinate of the ball (screen limit/boundary)
 * @return Return 0 upon success and non-zero otherwise
 */
int move_ball(Ball *ball, uint16_t x_max, uint16_t y_max);

/**
 * @brief Updates ball position and velocity after collision with wall
 *
 * @param ball pointer to the ball to be updated/moved
 * @param wall pointer to the wall that the ball collided with
 * @return Return 0 upon success and non-zero otherwise
 */
int move_ball_after_collision_with_wall(Ball *ball, Wall *wall);

/**
 * @brief Doubles ball velocity (in both directions, horizontal and vertical)
 *
 * @param ball pointer to the ball to be updated
 * @return Return 0 upon success and non-zero otherwise
 */
int speedup_ball(Ball *ball);

/**
 * @brief Ball destructor
 *
 * @param ball pointer to the ball to be destroyed
 */
void destroy_ball(Ball *ball);

/**@}*/

#endif /* _BALL_H_ */
