#ifndef LOCAL_OR_REMOTE_H
#define LOCAL_OR_REMOTE_H

#include "../controllers/gpu/sprites/sprite.h"
#include "../controllers/gpu/sprites/dynamicSprite.h"
#include "../game/game.h"
#include "../assets/xpm/all.xpm"
#include <lcom/timer.h>
#include <lcom/lcf.h>

/**
 * @brief Runs the menu to select between local or remote mode.
 *
 * This function presents the user with an option to choose between
 * local or remote gameplay modes and returns the user's selection.
 *
 * @return int Returns an integer indicating the selected mode.
 *             Typically, a specific value is assigned for local and remote.
 */
int run_local_or_remote();

#endif
