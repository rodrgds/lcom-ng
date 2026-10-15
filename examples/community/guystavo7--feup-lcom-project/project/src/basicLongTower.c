#include "../include/buildings/basicLongTower.h"
/** @file basicLongTower.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/data.h"
#include "../include/components/projectile.h"
#include "../resources/blue/tower/blueTower.h"
#include "../resources/blue/monk/monkIdle.h"
#include "../resources/blue/monk/monkHeal.h"
#include "../resources/arrow/arrow.h"

GenericTower* basicLongTowerNew(Vec2d position) {
    SpriteXPM* lowerHalf = newSpriteXPM((xpm_map_t)towerXPM[0], XPM_8_8_8, 0, 0);
    SpriteXPM* upperHalf = newSpriteXPM((xpm_map_t)towerXPM[1], XPM_8_8_8, BASIC_UPPER_HALF_OFFSET.x, BASIC_UPPER_HALF_OFFSET.y);

	SpriteXPM** archerIdle = malloc(sizeof(SpriteXPM*) * MONK_IDLE_FRAMES);
	for (int i = 0; i < MONK_IDLE_FRAMES; i++) archerIdle[i] = newSpriteXPM((xpm_map_t)monkIdleXPM[i], XPM_8_8_8, MONK_OFFSET.x, MONK_OFFSET.y);
	AnimatedSprite* idle = animatedSpriteNew(ARCHER_FRAME_SPEED, archerIdle, MONK_IDLE_FRAMES, LOOP);

	SpriteXPM** archerShoot = malloc(sizeof(SpriteXPM*) * MONK_HEAL_FRAMES);
	for (int i = 0; i < MONK_HEAL_FRAMES; i++) archerShoot[i] = newSpriteXPM((xpm_map_t)monkHealXPM[i], XPM_8_8_8, MONK_HEAL_OFFSET.x, MONK_HEAL_OFFSET.y);
	AnimatedSprite* shoot = animatedSpriteNew(BASIC_TOWER_SHOOT_SPEED, archerShoot, MONK_HEAL_FRAMES, NORMAL);

	SpriteXPM** projectileSprite = malloc(sizeof(SpriteXPM*) * ARROW_FRAMES); 
	for (int i = 0; i < ARROW_FRAMES; i++) projectileSprite[i] = newSpriteXPM((xpm_map_t)arrowXPM[i], XPM_8_8_8, 0, 0);

    return towerNew(upperHalf, lowerHalf, idle, shoot, position, BASIC_TOWER_SIZE, BASIC_TOWER_HIT_BOX_OFFSET, BASIC_TOWER_RANGE, BASIC_TOWER_RANGE_OFFSET, basicLongTowerUpdate, BASIC_TOWER_COOLDOWN, PROJ_NORMAL, projectileSprite, ARROW_FRAMES, TOWER_KIND_LONG);
}

void basicLongTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {}
