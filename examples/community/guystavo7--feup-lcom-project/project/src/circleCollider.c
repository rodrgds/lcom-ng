#include "../include/components/circleCollider.h"
/** @file circleCollider.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include <stdlib.h>

struct CircleCollider {
    int x;
    int y;

    int radius;
    int tag;
};

CircleCollider* circleColliderNew(int x, int y, int radius, int tag) {
    CircleCollider* res = malloc(sizeof(CircleCollider));
    res->x = x;
    res->y = y;
    res->radius = radius;
    res->tag = tag;

    return res;
}

void circleColliderFree(CircleCollider* c) {
    free(c);
}

bool circleIsCollidingWithPoint(CircleCollider* c, int x, int y) {
    return (x - c->x) * (x - c->x) + (y - c->y) * (y - c->y) <= c->radius * c->radius;
}

void circleColliderUpdate(CircleCollider* c, int x, int y) {
    c->x = x;
    c->y = y;
}

int circleColliderGetTag(CircleCollider* c) {
    return c->tag;
}
