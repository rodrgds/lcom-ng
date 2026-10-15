#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "asprite.h"

ASprite* create_asprite(int num_pic, xpm_map_t pics[], int aspeed, int x, int y, int xspeed, int yspeed){

    if (num_pic <= 0) return NULL;
    ASprite* asp = (ASprite*) malloc(sizeof(ASprite));
    if (asp == NULL) return NULL;
    asp->aspeed = aspeed;
    asp->num_fig = num_pic;
    asp->frames_left = aspeed;
    asp->cur_fig = 0;
    asp->sp = create_sprite(pics[0], x, y, xspeed, yspeed);
    if (asp->sp == NULL) {
        free(asp);
        return NULL;
    }
    asp->map = (uint8_t**) calloc(num_pic, sizeof(uint8_t*));
    if (asp->map == NULL) {
        destroy_sprite(asp->sp);
        free(asp);
        return NULL;
    }
    /* The animation owns every frame. Its drawable sprite borrows one frame. */
    asp->map[0] = asp->sp->map;
    for (int i = 1; i < num_pic; ++i) {
        xpm_image_t img;
        asp->map[i] = xpm_load(pics[i], get_type(), &img);
        if (asp->map[i] == NULL) {
            destroy_asprite(asp);
            return NULL;
        }
    }
    return asp;
}

ASprite* create_portal(portal_type type, int x, int y){
    xpm_row_t* pics[4];
    switch (type){
        case PURPLE_LEFT:{
            pics[0] = purple_1_left;
            pics[1] = purple_2_left;
            pics[2] = purple_3_left;
            pics[3] = purple_4_left;
            break;
        }
        case PURPLE_RIGHT:{
            pics[0] = purple_1_right;
            pics[1] = purple_2_right;
            pics[2] = purple_3_right;
            pics[3] = purple_4_right;
            break;
        }
        case PURPLE_DOWN:{
            pics[0] = purple_1_down;
            pics[1] = purple_2_down;
            pics[2] = purple_3_down;
            pics[3] = purple_4_down;
            break;
        }
        case PURPLE_UP:{
            pics[0] = purple_1_up;
            pics[1] = purple_2_up;
            pics[2] = purple_3_up;
            pics[3] = purple_4_up;
            break;
        }
        case GREEN_LEFT:{
            pics[0] = green_1_left;
            pics[1] = green_2_left;
            pics[2] = green_3_left;
            pics[3] = green_4_left;
            break;
        }
        case GREEN_RIGHT:{
            pics[0] = green_1_right;
            pics[1] = green_2_right;
            pics[2] = green_3_right;
            pics[3] = green_4_right;
            break;
        }
        case GREEN_DOWN:{
            pics[0] = green_1_down;
            pics[1] = green_2_down;
            pics[2] = green_3_down;
            pics[3] = green_4_down;
            break;
        }
        case GREEN_UP:{
            pics[0] = green_1_up;
            pics[1] = green_2_up;
            pics[2] = green_3_up;
            pics[3] = green_4_up;
            break;
        }
    }

    ASprite* portal = create_asprite(4, pics, 15, x, y, 0, 0);
    return portal;
}

void destroy_asprite(ASprite* asp){
    if (asp == NULL) return;
    if (asp->map) {
        for (int i = 0; i < asp->num_fig; ++i) free(asp->map[i]);
        free(asp->map);
        if (asp->sp) asp->sp->map = NULL;
    }
    destroy_sprite(asp->sp);
    free(asp);
    asp = NULL;
}

void animate_asprite(ASprite* asp){
    asp->frames_left--;
    if (asp->frames_left == 0){
        asp->frames_left = asp->aspeed;
        asp->cur_fig++;
        if (asp->cur_fig == asp->num_fig) asp->cur_fig = 0;
        asp->sp->map = asp->map[asp->cur_fig];
        
    }
    draw_sprite(asp->sp);
}
