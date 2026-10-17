#include "sprite.h"
#include <stdlib.h>

sprite *create_sprite(xpm_map_t xpm, int x, int y) {
    sprite *sp = malloc(sizeof(sprite));
    if (sp == NULL)
        return NULL;

    sp->colors = xpm_load(xpm, XPM_8_8_8_8, &sp->img);
    if (sp->colors == NULL) {
        free(sp);
        return NULL;
    }

    sp->x = x;
    sp->y = y;

    sp->width = sp->img.width;
    sp->height = sp->img.height;

    return sp;
}

void destroy_sprite(sprite **sp) {
    if (sp == NULL || *sp == NULL) return;
    if ((*sp)->colors != NULL) free((*sp)->colors);
    
    free(*sp);
    *sp = NULL;
}

void move_sprite(sprite *sp, int dx, int dy) {
    if (sp == NULL)
        return;
    sp->x += dx;
    sp->y += dy;
}
