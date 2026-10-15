#pragma once

#include "../model/tower/Tower.h"
#include "../model/tower/TowerConfig.h"
#include "HpBarViewer.h"
#include "sprite/Sprite.h"
#include <stdlib.h>
#include "./sprite/AnimSprite.h"
#include "../assets/game/tower/p1_missil_xpm.h"
#include "../assets/game/tower/p2_missil_xpm.h"

#include "video.h"

/**
 * @brief Renderer for both towers and their in-flight projectiles.
 */
typedef struct {
    HpBarViewer *hpBar;         /**< HP bar renderer shared by both towers. */
    Sprite *blue;               /**< Sprite for the blue team's tower. */
    Sprite *red;                /**< Sprite for the red team's tower. */
    AnimSprite *blueProjectile; /**< Animated sprite for the blue team's projectile. */
    AnimSprite *redProjectile;  /**< Animated sprite for the red team's projectile. */
} TowerViewer;

/**
 * @brief Allocates a TowerViewer and loads tower sprites from the provided XPM maps.
 * @param xpm_blue XPM map for the blue tower sprite.
 * @param xpm_red XPM map for the red tower sprite.
 * @return Pointer to the new TowerViewer, or NULL on failure.
 */
TowerViewer *createTowerViewer(xpm_map_t xpm_blue, xpm_map_t xpm_red);

/**
 * @brief Frees a TowerViewer and all owned sprites and HP bar.
 * @param viewer TowerViewer to destroy.
 */
void destroyTowerViewer(TowerViewer *viewer);

/**
 * @brief Draws both towers with their HP bars and any active in-flight projectiles.
 * @param viewer TowerViewer supplying sprites and the HP bar.
 * @param towers Towers state providing positions, HP, and projectile data.
 */
void towerViewerDraw(TowerViewer *viewer, Towers *towers);
