#include "../include/buildings/fastTower.h"
/** @file fastTower.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/data.h"
#include "../resources/red/archer/archerIdle.h"
#include "../resources/red/archer/archerShoot.h"
#include "../resources/red/tower/redTower.h"
#include "../resources/arrow/arrow.h"

GenericTower* fastTowerNew(Vec2d position) {
    SpriteXPM* lowerHalf = newSpriteXPM((xpm_map_t)towerXPM[0], XPM_8_8_8, 0, 0);
    SpriteXPM* upperHalf = newSpriteXPM((xpm_map_t)towerXPM[1], XPM_8_8_8, BASIC_UPPER_HALF_OFFSET.x, BASIC_UPPER_HALF_OFFSET.y);

    SpriteXPM** archerIdle = malloc(sizeof(SpriteXPM*) * ARCHER_IDLE_FRAMES);
    for (int i = 0; i < ARCHER_IDLE_FRAMES; i++) archerIdle[i] = newSpriteXPM((xpm_map_t)archerIdleXPM[i], XPM_8_8_8, ARCHER_OFFSET.x, ARCHER_OFFSET.y);
    AnimatedSprite* idle = animatedSpriteNew(ARCHER_FRAME_SPEED, archerIdle, ARCHER_IDLE_FRAMES, LOOP);

    SpriteXPM** archerShoot = malloc(sizeof(SpriteXPM*) * ARCHER_SHOOT_FRAMES);
    for (int i = 0; i < ARCHER_SHOOT_FRAMES; i++) archerShoot[i] = newSpriteXPM((xpm_map_t)archerShootXPM[i], XPM_8_8_8, ARCHER_OFFSET.x, ARCHER_OFFSET.y);
	AnimatedSprite* shoot = animatedSpriteNew(FAST_TOWER_SHOOT_SPEED, archerShoot, ARCHER_SHOOT_FRAMES, NORMAL);

	SpriteXPM** projectileSprite = malloc(sizeof(SpriteXPM*) * ARROW_FRAMES); 
	for (int i = 0; i < ARROW_FRAMES; i++) projectileSprite[i] = newSpriteXPM((xpm_map_t)arrowXPM[i], XPM_8_8_8, 0, 0);

    return towerNew(upperHalf, lowerHalf, idle, shoot, position, BASIC_TOWER_SIZE, BASIC_TOWER_HIT_BOX_OFFSET, FAST_TOWER_RANGE, FAST_TOWER_RANGE_OFFSET, fastTowerUpdate, FAST_TOWER_COOLDOWN, PROJ_NORMAL, projectileSprite, ARROW_FRAMES, TOWER_KIND_FAST);
}

void fastTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {}
