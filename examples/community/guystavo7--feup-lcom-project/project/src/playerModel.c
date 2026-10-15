#include "../include/player/playerModel.h"
/** @file playerModel.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../resources/blue/warrior/warriorIdle.h"
#include "../resources/blue/warrior/warriorRun.h"
#include "../include/data.h"

struct PlayerModel {
    Vec2d pos;
    Vec2d size;
    Vec2d velocity;
    Vec2d dir;

    AnimatedSprite* sprite;

	AnimatedSprite* idleSprite;
	AnimatedSprite* runningSprite;

    BoxCollider* hitBox;
    RigidBody* rb;
    float maxVelocity;
	bool isFlipped;
};

static AnimatedSprite* getSprites(char** XPMs[], int n, float frameSpeed, Vec2d offset) {
	SpriteXPM** res = malloc(n * sizeof(SpriteXPM*));
    for (int i = 0; i < n; i++) res[i] = newSpriteXPM((xpm_map_t)XPMs[i], XPM_8_8_8, offset.x, offset.y);
    return animatedSpriteNew(frameSpeed, res, n, LOOP);
}

PlayerModel* playerModelNew(Vec2d pos) {
    PlayerModel* res = malloc(sizeof(PlayerModel));
    res->pos.x = pos.x;
    res->pos.y = pos.y;
	res->isFlipped = false;

	res->size = PLAYER_SIZE;
    res->maxVelocity = PLAYER_MAX_VELOCITY;
	res->dir = EMPTY_VEC2D;

    res->hitBox = boxColliderNew(pos, PLAYER_HIT_BOX_OFFSET, res->size, PLAYER_TAG);
    res->rb = rigidBodyNew();

	res->idleSprite = getSprites(all_idle, PLAYER_IDLE_FRAMES, PLAYER_FRAME_SPEED, EMPTY_VEC2D);
	res->runningSprite = getSprites(all_run, PLAYER_RUNNING_FRAMES, PLAYER_FRAME_SPEED, EMPTY_VEC2D);
	res->sprite = res->idleSprite;

    return res;
}

void playerModelFree(PlayerModel* playerModel) {
	boxColliderFree(playerModel->hitBox);
	rigidBodyFree(playerModel->rb);
	animatedSpriteFree(playerModel->idleSprite);
	animatedSpriteFree(playerModel->runningSprite);
	free(playerModel);
}


float playerModelGetX(PlayerModel* playerModel) {
	return playerModel->pos.x;
}

float playerModelGetY(PlayerModel* playerModel) {
	return playerModel->pos.y;
}

float playerModelGetWidth(PlayerModel* playerModel) {
	return playerModel->size.x;
}

float playerModelGetHeight(PlayerModel* playerModel) {
	return playerModel->size.y;
}

float playerModelGetMaxVelocity(PlayerModel* playerModel) {
	return playerModel->maxVelocity;
}

AnimatedSprite* playerModelGetAnimatedSprite(PlayerModel* playerModel) {
	return playerModel->sprite;
} 

BoxCollider* playerModelGetHitBox(PlayerModel* playerModel) {
	return playerModel->hitBox;
}

RigidBody* playerModelGetRigidBody(PlayerModel* playerModel) {
	return playerModel->rb;
}

float playerModelGetDirX(PlayerModel* playerModel) {
	return playerModel->dir.x;
}

float playerModelGetDirY(PlayerModel* playerModel) {
	return playerModel->dir.y;
}

float playerModelGetVelocityX(PlayerModel* playerModel) {
	return playerModel->velocity.x;
}

float playerModelGetVelocityY(PlayerModel* playerModel) {
	return playerModel->velocity.y;
}

void playerModelSetVelocityX(PlayerModel* playerModel, float x) {
	playerModel->velocity.x = x;
}

void playerModelSetVelocityY(PlayerModel* playerModel, float y) {
	playerModel->velocity.y = y;
}

void playerModelSetX(PlayerModel* playerModel, float x) {
	playerModel->pos.x = x;
}

void playerModelSetY(PlayerModel* playerModel, float y) {
	playerModel->pos.y = y;
}

void playerModelSetDirX(PlayerModel* playerModel, float dirX) {
	playerModel->dir.x = dirX;
}

void playerModelSetDirY(PlayerModel* playerModel, float dirY) {
	playerModel->dir.y = dirY;
}

void playerModelSetPos(PlayerModel* playerModel, Vec2d pos) {
	playerModel->pos.x = pos.x;
	playerModel->pos.y = pos.y;
}

void playerModelSetDir(PlayerModel* playerModel, Vec2d dir) {
	playerModel->dir.x = dir.x;
	playerModel->dir.y = dir.y;
}

AnimatedSprite* playerModelGetRunningSprite(PlayerModel* model) {
	return model->runningSprite;
}

AnimatedSprite* playerModelGetIdleSprite(PlayerModel* model) {
	return model->idleSprite;
}

void playerModelSetAnimatedSprite(PlayerModel* model, AnimatedSprite* sprite) {
	model->sprite = sprite;
}

bool playerModelGetIsFlipped(PlayerModel* model) {
	return model->isFlipped;
}

void playerModelSetIsFlipped(PlayerModel* model, bool isFlip) {
	model->isFlipped = isFlip;
}
