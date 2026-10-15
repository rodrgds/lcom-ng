#ifndef PHYSICS_CONTROLLER_H
#define PHYSICS_CONTROLLER_H

#include "../worm/worm.h"

/**
 * @brief Updates one worm's physics using terrain and screen collisions.
 * @param worm Worm to update. NULL is accepted.
 */
void physics_update_worm(Worm *worm);

#endif /* PHYSICS_CONTROLLER_H */
