#include <lcom/lcf.h>
#include "sprite.h"
#include "video_card.h"

extern uint32_t bitsPerPixel;



Sprite* create_sprite(xpm_map_t xpm, int x, int y) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
    if (sp == NULL)
        return NULL;

    enum xpm_image_type type = XPM_5_6_5;
    xpm_image_t img;
    sp->map = xpm_load(xpm, type, &img);

    sp->width = img.width;
    sp->height = img.height;

    if (sp->map == NULL) {
        free(sp);
        return NULL;
    }
    sp->x=x;
    sp->y=y;
    sp->xspeed=0;
    sp->yspeed=0;


    return sp;
}


int draw_sprite(Sprite *sp, uint8_t *base) {

    size_t aux = 0;
    uint32_t cor = 0;
    uint32_t bytes_per_pixel = bitsPerPixel / 8;

    for (int i = sp->y; i < sp->y + sp->height; i++) {
        for (int j = sp->x; j < sp->x + sp->width; j++) {
            if ((i < 0) || (j < 0) || (j > h_res) || (i > v_res)) {
                return 1;
            }
            memcpy(&cor, &sp->map[aux], bytes_per_pixel);
            if (cor != CHROMA_KEY_GREEN_565) // não fazer o display do fundo dos XPMs
                memcpy(base + (i * h_res + j) * bytes_per_pixel, &cor, bytes_per_pixel);
            aux += bytes_per_pixel;
        }
    }

    return 0;
}

int draw_sprite_with_color(Sprite *sp, uint32_t final_color, uint8_t *base) {

    size_t aux = 0;
    uint32_t bytes_per_pixel = bitsPerPixel / 8;
    uint32_t cor = 0;

    for (int i = sp->y; i < sp->y + sp->height; i++) {
        for (int j = sp->x; j < sp->x + sp->width; j++) {
            if ((i < 0) || (j < 0) || (j > h_res) || (i > v_res)) {
                return 1;
            }
            memcpy(&cor, &sp->map[aux], bytes_per_pixel);
            if (cor != CHROMA_KEY_GREEN_565) // não fazer o display do fundo dos XPMs
                memcpy(base + (i * h_res + j) * bytes_per_pixel, &final_color, bytes_per_pixel);
            aux += bytes_per_pixel;
        }
    }

    return 0;

}

