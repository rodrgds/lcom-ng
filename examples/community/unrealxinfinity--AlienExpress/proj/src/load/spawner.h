#ifndef SPAWNER_H
#define SPAWNER_H

#include "view.h"


/**
 * @brief Sets a dead monster to not dead if the rate was high enough
 * @param rate  spawn rate to spawn
 */
void spawn_monsters(int rate);

/**
 * @brief Sets a dead worker to not dead if the rate was high enough
 * @param rate  spawn rate to spawn
 */
void spawn_workers(int rate);

/**
 * @brief Sets a dead wolf to not dead if the rate was high enough
 * @param rate  spawn rate to spawn
 */
void spawn_wolf(int rate);


/**
 * @brief Sets a dead package to not dead if the rate was high enough
 * @param rate  spawn rate to spawn
 */
void spawn_packages(int rate);

/**
 * @brief Sets a dead teleport to not dead if the rate was high enough
 * @param rate  spawn rate to spawn
 */

void spawn_teleports(int rate);

/**
 * @brief Sets a dead bullet to not dead
 * @param index Position of array where the bullet resides
 */

void spawn_bullet(int index);


/**
 * @brief Calls all other spawn functions
 */
void spawn();



#endif
