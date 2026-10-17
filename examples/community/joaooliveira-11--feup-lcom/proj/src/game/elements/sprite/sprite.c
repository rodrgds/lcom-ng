#include "sprite.h"
extern struct gamecontext context;

int sprite_build1(sprite_t* sprite, xpm_map_t xpm_){
    xpm_image_t img;
    sprite->xpm = (uint32_t *) xpm_load(xpm_, XPM_8_8_8_8, &img);
    
    if(sprite->xpm == NULL) return 1;

    sprite->height = img.height;
    sprite->width = img.width;

    return 0;
}

void set_sprite_xpos(sprite_t *sprite, uint16_t x){
    sprite->xpos = x;
}
void set_sprite_ypos(sprite_t *sprite, uint16_t y){
    sprite->ypos = y;
}
uint16_t get_sprite_xpos(sprite_t *sprite){
    return sprite->xpos;
}
uint16_t get_sprite_ypos(sprite_t *sprite){
    return sprite->ypos;
}

void (sprite_delete)(sprite_t * sprite){
    free(sprite->xpm);
}
