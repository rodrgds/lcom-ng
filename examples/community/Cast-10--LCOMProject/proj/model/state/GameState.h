#pragma once

#define MAX_MINIONS 20

#include <lcom/lcf.h>
#include <stdlib.h>

#include "../champion/ChampionConfig.h"
#include "../champion/Champion.h"
#include "../minion/Minion.h"
#include "../tower/Tower.h"

#include "../../controller/MinionController.h"

#include "vbe.h"

/**
 * @brief Aggregates all in-match entities.
 */
typedef struct {
    Champion *p1; /**< Player 1 (blue) champion. */
    Champion *p2; /**< Player 2 (red) champion. */
    Minions minions; /**< All active minion waves. */
    Towers towers; /**< Both team towers. */
} GameState;

/**
 * @brief Allocates and initialises a GameState, creating both champions and the towers.
 * @return Pointer to the new GameState, or NULL on allocation failure.
 */
GameState *gameStateCreate();

/**
 * @brief Frees a GameState and the champions it owns.
 * @param gs GameState to destroy.
 */
void gameStateDestroy(GameState *gs);
