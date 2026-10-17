#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "points.h"

xpm_image_t img0;
uint8_t *map0;
xpm_map_t xpm0 = zero_xpm;

xpm_image_t img1;
uint8_t *map1;
xpm_map_t xpm1 = one_xpm;

xpm_image_t img2;
uint8_t *map2;
xpm_map_t xpm2 = two_xpm;

xpm_image_t img3;
uint8_t *map3;
xpm_map_t xpm3 = three_xpm;

xpm_image_t img4;
uint8_t *map4;
xpm_map_t xpm4 = four_xpm;

xpm_image_t img5;
uint8_t *map5;
xpm_map_t xpm5 = five_xpm;

xpm_image_t img6;
uint8_t *map6;
xpm_map_t xpm6 = six_xpm;

xpm_image_t img7;
uint8_t *map7;
xpm_map_t xpm7 = seven_xpm;

xpm_image_t img8;
uint8_t *map8;
xpm_map_t xpm8 = eight_xpm;

xpm_image_t img9;
uint8_t *map9;
xpm_map_t xpm9 = nine_xpm;



int (initialize_points)(){
    map0 = xpm_load(xpm0,XPM_8_8_8,&img0);
    maps[0] = map0; imgs[0] = img0;

    map1 = xpm_load(xpm1,XPM_8_8_8,&img1);
    maps[1] = map1; imgs[1] = img1;

    map2 = xpm_load(xpm2,XPM_8_8_8,&img2);
    maps[2] = map2; imgs[2] = img2;

    map3 = xpm_load(xpm3,XPM_8_8_8,&img3);
    maps[3] = map3; imgs[3] = img3;

    map4 = xpm_load(xpm4,XPM_8_8_8,&img4);
    maps[4] = map4; imgs[4] = img4;

    map5 = xpm_load(xpm5,XPM_8_8_8,&img5);
    maps[5] = map5; imgs[5] = img5;

    map6 = xpm_load(xpm6,XPM_8_8_8,&img6);
    maps[6] = map6; imgs[6] = img6;

    map7 = xpm_load(xpm7,XPM_8_8_8,&img7);
    maps[7] = map7; imgs[7] = img7;

    map8 = xpm_load(xpm8,XPM_8_8_8,&img8);
    maps[8] = map8; imgs[8] = img8;

    map9 = xpm_load(xpm9,XPM_8_8_8,&img9);
    maps[9] = map9; imgs[9] = img9;

    vg_draw_sprite(maps[0], POINTS_X, POINTS_Y, imgs[0]);
    return 0;
}

int (draw_points)(int points){

    int auxP = points;
    uint16_t x = (uint16_t)110, y = (uint16_t)540;

    do{
        int mod = auxP % 10;
        vg_draw_sprite(maps[mod], x, y, imgs[mod]);
        auxP /= 10;
        x -= 40;
    }while(auxP != 0);
    return 0;
}

