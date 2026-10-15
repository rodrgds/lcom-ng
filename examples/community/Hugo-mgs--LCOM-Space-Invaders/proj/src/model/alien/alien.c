#include "alien.h"

Alien *createAlien(int x, int y, int lives, Sprite *sprite) {
    Alien *alien = (Alien *)malloc(sizeof(Alien));
    if (alien == NULL) {
        return NULL; // Memory allocation failed
    }
    alien->x = x;
    alien->y = y;
    alien->lives = lives;
    alien->sprite = sprite;
    return alien;
}

void destroyAlien(Alien *alien) {
    if (alien != NULL) {
        free(alien);
    }
}

void drawAlien(Alien *alien) {
    if (alien != NULL && alien->sprite != NULL) {
        draw_sprite(alien->sprite, alien->x, alien->y);
    }
}

void alienMove(Alien *alien, int direction) {
    int delta_x = 0, delta_y = 0;
    switch (direction) {
        case 0: // left
            delta_x = -20;
            break;
        case 1: // down
            delta_y = 20;
            break;
        case 2: // right
            delta_x = 20;
            break;
        case 3: // down
            delta_y = 20;
            break;    
        default:
            return; // Invalid direction
    }
    alien->x += delta_x;
    alien->y += delta_y;

    if (alien->x < 0) alien->x = 0;
    if (alien->x > 500 - alien->sprite->width) alien->x = 500 - alien->sprite->width;
    if (alien->y < 0) alien->y = 0;
    // PODE ALTERAR
    if (alien->y > 800 - alien->sprite->height) alien->y = 800 - alien->sprite->height;
}

int damageAlien(Alien *alien) {
    if (alien != NULL) {
        alien->lives--;
        if (alien->lives <= 0) {
            return 1;
        }
    }
    return 0;
}
