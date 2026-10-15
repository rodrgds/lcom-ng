#include <lcom/lcf.h>
#include <math.h>
#include <stdlib.h>
#include "sprite.h"
#include "../gpu.h"

static uint32_t transparent = 0xff000000;

static void destroy_sprite(Sprite* self) {
    if (!self) return;
    if (self->map) free(self->map);
    free(self);
}

static int draw_sprite(Sprite* self) {
    if (!self) return 1;

    for (int i = 0; i < self->height; i++) {
        for (int j = 0; j < self->width; j++) {
            uint32_t color = self->map[i * self->width + j];
            if (color == transparent)
                continue;
            vg_draw_pixel(self->x + j, self->y + i, color);
        }
    }
    return 0;
}

static void changeXPM(Sprite* self, const char** xpm) {
    xpm_image_t img;
    free(self->map);
    self->map = (uint32_t*) xpm_load(xpm, XPM_8_8_8_8, &img);
    self->width  = img.width;
    self->height = img.height;
}

Sprite* create_sprite(const char* pic[], int x, int y, int xspeed, int yspeed) {
    Sprite* sp = (Sprite*) malloc(sizeof(Sprite));
    xpm_image_t img;
    if (!sp) return NULL;

    sp->map = (uint32_t*) xpm_load(pic, XPM_8_8_8_8, &img);
    transparent = xpm_transparency_color(XPM_8_8_8_8);

    if (!sp->map) {
        free(sp);
        return NULL;
    }

    sp->width = img.width;
    sp->height = img.height;
    sp->x = x;
    sp->y = y;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;

    sp->destroy = destroy_sprite;
    sp->draw = draw_sprite;
    sp->changeXPM = changeXPM;

    return sp;
}
