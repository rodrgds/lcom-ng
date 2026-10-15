#include "../include/tiles/collectable.h"
/** @file collectable.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include <stdlib.h>

struct Collectable {
	SpriteXPM* sprite;

	BoxCollider* hitBox;

	Vec2d position;
	Vec2d size;
	bool isActive;
};

Collectable* collectableNew(Vec2d position, Vec2d offset, char** XPM) {
	Collectable* res = malloc(sizeof(Collectable));

	SpriteXPM* sprite = newSpriteXPM((xpm_map_t)XPM, XPM_8_8_8, offset.x, offset.y);

	Vec2d hitBoxSize = {getSpriteWidth(sprite), getSpriteHeight(sprite	)};
	BoxCollider* hitBox = boxColliderNew(position, offset, hitBoxSize, 2);
	res->sprite = sprite;
	res->hitBox = hitBox;
	res->position = position;
	res->size = hitBoxSize;
	res->isActive = true;

	return res;
}

void collectableFree(Collectable* c) {
	freeSpriteXPM(c->sprite);
	boxColliderFree(c->hitBox);
	free(c);
}

void collectableSetActive(Collectable* c, bool isActive) {
	c->isActive = isActive;
}

bool collectableIsActive(Collectable* c) {
	return c->isActive;
}

void collectableSetX(Collectable* c, float x) {
	c->position.x = x;
}

void collectableSetY(Collectable* c, float y) {
	c->position.y = y;
}

void collectableSetPostion(Collectable* c, Vec2d position) {
	c->position = position;
}

float collectableGetX(Collectable* c) {
	return c->position.x;	
}

float collectableGetY(Collectable* c) {
	return c->position.y;
}

void collectableUpdate(Collectable* c, Terminal* terminal) {
	if (!c->isActive) return;
	drawSpriteXPM(c->sprite, terminal, c->position.x, c->position.y, false);
}

BoxCollider* collectableGetHitBox(Collectable* c) {
	return c->hitBox;
}
