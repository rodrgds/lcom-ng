#ifndef AIM_VIEWER_H
#define AIM_VIEWER_H

#include "../worm/worm.h"

/**
 * @brief Draws the aiming trajectory and angle sprite for the currently equipped weapon.
 * @param worm Pointer to the active worm.
 * @param aim_angle Current aim angle.
 * @param aim_power Current aim power.
 */
void aim_viewer_draw(const Worm *worm, double aim_angle, int aim_power);

#endif /* AIM_VIEWER_H */
