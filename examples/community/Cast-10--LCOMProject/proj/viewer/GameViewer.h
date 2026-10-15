#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "sprite/BuildSprites.h"
#include "ChampionViewer.h"
#include "MinionViewer.h"
#include "TowerViewer.h"

#include "../assets/game/minion_p1/minion_p1_xpms.h"
#include "../assets/game/minion_p2/minion_p2_xpms.h"
#include "../assets/game/tower/tower_p1_xpm.h"
#include "../assets/game/tower/tower_p2_xpm.h"

#include "../model/champion/Champion.h"
#include "../model/minion/Minion.h"
#include "../model/tower/Tower.h"

/**
 * @brief Aggregated renderer for all in-match entities.
 */
typedef struct {
    ChampionViewer *p1; /**< Viewer for the player-1 champion. */
    ChampionViewer *p2; /**< Viewer for the player-2 champion. */
    MinionViewer *minion_blue; /**< Viewer for blue-team minions. */
    MinionViewer *minion_red; /**< Viewer for red-team minions. */
    TowerViewer *tower; /**< Viewer for both towers. */
} GameViewer;

/**
 * @brief Allocates and initialises a GameViewer, creating all sub-viewers.
 * @return Pointer to the new GameViewer, or NULL on failure.
 */
GameViewer *gameViewerCreate();

/**
 * @brief Frees a GameViewer and all sub-viewers it owns.
 * @param gv GameViewer to destroy.
 */
void gameViewerDestroy(GameViewer *gv);

/**
 * @brief Updates animation state for both champions and all minions for the current tick.
 * @param gv GameViewer to update.
 * @param p1 Player-1 champion providing state and direction.
 * @param p2 Player-2 champion providing state and direction.
 * @param minions All active minion waves.
 * @param p1_looped Output: set to true if the player-1 animation completed a loop this tick.
 * @param p2_looped Output: set to true if the player-2 animation completed a loop this tick.
 */
void gameViewerUpdate(GameViewer *gv, Champion *p1, Champion *p2, Minions *minions, bool *p1_looped, bool *p2_looped);

/**
 * @brief Draws all in-match entities (towers, minions, then champions) to the back buffer.
 * @param gv GameViewer supplying all sub-viewers.
 * @param p1 Player-1 champion.
 * @param p2 Player-2 champion.
 * @param minions All active minion waves.
 * @param towers Both team towers.
 */
void gameViewerDraw(GameViewer *gv, Champion *p1, Champion *p2, Minions *minions, Towers *towers);
