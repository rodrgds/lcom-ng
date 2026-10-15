#ifndef BAZOOKA_CONTROLLER_H
#define BAZOOKA_CONTROLLER_H

#include "../../game.h"
#include "bazooka.h"

/**
 * @brief Ticks the bazooka projectile and handles collisions.
 *
 * Updates gravity physics. If the missile hits the terrain or water,
 * it is deactivated, terrain is destroyed, an explosion is spawned,
 * and nearby worms are damaged.
 *
 * @param context Pointer to the game context.
 * @param bazooka Pointer to the active Bazooka.
 */
void bazooka_controller_tick(GameContext *context, Bazooka *bazooka);

/**
 * @brief Fires an aimed bazooka shot from the active worm's facing-side muzzle.
 *
 * @param context Pointer to the game context.
 * @param active_worm Worm that is firing. NULL is accepted.
 * @param angle Shot angle in radians.
 * @param power Shot power percentage.
 */
void bazooka_controller_fire_aimed_from_worm(GameContext *context, Worm *active_worm, double angle, int power);

/**
 * @brief Handles bazooka aim input and fires on space release.
 * @param context Pointer to the game context.
 * @param active_worm Worm currently aiming.
 * @param aim_angle Pointer to current aim angle.
 * @param aim_power Pointer to current aim power.
 * @param space_held Pointer to previous space-key state.
 * @return true when a shot was fired, false otherwise.
 */
bool bazooka_controller_handle_aim(GameContext *context, Worm *active_worm, double *aim_angle, int *aim_power, bool *space_held);

#endif /* BAZOOKA_CONTROLLER_H */
