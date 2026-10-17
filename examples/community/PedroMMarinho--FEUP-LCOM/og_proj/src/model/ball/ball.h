
#pragma once
#include <lcom/lcf.h>
#include "../vector.h"
#include "../event.h"

/**
 * @brief Struct that represents the ball states
*/
typedef enum BALL_STATE {
  STATIONARY,
  SLIDING,
  SPINNING,
  ROLLING,
  POCKETED,
} BALL_STATE;

/**
 * @brief Enum that represents the type of ball
*/
typedef enum BALL_TYPE {
  STRIPED,
  SOLID,
  BLACK,
  WHITE,
} BALL_TYPE;

/**
 * @brief Struct that represents the ball
*/
typedef struct Ball {
  BALL_STATE state;
  vector_t position;
  vector_t velocity;
  vector3_t ang_velocity;
  xpm_image_t img;
  size_t radius;
  BALL_TYPE type;
  Event* transition;
} Ball;

/**
 * @brief Creates a new ball
 * 
 * @param position 
 * @param img 
 * @param type 
 * @return Ball* 
*/
Ball *newBall(vector_t position,xpm_image_t img,BALL_TYPE type);

/**
 * @brief Destroys a Ball object.
 *
 * This function frees the memory allocated for the transition attribute of the Ball object.
 *
 * @param ball Pointer to the Ball object to be destroyed.
 */

void destroyBall(Ball *ball);

/**
 * @brief Gets the position of the Ball.
 *
 * This function returns the position vector of the specified Ball object.
 *
 * @param ball Pointer to the Ball object.
 * @return vector_t The position of the Ball.
 */

vector_t getBallPosition(Ball *ball);
/**
 * @brief Gets the velocity of the Ball.
 *
 * This function returns the velocity vector of the specified Ball object.
 *
 * @param ball Pointer to the Ball object.
 * @return vector_t The velocity of the Ball.
 */

vector_t getBallVelocity(Ball *ball);

/**
 * @brief Gets the angular velocity of the Ball.
 *
 * This function returns the angular velocity vector of the specified Ball object.
 *
 * @param ball Pointer to the Ball object.
 * @return vector3_t The angular velocity of the Ball.
 */

vector3_t getBallAngVelocity(Ball *ball);
/**
 * @brief Gets the state of the Ball.
 *
 * This function returns the state of the specified Ball object.
 *
 * @param ball Pointer to the Ball object.
 * @return BALL_STATE The state of the Ball.
 */

BALL_STATE getBallState(Ball *ball);
/**
 * @brief Gets the image of the Ball.
 *
 * This function returns the image of the specified Ball object.
 *
 * @param ball Pointer to the Ball object.
 * @return xpm_image_t The image of the Ball.
 */

xpm_image_t getBallImage(Ball *ball);

/**
 * @brief Checks if the Ball is not moving.
 *
 * This function checks if the Ball is in a state where it is not moving, which includes being pocketed, stationary, or spinning.
 *
 * @param ball Pointer to the Ball object.
 * @return bool True if the Ball is not moving, false otherwise.
 */

bool ballNotMoving(Ball *ball);
