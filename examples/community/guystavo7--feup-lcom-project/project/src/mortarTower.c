#include "../include/buildings/mortarTower.h"
/** @file mortarTower.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/data.h"
#include "../resources/asteroid/asteroid.h"
#include "../resources/black/monk/monkHeal.h"
#include "../resources/black/monk/monkIdle.h"
#include "../resources/black/tower/blackTower.h"

GenericTower* mortarTowerNew(Vec2d position) {
    SpriteXPM* lowerHalf = newSpriteXPM((xpm_map_t)towerXPM[0], XPM_8_8_8, 0, 0);
    SpriteXPM* upperHalf = newSpriteXPM((xpm_map_t)towerXPM[1], XPM_8_8_8, BASIC_UPPER_HALF_OFFSET.x, BASIC_UPPER_HALF_OFFSET.y);

    SpriteXPM** archerIdle = malloc(sizeof(SpriteXPM*) * MONK_IDLE_FRAMES);
    for (int i = 0; i < MONK_IDLE_FRAMES; i++) archerIdle[i] = newSpriteXPM((xpm_map_t)monkIdleXPM[i], XPM_8_8_8, MONK_OFFSET.x, MONK_OFFSET.y);
    AnimatedSprite* idle = animatedSpriteNew(MONK_FRAME_SPEED, archerIdle, MONK_IDLE_FRAMES, LOOP);

    SpriteXPM** archerShoot = malloc(sizeof(SpriteXPM*) * MONK_HEAL_FRAMES);
    for (int i = 0; i < MONK_HEAL_FRAMES; i++) archerShoot[i] = newSpriteXPM((xpm_map_t)monkHealXPM[i], XPM_8_8_8, MONK_HEAL_OFFSET.x, MONK_HEAL_OFFSET.y);
    AnimatedSprite* shoot = animatedSpriteNew(MORTAR_TOWER_SHOOT_SPEED, archerShoot, MONK_HEAL_FRAMES, NORMAL);

    SpriteXPM** projectileSprite = malloc(sizeof(SpriteXPM*) * ASTEROID_FRAMES);
    for (int i = 0; i < ASTEROID_FRAMES; i++) projectileSprite[i] = newSpriteXPM((xpm_map_t)asteroidXPM[i], XPM_8_8_8, 0, 0);

    return towerNew(upperHalf, lowerHalf, idle, shoot, position, BASIC_TOWER_SIZE, BASIC_TOWER_HIT_BOX_OFFSET, MORTAR_TOWER_RANGE, MORTAR_TOWER_RANGE_OFFSET, mortarTowerUpdate, MORTAR_COOLDOWN, PROJ_SKY_FALL, projectileSprite, ASTEROID_FRAMES, TOWER_KIND_MORTAR);
}

void mortarTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {}
