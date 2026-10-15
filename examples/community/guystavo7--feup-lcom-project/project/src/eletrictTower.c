#include "../include/buildings/eletricTower.h"
/** @file eletrictTower.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/data.h"
#include "../resources/yellow/tower/yellowTower.h"
#include "../resources/yellow/monk/monkIdle.h"
#include "../resources/yellow/monk/monkHeal.h"
#include "../resources/lightning/lightning.h"

GenericTower* eletrictTowerNew(Vec2d position) {
    SpriteXPM* lowerHalf = newSpriteXPM((xpm_map_t)towerXPM[0], XPM_8_8_8, 0, 0);
    SpriteXPM* upperHalf = newSpriteXPM((xpm_map_t)towerXPM[1], XPM_8_8_8, BASIC_UPPER_HALF_OFFSET.x, BASIC_UPPER_HALF_OFFSET.y);

    SpriteXPM** archerIdle = malloc(sizeof(SpriteXPM*) * MONK_IDLE_FRAMES);
    for (int i = 0; i < MONK_IDLE_FRAMES; i++) archerIdle[i] = newSpriteXPM((xpm_map_t)monkIdleXPM[i], XPM_8_8_8, MONK_OFFSET.x, MONK_OFFSET.y);
    AnimatedSprite* idle = animatedSpriteNew(ARCHER_FRAME_SPEED, archerIdle, MONK_IDLE_FRAMES, LOOP);

    SpriteXPM** archerShoot = malloc(sizeof(SpriteXPM*) * MONK_HEAL_FRAMES);
    for (int i = 0; i < MONK_HEAL_FRAMES; i++) archerShoot[i] = newSpriteXPM((xpm_map_t)monkHealXPM[i], XPM_8_8_8, MONK_HEAL_OFFSET.x, MONK_HEAL_OFFSET.y);
    AnimatedSprite* shoot = animatedSpriteNew(ELETRICT_TOWER_SHOOT_SPEED, archerShoot, MONK_HEAL_FRAMES, NORMAL);

	SpriteXPM** projectileSprite = malloc(sizeof(SpriteXPM*) * LIGHTNING_FRAMES); 
	for (int i = 0; i < LIGHTNING_FRAMES; i++) projectileSprite[i] = newSpriteXPM((xpm_map_t)lightningXPM[i], XPM_8_8_8, 0, 0);

    return towerNew(upperHalf, lowerHalf, idle, shoot, position, BASIC_TOWER_SIZE, BASIC_TOWER_HIT_BOX_OFFSET, ELETRICT_TOWER_RANGE, ELETRICT_TOWER_RANGE_OFFSET, eletrictTowerUpdate, ELETRICT_COOLDOWN, PROJ_NEAR, projectileSprite, LIGHTNING_FRAMES, TOWER_KIND_ELETRICT);
}

void eletrictTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {}
