#pragma once
#include <lcom/lcf.h>
#include "../model/table.h"
#include "../model/event.h"



/**
 * @brief Finds the next collision event between two balls on the table.
 *
 * This function calculates the time until the next collision event between two balls
 * on the table and returns an Event structure representing the collision event.
 *
 * @param table Pointer to the Table structure.
 * @return An Event structure representing the next ball-ball collision event.
 */
Event getNextBallBallCollision(Table* table);

/**
 * @brief Finds the next collision event between a ball and a linear cushion on the table.
 *
 * This function calculates the time until the next collision event between a ball and a linear cushion
 * on the table and returns an Event structure representing the collision event.
 *
 * @param table Pointer to the Table structure.
 * @return An Event structure representing the next ball-linear cushion collision event.
 */

Event getNextBallLinearCushionCollision(Table* table);

/**
 * @brief Finds the next collision event between a ball and a circular cushion on the table.
 *
 * This function calculates the time until the next collision event between a ball and a circular cushion
 * on the table and returns an Event structure representing the collision event.
 *
 * @param table Pointer to the Table structure.
 * @return An Event structure representing the next ball-circular cushion collision event.
 */
Event getNextBallCircularCushionCollision(Table* table);

/**
 * @brief Finds the next collision event between a ball and a pocket on the table.
 *
 * This function calculates the time until the next collision event between a ball and a pocket
 * on the table and returns an Event structure representing the collision event.
 *
 * @param table Pointer to the Table structure.
 * @return An Event structure representing the next ball-pocket collision event.
 */
Event getNextBallPocketCollision(Table* table);

/**
 * @brief Updates the next transition event for a ball on the table.
 *
 * This function updates the next transition event (e.g., from rolling to stationary) for a ball on the table
 * based on its current state and other parameters such as friction and gravity.
 *
 * @param table Pointer to the Table structure.
 * @param ball Pointer to the Ball structure representing the ball.
 */
void updateBallNextTransition(Table* table, Ball* ball);

/**
 * @brief Finds the next transition event for a ball on the table.
 *
 * This function calculates the time until the next transition event (e.g., from rolling to stationary)
 * for a ball on the table and returns an Event structure representing the transition event.
 *
 * @param table Pointer to the Table structure.
 * @return An Event structure representing the next ball transition event.
 */
Event getNextTransition(Table* table);

/**
 * @brief Finds the next event on the table.
 *
 * This function calculates the time until the next event (collision or transition) on the table
 * and returns an Event structure representing the next event.
 *
 * @param table Pointer to the Table structure.
 * @return An Event structure representing the next event on the table.
 */

Event getNextEvent(Table* table);
