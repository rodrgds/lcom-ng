 
/** @file fleeMonster.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../../include/monster/fleeMonster.h"

#include "../../resources/flee/fleefly.h"

#define FLEE_MONSTER_IDLE_FRAMES 19
#define FLEE_MONSTER_FRAME_SPEED 10

static AnimatedSprite* pawnAnimation=NULL; 

static AnimatedSprite* getSprites_(char** XPMs[], int n, float frameSpeed, Vec2d offset) {
	SpriteXPM** res = malloc(n * sizeof(SpriteXPM*));
    for (int i = 0; i < n; i++) res[i] = newSpriteXPM((xpm_map_t)XPMs[i], XPM_8_8_8, offset.x, offset.y);
    return animatedSpriteNew(frameSpeed, res, n , LOOP);
}


Monster* fleeMonsterNew(float x,float y ,Path* path){
    if (pawnAnimation == NULL) {
        pawnAnimation = getSprites_(all_flee_xpms, FLEE_MONSTER_IDLE_FRAMES, FLEE_MONSTER_FRAME_SPEED, (Vec2d){0,0});
    }
    Monster* p=monsterNew(x,y,10,10,50,10000,pawnAnimation,path);
    return p;
} 
