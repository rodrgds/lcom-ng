#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "shots.h"

int (initialize_shots)(struct shot shots[], struct shot_animation shot_animations[]){
    xpm_map_t shot1 = shot1_xpm;
    xpm_map_t shot2 = shot2_xpm;
    xpm_map_t shot3 = shot3_xpm;
    xpm_map_t shot4 = shot4_xpm;
    xpm_map_t shot5 = shot5_xpm;
    xpm_map_t shot6 = shot6_xpm;
    xpm_map_t shot_asset = shot_asst_xpm;
    xpm_map_t shot_exp1 = shot_exp1_xpm;
    xpm_map_t shot_exp2 = shot_exp2_xpm;
    xpm_map_t shot_exp3 = shot_exp3_xpm;
    xpm_map_t shot_exp4 = shot_exp4_xpm;
    xpm_map_t shot_exp5 = shot_exp5_xpm;

    xpm_image_t shot_img_1;
    shot_animations[0].img = shot_img_1;
    uint8_t *shot_sprite_1 = xpm_load(shot1,XPM_8_8_8,&shot_animations[0].img);
    shot_animations[0].sprite = shot_sprite_1;

    xpm_image_t shot_img_2;
    shot_animations[1].img = shot_img_2;
    uint8_t *shot_sprite_2 = xpm_load(shot2,XPM_8_8_8,&shot_animations[1].img);
    shot_animations[1].sprite = shot_sprite_2;

    xpm_image_t shot_img_3;
    shot_animations[2].img = shot_img_3;
    uint8_t *shot_sprite_3 = xpm_load(shot3,XPM_8_8_8,&shot_animations[2].img);
    shot_animations[2].sprite = shot_sprite_3;

    xpm_image_t shot_img_4;
    shot_animations[3].img = shot_img_4;
    uint8_t *shot_sprite_4 = xpm_load(shot4,XPM_8_8_8,&shot_animations[3].img);
    shot_animations[3].sprite = shot_sprite_4;

    xpm_image_t shot_img_5;
    shot_animations[4].img = shot_img_5;
    uint8_t *shot_sprite_5 = xpm_load(shot5,XPM_8_8_8,&shot_animations[4].img);
    shot_animations[4].sprite = shot_sprite_5;

    xpm_image_t shot_img_6;
    shot_animations[5].img = shot_img_6;
    uint8_t *shot_sprite_6 = xpm_load(shot6,XPM_8_8_8,&shot_animations[5].img);
    shot_animations[5].sprite = shot_sprite_6;

    xpm_image_t shot_img_7;
    shot_animations[6].img = shot_img_7;
    uint8_t *shot_sprite_7 = xpm_load(shot_asset,XPM_8_8_8,&shot_animations[6].img);
    shot_animations[6].sprite = shot_sprite_7;

    xpm_image_t shot_img_8;
    shot_animations[7].img = shot_img_8;
    uint8_t *shot_sprite_8 = xpm_load(shot_exp1,XPM_8_8_8,&shot_animations[7].img);
    shot_animations[7].sprite = shot_sprite_8;

    xpm_image_t shot_img_9;
    shot_animations[8].img = shot_img_9;
    uint8_t *shot_sprite_9 = xpm_load(shot_exp2,XPM_8_8_8,&shot_animations[8].img);
    shot_animations[8].sprite = shot_sprite_9;

    xpm_image_t shot_img_10;
    shot_animations[9].img = shot_img_10;
    uint8_t *shot_sprite_10 = xpm_load(shot_exp3,XPM_8_8_8,&shot_animations[9].img);
    shot_animations[9].sprite = shot_sprite_10;

    xpm_image_t shot_img_11;
    shot_animations[10].img = shot_img_11;
    uint8_t *shot_sprite_11 = xpm_load(shot_exp4,XPM_8_8_8,&shot_animations[10].img);
    shot_animations[10].sprite = shot_sprite_11;

    xpm_image_t shot_img_12;
    shot_animations[11].img = shot_img_12;
    uint8_t *shot_sprite_12 = xpm_load(shot_exp5,XPM_8_8_8,&shot_animations[11].img);
    shot_animations[11].sprite = shot_sprite_12;

    for(int i = 0; i < 20; i++){
        shots[i].animation = -1;
        shots[i].x = -100;
        shots[i].y = -100;
    }
    return 0;
}
