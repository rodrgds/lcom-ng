#pragma once

#include <lcom/lcf.h>
#include <stdlib.h>

#include "ChampionController.h"
#include "MinionController.h"
#include "TowerController.h"

#include "../input/KeyboardInterpreter.h"
#include "../input/MouseInterpreter.h"

#include "vbe.h"

/**
 * @brief Stores map dimensions used by the in-match update logic.
 */
typedef struct {
    int map_w; /**< Map width in pixels. */
    int map_h; /**< Map height in pixels. */
} GameController;

/**
 * @brief Allocates and initialises a GameController with the given map dimensions.
 * @param map_w Map width in pixels.
 * @param map_h Map height in pixels.
 * @return Pointer to the new GameController, or NULL on failure.
 */
GameController *gameControllerCreate(int map_w, int map_h);

/**
 * @brief Frees a GameController previously created with gameControllerCreate().
 * @param gc GameController to destroy.
 */
void gameControllerDestroy(GameController *gc);

/**
 * @brief Runs one tick of in-match game logic.
 * Processes in order: champion respawn/state updates, champion movement from keyboard input,
 * champion attacks from mouse input (left click = player 1, right click = player 2),
 * minion AI updates, and tower targeting/projectile updates.
 * @param gc GameController providing the map dimensions.
 * @param p1 Player-1 champion.
 * @param p2 Player-2 champion.
 * @param minions All active minion waves.
 * @param towers Both towers.
 * @param kbd Current keyboard state providing movement input.
 * @param mouse Current mouse state providing attack input.
 * @param second_elapsed True if a full second has passed since the last update.
 * @param p1_looped True if the player-1 animation completed a loop this tick.
 * @param p2_looped True if the player-2 animation completed a loop this tick.
 */
void gameControllerUpdate(GameController *gc, Champion *p1, Champion *p2, Minions *minions,
                          Towers *towers, KeyboardState *kbd, MouseState *mouse, bool second_elapsed,
                          bool p1_looped, bool p2_looped);
