#ifndef ENEMY_FIRE_SKULL_WALKING_H
#define ENEMY_FIRE_SKULL_WALKING_H

#include "enemy_fire_skull_1.xpm"
#include "enemy_fire_skull_2.xpm"
#include "enemy_fire_skull_3.xpm"
#include "enemy_fire_skull_4.xpm"
#include "enemy_fire_skull_5.xpm"
#include "enemy_fire_skull_6.xpm"
#include "enemy_fire_skull_7.xpm"
#include "enemy_fire_skull_8.xpm"
#include "enemy_fire_skull_9.xpm"
#include "enemy_fire_skull_10.xpm"
#include "enemy_fire_skull_11.xpm"
#include "enemy_fire_skull_12.xpm"
#include "enemy_fire_skull_13.xpm"
#include "enemy_fire_skull_14.xpm"


static xpm_map_t enemy_fire_skull_walking[] = {
    enemy_fire_skull_1, enemy_fire_skull_2, enemy_fire_skull_3,
    enemy_fire_skull_4, enemy_fire_skull_5, enemy_fire_skull_6,
    enemy_fire_skull_7, enemy_fire_skull_8, enemy_fire_skull_9,
    enemy_fire_skull_10, enemy_fire_skull_11, enemy_fire_skull_12,
    enemy_fire_skull_13, enemy_fire_skull_14
};

uint8_t* enemy_pixmaps[14];
xpm_image_t enemy_imgs[14];
uint8_t* enemy_pixmaps_test;
xpm_image_t enemy_imgs_test;

#endif
