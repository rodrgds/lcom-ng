#include "sprite.h"

Sprite* (create_sprite)(xpm_map_t sprite) {
    Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
    xpm_image_t img;
    if (sp == NULL) return NULL;
    sp->map = (uint32_t *) xpm_load(sprite, XPM_8_8_8_8, &img);
    sp->height = img.height;
    sp->width = img.width;
    if (sp->map == NULL) {
        free(sp);
        return NULL;
    }
    return sp;
}

void (destroy_sprite)(Sprite *sprite) {
    if (sprite == NULL) return;
    if (sprite->map) free(sprite->map);
    free(sprite);
    sprite = NULL;
}
