
#pragma once
#include <lcom/lcf.h>
#include "../model/ball/ball.h"
#include "../model/table.h"

/**
 * @brief Updates the motion of a ball over a specified time period.
 *
 * This function updates the motion of a ball over a specified time period, considering its current state and transitions,
 * along with parameters such as friction and gravity.
 *
 * @param table Pointer to the Table structure.
 * @param ball Pointer to the Ball structure representing the ball.
 * @param time The time period over which the motion is updated.
 */
void evolveBallMotion(Table* table, Ball* ball, double time);

/**
 * @brief Evolves the sliding state of a ball over a specified time period.
 *
 * This function evolves the sliding state of a ball over a specified time period, updating its position and velocity
 * based on sliding friction, spinning friction, and gravity.
 *
 * @param ball Pointer to the Ball structure representing the ball.
 * @param t The time period over which the sliding state is evolved.
 * @param uSpinning Spinning friction coefficient.
 * @param uSliding Sliding friction coefficient.
 * @param g Acceleration due to gravity.
 */

void evolveSlideState(Ball* ball, double t, double uSpinning, double uSliding, double g);

/**
 * @brief Evolves the rolling state of a ball over a specified time period.
 *
 * This function evolves the rolling state of a ball over a specified time period, updating its position and velocity
 * based on rolling friction, spinning friction, and gravity.
 *
 * @param ball Pointer to the Ball structure representing the ball.
 * @param t The time period over which the rolling state is evolved.
 * @param uRolling Rolling friction coefficient.
 * @param uSpinning Spinning friction coefficient.
 * @param g Acceleration due to gravity.
 */

void evolveRollState(Ball* ball, double t, double uRolling, double uSliding, double g);

/**
 * @brief Evolves the perpendicular spin of a ball over a specified time period.
 *
 * This function evolves the perpendicular spin of a ball over a specified time period, updating its angular velocity
 * based on spinning friction and gravity.
 *
 * @param ball Pointer to the Ball structure representing the ball.
 * @param t The time period over which the perpendicular spin is evolved.
 * @param uSpinning Spinning friction coefficient.
 * @param g Acceleration due to gravity.
 */
void evolvePrependicularSpin(Ball* ball, double t , double uSpinning, double g);
