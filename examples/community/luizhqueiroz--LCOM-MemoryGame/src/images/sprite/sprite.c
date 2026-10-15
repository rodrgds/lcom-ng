#include "sprite.h"

Sprite *create_sprite(xpm_map_t xmap) {
    
    Sprite *sp = (Sprite *) malloc (sizeof(Sprite));

    xpm_image_t img;

    if (sp == NULL) {
        return NULL;
    }
    
    sp->map = (uint32_t *) xpm_load(xmap, XPM_8_8_8_8, &img);

    if (sp->map == NULL) {
        free(sp);
        return NULL;
    }

    sp->width = img.width; 
    sp->height = img.height;
    
    return sp;
}


void destroy_sprite(Sprite *sp) {
    if (sp == NULL) {
        return;
    }

    if (sp->map) {
        free(sp->map);
    }

    free(sp);
    sp = NULL;
}

int display_sprite(Sprite *sprite, int x, int y) { 

    for (int j = 0 ; j < sprite->height ; j++) {
      for (int i = 0 ; i < sprite->width ; i++) {
        if (vg_draw_pixel(x + i, y + j, sprite->map[i + j*sprite->width])) {
            return 1;
        }
      }
    }

    return 0; 
}
