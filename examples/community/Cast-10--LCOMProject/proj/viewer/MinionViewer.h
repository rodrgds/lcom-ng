#pragma once

#include <stdlib.h>
#include <lcom/lcf.h>

#include "sprite/AnimSprite.h"
#include "HpBarViewer.h"

#include "../assets/game/minion_p1/minion_p1_xpms.h"
#include "../assets/game/minion_p2/minion_p2_xpms.h"

#include "../model/minion/MinionConfig.h"
#include "../model/minion/Minion.h"

/**
 * @brief Renderer for all minions of a single team.
 */
typedef struct {
    AnimSprite *idle; /**< Animation clip played while the minion is walking. */
    AnimSprite *atk;  /**< Animation clip played while the minion is attacking. */
    HpBarViewer *hpBar; /**< HP bar renderer shared by all minions of this team. */
} MinionViewer;

/**
 * @brief Allocates a MinionViewer and loads animation clips from the provided XPM rows.
 * @param stop_xpm XPM row data for the idle/walk animation.
 * @param atk_xpm XPM row data for the attack animation.
 * @return Pointer to the new MinionViewer, or NULL on failure.
 */
MinionViewer *createMinionViewer(xpm_row_t *stop_xpm, xpm_row_t *atk_xpm);

/**
 * @brief Frees a MinionViewer and its animation clips and HP bar.
 * @param viewer MinionViewer to destroy.
 */
void destroyMinionViewer(MinionViewer *viewer);

/**
 * @brief Advances both animation clips by one tick.
 * @param viewer MinionViewer to update.
 */
void minionViewerUpdate(MinionViewer *viewer);

/**
 * @brief Draws all active minions of the given team with their HP bars.
 * Selects the idle or attack clip depending on each minion's state.
 * @param viewer MinionViewer supplying sprites and the HP bar.
 * @param minions All active minion waves.
 * @param team Team whose minions should be drawn.
 * @return 0 on success, non-zero on error.
 */
int minionViewerDraw(MinionViewer *viewer, Minions *minions, Team team);
