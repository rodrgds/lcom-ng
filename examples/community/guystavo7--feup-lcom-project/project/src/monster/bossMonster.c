#include "../../include/monster/bossMonster.h"
/** @file bossMonster.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../../resources/black/warrior/warriorRun.h"

#define BOSS_FRAMES 6
#define BOSS_FRAME_SPEED 10

static AnimatedSprite* bossAnimation = NULL;

static AnimatedSprite* getSprites_(char** XPMs[], int n, float frameSpeed, Vec2d offset) {
    SpriteXPM** res = malloc(n * sizeof(SpriteXPM*));
    for (int i = 0; i < n; i++) res[i] = newSpriteXPM((xpm_map_t)XPMs[i], XPM_8_8_8, offset.x, offset.y);
    return animatedSpriteNew(frameSpeed, res, n, LOOP);
}

static AnimatedSprite* bossAnimationBuilder() {
    bossAnimation = getSprites_(warriorRunXPM, BOSS_FRAMES, BOSS_FRAME_SPEED, (Vec2d){-128, -256});
    return bossAnimation;
}

Monster* bossMonsterNew(float x, float y, Path* path) {
    Monster* p = monsterNew(x, y, 100, 100, BOSS_SPEED, BOSS_HEALTH, bossAnimationBuilder, path);
    return p;
}
