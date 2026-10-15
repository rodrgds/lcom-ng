#include "../../include/monster/spearManMonster.h"
/** @file spearManMonster.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../../resources/red/lancer/lancer.h"

#define SPEAR_MAN_FRAMES 6
#define SPEAR_MAN_FRAME_SPEED 10

static AnimatedSprite* spearManAnimation = NULL;

static AnimatedSprite* getSprites_(char** XPMs[], int n, float frameSpeed, Vec2d offset) {
    SpriteXPM** res = malloc(n * sizeof(SpriteXPM*));
    for (int i = 0; i < n; i++) res[i] = newSpriteXPM((xpm_map_t)XPMs[i], XPM_8_8_8, offset.x, offset.y);
    return animatedSpriteNew(frameSpeed, res, n, LOOP);
}

static AnimatedSprite* spearManAnimationBuilder() {
    spearManAnimation = getSprites_(lancerXPM, SPEAR_MAN_FRAMES, SPEAR_MAN_FRAME_SPEED, (Vec2d){0, -35});
    return spearManAnimation;
}

Monster* spearManMonsterNew(float x, float y, Path* path) {
    Monster* p = monsterNew(x, y, 36, 74, LANCER_SPEED, LANCER_HEALTH, spearManAnimationBuilder, path);
    return p;
}
