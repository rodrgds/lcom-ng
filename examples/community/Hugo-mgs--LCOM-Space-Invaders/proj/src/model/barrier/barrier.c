#include "barrier.h"
#include "sprites/sprite.h"

Barrier *createBarrier(int x, int y) {
    Barrier *barrier = (Barrier *)malloc(sizeof(Barrier));
    if (barrier == NULL) {
        return NULL; // Memory allocation failed
    }
    barrier->x = x;
    barrier->y = y;
    barrier->lives = 4;
    barrier->sprite = barrier_st0;
    return barrier;
}

void destroyBarrier(Barrier *barrier) {
    if (barrier != NULL) {
        if (barrier->sprite != NULL) {
            destroy_sprite(barrier->sprite);
        }
        free(barrier);
    }
}

void drawBarrier(Barrier *barrier) {
    if (barrier != NULL && barrier->sprite != NULL) {
        draw_sprite(barrier->sprite, barrier->x, barrier->y);
    }
}

int damageBarrier(Barrier *barrier) {
    if (barrier != NULL) {
        barrier->lives--;
        if (barrier->lives <= 0) {
            return 1;
        } else if (barrier->lives == 3) {
            barrier->sprite = barrier_st1;
        } else if (barrier->lives == 2) {
            barrier->sprite = barrier_st2;
        } else if (barrier->lives == 1) {
            barrier->sprite = barrier_st3;
        }
    }
    return 0;
}
