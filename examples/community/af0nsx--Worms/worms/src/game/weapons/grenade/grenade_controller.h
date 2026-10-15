#ifndef GRENADE_CONTROLLER_H
#define GRENADE_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>
#include "grenade.h"
#include "../../game.h"

/**
 * @brief Fires an aimed grenade shot from the active worm's facing-side muzzle.
 *
 * @param context Pointer to GameContext.
 * @param active_worm Worm that is firing.
 * @param angle Firing angle in radians.
 * @param power Firing power (0-100).
 */
void grenade_controller_fire_aimed_from_worm(GameContext *context, Worm *active_worm, double angle, int power);

/**
 * @brief Updates the grenade state each tick, applying bouncing and explosion logic.
 * @param context Pointer to GameContext.
 * @param grenade Pointer to the active Grenade.
 */
void grenade_controller_tick(GameContext *context, Grenade *grenade);

/**
 * @brief Handles aiming controls for the grenade.
 * @param context Pointer to GameContext.
 * @param active_worm Pointer to the worm currently aiming.
 * @param aim_angle Pointer to current aim angle.
 * @param aim_power Pointer to current aim power.
 * @param space_held Pointer to space key state.
 * @return true if fired, false otherwise.
 */
bool grenade_controller_handle_aim(GameContext *context, Worm *active_worm, double *aim_angle, int *aim_power, bool *space_held);

#endif /* GRENADE_CONTROLLER_H */
