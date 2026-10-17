#include "target.h"

Target* (createTarget)(int x, int y, Direction dir) {
    Target* target = (Target*)malloc(sizeof(Target));

    target->pos.x = x;
    target->pos.y = y;
    target->dir = dir;
    target->active = true;
    target->fallCounter = 0;

    return target;
}

int16_t (getTargetX)(Target* target) {
    return target->pos.x;
}

int16_t (getTargetY)(Target* target) {
    return target->pos.y;
}

void (destroyTarget)(Target* target) {
    free(target);
}
