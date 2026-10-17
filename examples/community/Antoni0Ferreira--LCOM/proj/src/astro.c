#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "astro.h"

int (initialize_astros)(struct astro astros[]){

	uint16_t yAstro = 50;
    xpm_map_t astroMap = asteroide_xpm;

	xpm_image_t img_1;
    astros[0].img = img_1;
    uint8_t *sprite_1 = xpm_load(astroMap,XPM_8_8_8,&astros[0].img);
    astros[0].sprite = sprite_1;

    xpm_image_t img_2;
    astros[1].img = img_2;
    uint8_t *sprite_2 = xpm_load(astroMap,XPM_8_8_8,&astros[1].img);
    astros[1].sprite = sprite_2;

    xpm_image_t img_3;
    astros[2].img = img_3;
    uint8_t *sprite_3 = xpm_load(astroMap,XPM_8_8_8,&astros[2].img);
    astros[2].sprite = sprite_3;

    xpm_image_t img_4;
    astros[3].img = img_4;
    uint8_t *sprite_4 = xpm_load(astroMap,XPM_8_8_8,&astros[3].img);
    astros[3].sprite = sprite_4;

    xpm_image_t img_5;
    astros[4].img = img_5;
    uint8_t *sprite_5 = xpm_load(astroMap,XPM_8_8_8,&astros[4].img);
    astros[4].sprite = sprite_5;

    xpm_image_t img_6;
    astros[5].img = img_6;
    uint8_t *sprite_6 = xpm_load(astroMap,XPM_8_8_8,&astros[5].img);
    astros[5].sprite = sprite_6;

    xpm_image_t img_7;
    astros[6].img = img_7;
    uint8_t *sprite_7 = xpm_load(astroMap,XPM_8_8_8,&astros[6].img);
    astros[6].sprite = sprite_7;

    xpm_image_t img_8;
    astros[7].img = img_8;
    uint8_t *sprite_8 = xpm_load(astroMap,XPM_8_8_8,&astros[7].img);
    astros[7].sprite = sprite_8;


    for(int i = 0; i < 8; i++){
        if((i % 2) == 0){
            astros[i].x = 0;
            srand(time(NULL));
            astros[i].speed = (rand() % (5 - 2)) + 2;
        }
        else { 
            astros[i].x = RIGHT_BORDER_LIMIT - astros[i].img.width;
            astros[i].speed = -((rand() % (5 - 2)) + 2);
        }
        astros[i].y = yAstro;
        yAstro += ( 50);


    }

    return 0;
}

void (move_astros)(struct astro astros[]){
    for (int i = 0; i < 8; i++) {
        astros[i].x += astros[i].speed;
        if (astros[i].x > RIGHT_BORDER_LIMIT && astros[i].speed > 0) {
        astros[i].x = 0;
        }
        if (astros[i].x > RIGHT_BORDER_LIMIT && astros[i].speed < 0) {
        astros[i].x = RIGHT_BORDER_LIMIT;
        }
    }
}

