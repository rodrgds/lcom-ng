#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "boost.h"

extern int counter_boost;
extern int draw_b;

uint16_t static y_boost_positions[8] = {10, 80, 130, 180, 230, 280, 330, 380};

int (initialize_boost)(struct boost *boost){
    uint16_t xBoost = 50, yBoost = 10;
	xpm_image_t imgBoost;
    boost->img = imgBoost;
    xpm_map_t boostMap = star_xpm;
    boost->sprite = xpm_load(boostMap, XPM_8_8_8, &boost->img);
    boost->x = xBoost; boost->y = yBoost;

    return 0;
}

int (draw_boost)(struct boost *boost, uint8_t *bgMap){

    counter_boost++;
    printf("counter boost - %u\n", counter_boost);

    vg_draw_sprite(boost->sprite,boost->x,boost->y,boost->img);

    if(counter_boost == 3 * 60){
        counter_boost = 0;
        draw_b = 0;
        vg_destroy_sprite(boost->x,boost->y,boost->img,bgMap);

        srand(time(NULL));
        uint8_t next_pos_x = rand() % 56;
        uint8_t next_pos_y = rand() % 8;
        boost->x = 10 * next_pos_x;
        boost->y = y_boost_positions[next_pos_y];

    }

    return 0;
    
}
