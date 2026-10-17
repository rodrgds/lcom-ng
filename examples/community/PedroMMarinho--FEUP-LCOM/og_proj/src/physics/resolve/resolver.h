
#pragma once
#include <lcom/lcf.h>
#include "../../model/event.h"
#include "../../model/table.h"
#include "../../model/cushion.h"

/**
 * @brief Resolves the given event on the table.
 *
 * This function handles different types of events that occur during gameplay
 * and applies appropriate actions to resolve them.
 *
 * @param table Pointer to the Table structure.
 * @param event The event to be resolved.
 */
void resolveEvent(Table *table, Event event);

/**
 * @brief Resolves the collision between two balls.
 *
 * This function calculates the result of a collision between two balls and updates their velocities accordingly.
 *
 * @param ball1 Pointer to the first ball involved in the collision.
 * @param ball2 Pointer to the second ball involved in the collision.
 */

void resolveBallBall(Ball* ball1, Ball* ball2);


/**
 * @brief Resolves the collision between a ball and a cushion.
 *
 * This function calculates the result of a collision between a ball and a cushion and updates the ball's velocity accordingly.
 *
 * @param ball Pointer to the ball involved in the collision.
 * @param cushionNormal The normal vector of the cushion.
 * @param restitution The coefficient of restitution for the collision.
 */

void resolveBallCushion(Ball* ball, vector_t cushionNormal, double restitution);

/**
 * @brief Resolves the collision between a ball and a cushion in a realistic manner.
 *
 * This function calculates the result of a collision between a ball and a cushion, considering realistic physics.
 *
 * @param ball Pointer to the ball involved in the collision.
 * @param cushionNormal The normal vector of the cushion.
 * @param restitution The coefficient of restitution for the collision.
 * @param friction The coefficient of friction for the collision.
 */

void resolveBallCushionRealistic(Ball *ball, vector_t cushionNormal, double restitution, double friction);

/**
 * @brief Resolves the collision between a ball and a pocket.
 *
 * This function calculates the result of a collision between a ball and a pocket and updates the ball's state accordingly.
 *
 * @param ball Pointer to the ball involved in the collision.
 * @param pocket Pointer to the Pocket structure representing the pocket.
 */

void resolveBallPocket(Ball* ball, Table* table, Pocket* pocket);

/**
 * @brief Resolves the collision between the cue stick and a ball.
 *
 * This function calculates the result of a collision between the cue stick and a ball and updates the ball's velocity accordingly.
 *
 * @param cue Pointer to the Cue structure representing the cue stick.
 * @param ball Pointer to the ball involved in the collision.
 * @param maxSpeed The maximum speed of the shot.
 */

void resolveStickBall(Cue* cue, Ball* ball, double maxSpeed);

/**
 * @brief Adjusts the positions of two balls to prevent overlap.
 *
 * This function adjusts the positions of two balls if they overlap after a collision.
 *
 * @param ball1 Pointer to the first ball.
 * @param ball2 Pointer to the second ball.
 */

void makeBallsKiss(Ball* ball1, Ball* ball2);

/**
 * @brief Adjusts the position of a ball after a linear cushion collision.
 *
 * This function adjusts the position of a ball after colliding with a linear cushion to prevent overlap.
 *
 * @param ball Pointer to the ball involved in the collision.
 * @param cushion Pointer to the LinearCushion structure representing the cushion.
 */

void makeBallLinearCushionKiss(Ball* ball, LinearCushion* cushion);
/**
 * @brief Adjusts the position of a ball after a circular cushion collision.
 *
 * This function adjusts the position of a ball after colliding with a circular cushion to prevent overlap.
 *
 * @param ball Pointer to the ball involved in the collision.
 * @param cushion Pointer to the CircularCushion structure representing the cushion.
 */

void makeBallCircularCushionKiss(Ball* ball, CircularCushion* cushion);


/**
 * @brief Asserts that a ball is in a spinning state.
 *
 * This function sets a ball's velocity and angular velocity to zero, putting it in a spinning state.
 *
 * @param ball Pointer to the ball to be set as spinning.
 */

void assertSpinning(Ball *ball);

/**
 * @brief Asserts that a ball is in a stationary state.
 *
 * This function sets a ball's velocity and angular velocity to zero, putting it in a stationary state.
 *
 * @param ball Pointer to the ball to be set as stationary.
 */
void assertStationary(Ball *ball);
