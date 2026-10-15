#include "sprite.h"

Sprite *create_sprite(const char *pic[], int x, int y, int xspeed, int yspeed) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    xpm_image_t img;
    if( sp == NULL )
        return NULL;

    sp->map = xpm_load(pic, XPM_8_8_8, &img);
    
    if( sp->map == NULL ) {
        free(sp);
        return NULL;
    }

    sp->x = x;
    sp->y = y;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;
    sp->width = img.width;
    sp->height = img.height;

    return sp;
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL ) return;
        
    if(sp->map) free(sp->map);
        
    free(sp);
    sp = NULL;
}
    
int draw_sprite(Sprite *sp, uint8_t *base) {
    if (!sp || !base) return 1;

    for (int row = 0; row < sp->height; row++) {
        for (int col = 0; col < sp->width; col++) {
            // Cada pixel tem 3 bytes (R, G, B)
            uint8_t *color = &sp->map[(row * sp->width + col) * 3];

            // Se for transparente, salta
            if (color[0] == 0 && color[1] == 0 && color[2] == 0) continue;

            uint16_t x = sp->x + col;
            uint16_t y = sp->y + row;

            if (x >= mode_info.XResolution || y >= mode_info.YResolution)
                continue;

            unsigned int idx = (y * mode_info.XResolution + x) * 3;
            base[idx + 0] = color[0];
            base[idx + 1] = color[1];
            base[idx + 2] = color[2];
        }
    }

    return 0;
}

