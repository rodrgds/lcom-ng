#include "dynamite.h"

Dynamite* (createDynamite)(int x, int y, Direction dir) {
    Dynamite* dynamite = (Dynamite*)malloc(sizeof(Dynamite));

    dynamite->pos.x = x;
    dynamite->pos.y = y;
    dynamite->dir = dir;
    dynamite->active = true;
    dynamite->explosionFrame = 0;

    return dynamite;
}

int16_t (getDynamiteX)(Dynamite* dynamite) {
    return dynamite->pos.x;
}

int16_t (getDynamiteY)(Dynamite* dynamite) {
    return dynamite->pos.y;
}

void (destroyDynamite)(Dynamite* dynamite) {
    free(dynamite);
}
