#include "../include/button.h"
/** @file button.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/components/boxCollider.h"
#include "../include/utils/vec2d.h"
#include "../include/data.h"

#include <stdlib.h>

struct Button {
	AnimatedSprite* sprite;
    BoxCollider* bounds;
    int x;
    int y;

    ButtonClickFn onClick;
	ButtonHoverFn onHover;

	bool isPressed;
	bool isHovering;
};

Button* buttonNew(int x, int y, int width, int height, AnimatedSprite* sprite, ButtonClickFn onClick, ButtonHoverFn onHover) {
    Button* b = malloc(sizeof(Button));
	b->sprite = sprite;
    b->x = x;
    b->y = y;
    b->onClick = onClick;
	b->onHover = onHover;

	b->isPressed = false;
	b->isHovering = false;


	Vec2d pos = {x, y};
	Vec2d size = {width, height};
    b->bounds = boxColliderNew(pos, EMPTY_VEC2D, size, UI_TAG);

    return b;
}

void buttonFree(Button* b) {
    if (!b) return;
	animatedSpriteFree(b->sprite);
    boxColliderFree(b->bounds);
    free(b);
}

void buttonDraw(Button* b, Terminal* t, float deltaTime, bool isFlipped) {
	animatedSpriteUpdate(b->x, b->y, b->sprite, t, deltaTime, isFlipped);
}

void buttonClick(Button* button) {
	if (button->isPressed) return;
	button->onClick(button);
	button->isPressed = true;
}

void buttonHover(Button* button) {
	button->onHover(button);
}

void buttonReset(Button* button) {
	button->isPressed = false;
	button->isHovering = false;

	animatedSpriteReset(button->sprite);
}

BoxCollider* buttonGetBoxCollider(Button* button) {
	return button->bounds;
}

// Button Hover
void basicButtonHoverFn(Button* button) {
	if (button->isHovering) return;

	animatedSpriteStart(button->sprite);
	button->isHovering = true;
}

bool buttonIsFinished(Button* button) {
	return animatedSpriteIsFinished(button->sprite);
}

bool buttonContainsPoint(Button* b, int px, int py) {
    if (!b) return false;
    Vec2d p = {(float)px, (float)py};
    return boxIsCollidingWithPoint(b->bounds, p);
}

bool buttonIsPressed(Button* b) {
	return b->isPressed;
}
