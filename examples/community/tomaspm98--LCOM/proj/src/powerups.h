#ifndef __POWERUPS_H
#define __POWERUPS_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "images/powerup_ball.xpm"
#include "images/powerup_limits.xpm"
#include "entity.h"
#include "ball.h"

/// @brief Selects the powerup of the round and draws it in the screen
/// @return 0 if success, 1 if no success
int draw_powerup();
/// @brief Detects the collision between the ball and the powerup and which player gets it
/// @return 0 if success, 1 if no success
int catch_powerup();

#endif /* _POWERUPS_H_ */
