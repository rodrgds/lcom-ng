#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

typedef struct Sprite Sprite;

struct Sprite {
    int x, y;
    int width, height;
    int xspeed, yspeed;
    uint32_t* map;

    void (*destroy)(Sprite* self);
    int (*draw)(Sprite* self);
    void (*changeXPM)(Sprite* self, const char** xpm);
};

Sprite* create_sprite(const char* pic[], int x, int y, int xspeed, int yspeed);

#endif
