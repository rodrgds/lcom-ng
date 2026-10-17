#include "grass.h"

grass* grass1;

void (create_grass)(){
    grass1 = malloc(sizeof(grass));
    grass1->x = 0;
    grass1->y = 150;
    xpm_load(grass_xpm,XPM_8_8_8,&grass1->img);
}

void (draw_grass)(){
    draw_xpm(grass1->img.bytes,&grass1->img,grass1->x,grass1->y);
}

