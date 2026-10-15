 
/** @file pawnMonster.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../../include/monster/pawnMonster.h"

#include "../../resources/pawn/pawnRun.h"

#define PAWN_MONSTER_IDLE_FRAMES 6
#define PAWN_MONSTER_FRAME_SPEED 10

static AnimatedSprite* pawnAnimation=NULL; 

static AnimatedSprite* getSprites_(char** XPMs[], int n, float frameSpeed, Vec2d offset) {
	SpriteXPM** res = malloc(n * sizeof(SpriteXPM*));
    for (int i = 0; i < n; i++) res[i] = newSpriteXPM((xpm_map_t)XPMs[i], XPM_8_8_8, offset.x, offset.y);
    return animatedSpriteNew(frameSpeed, res, n , LOOP);
}

static AnimatedSprite* pawnAnimationBuilder(){

    pawnAnimation = getSprites_(all_pawn_xpms, PAWN_MONSTER_IDLE_FRAMES, PAWN_MONSTER_FRAME_SPEED, (Vec2d){0,0});
    return pawnAnimation;
}


Monster* pawnMonsterNew(float x,float y ,Path* path){
    
    Monster* p=monsterNew(x,y,10,10,PAWN_SPEED,PAWN_HEALTH,pawnAnimationBuilder,path);
    return p;
} 
