#include "../include/components/animatedSprite.h"
/** @file animatedSprite.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include <stdint.h>
#include <stdlib.h>

struct AnimatedSprite {
    SpriteXPM** sprites;
    int n;

    float frameSpeed;
    float current;
    enum AnimatedSpriteType type;

    bool isActive;
    bool isFinished;
};

AnimatedSprite* animatedSpriteNew(float frameSpeed, SpriteXPM** xpm, int n, enum AnimatedSpriteType type) {
    AnimatedSprite* s = malloc(sizeof(AnimatedSprite));
    s->frameSpeed = frameSpeed;
    s->sprites = xpm;

    s->n = n;
    s->current = 0;
    s->type = type;
    s->isFinished = false;

    switch (type) {
        case LOOP:
            s->isActive = true;
            break;
        case ONE_SHOT:
            s->isActive = false;
            break;
        case NORMAL:
            s->isActive = true;
            break;
    }

    return s;
}

void animatedSpriteFree(AnimatedSprite* sprite) {
    for (int i = 0; i < sprite->n; i++) {
        freeSpriteXPM(sprite->sprites[i]);
    }

    free(sprite->sprites);
    free(sprite);
}

void animatedSpriteFreeSpritesArrayOnly(AnimatedSprite* sprite) {
    if (sprite) {
        free(sprite->sprites);
    }
}

// Updates
static void loopUpdate(int x, int y, AnimatedSprite* sprite, Terminal* t, float deltaTime, bool isFlipped) {
    sprite->current += deltaTime * sprite->frameSpeed;
    if (sprite->current >= sprite->n) sprite->current = 0;

    SpriteXPM* currentFrame = sprite->sprites[(int)sprite->current];
    drawSpriteXPM(currentFrame, t, x, y, isFlipped);
}

static void oneShotUpdate(int x, int y, AnimatedSprite* sprite, Terminal* t, float deltaTime, bool isFlipped) {
    if (!sprite->isActive) return;
    sprite->current += deltaTime * sprite->frameSpeed;

    if (sprite->current >= sprite->n) {
        sprite->isActive = false;
        sprite->isFinished = true;

    } else {
        SpriteXPM* currentFrame = sprite->sprites[(int)sprite->current];
        drawSpriteXPM(currentFrame, t, x, y, isFlipped);
    }
}

static void normalUpdate(int x, int y, AnimatedSprite* sprite, Terminal* t, float deltaTime, bool isFlipped) {
    if (sprite->isActive) sprite->current += deltaTime * sprite->frameSpeed;
    if (sprite->current >= sprite->n) {
        sprite->isActive = false;
        sprite->isFinished = true;
        sprite->current = sprite->n - 1;
    }

    SpriteXPM* currentFrame = sprite->sprites[(int)sprite->current];
    drawSpriteXPM(currentFrame, t, x, y, isFlipped);
}

void animatedSpriteUpdate(int x, int y, AnimatedSprite* sprite, Terminal* t, float deltaTime, bool isFlipped) {
    switch (sprite->type) {
        case LOOP:
            loopUpdate(x, y, sprite, t, deltaTime, isFlipped);
            break;

        case ONE_SHOT:
            oneShotUpdate(x, y, sprite, t, deltaTime, isFlipped);
            break;

        case NORMAL:
            normalUpdate(x, y, sprite, t, deltaTime, isFlipped);
            break;
    }
}

void animatedSpriteStart(AnimatedSprite* sprite) {
    sprite->isActive = true;
	sprite->isFinished = false;
    sprite->current = 0;
}

bool animatedSpriteIsFinished(AnimatedSprite* sprite) {
    return sprite->isFinished;
}

void animatedSpriteReset(AnimatedSprite* sprite) {
	sprite->current = 0;
}

int animatedSpriteGetWidth(AnimatedSprite* sprite) {
	return getSpriteWidth(sprite->sprites[(int)sprite->current]);
}

int animatedSpriteGetHeight(AnimatedSprite* sprite) {
	return getSpriteHeight(sprite->sprites[(int)sprite->current]);
}

void animatedSpriteSetSpriteAtIndex(AnimatedSprite* sprite, int index, SpriteXPM* xpm) {
    if (sprite && index >= 0 && index < sprite->n) {
        sprite->sprites[index] = xpm;
    }
}
