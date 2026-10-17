#pragma once
#include <lcom/lcf.h>
#include "../model/table.h"
#include "../model/event.h"



/**
 * @brief Initializes the shot event, setting the stick ball as the first event in the simulation.
 *
 * This function initializes the shot event with a time of 0, representing the initial state where the stick
 * is in contact with the cue ball. It sets the next event for the table.
 *
 * @param table Pointer to the Table structure.
 */
void processShot(Table* table);

/**
 * @brief Updates the physics simulation of the table over a specified time period.
 *
 * This function updates the physics simulation of the table over a specified time period (dt), processing events
 * and evolving the state of the balls accordingly.
 *
 * @param table Pointer to the Table structure.
 * @param dt The time step for the physics simulation.
 * @return True if the simulation is still ongoing after the update, false otherwise.
 */
bool updatePhysics(Table* table, double dt);

/**
 * @brief Evolves the motion of all balls on the table over a specified time period.
 *
 * This function evolves the motion of all balls on the table over a specified time period (time),
 * updating their positions and velocities based on their current states and physical laws.
 *
 * @param table Pointer to the Table structure.
 * @param time The time period over which the motion is evolved.
 */

void evolveBalls(Table *table, double time);
