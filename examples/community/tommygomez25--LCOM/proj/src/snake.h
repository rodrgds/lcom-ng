#pragma once
#include <lcom/lcf.h>
#include "sprites/snakebodyhorizontal.xpm"
#include "sprites/snakebodyvertical.xpm"
#include "sprites/snakeheaddown.xpm"
#include "sprites/snakeheadleft.xpm"
#include "sprites/snakeheadright.xpm"
#include "sprites/snakeheadup.xpm"
#include "sprites/snaketaildown.xpm"
#include "sprites/snaketailleft.xpm"
#include "sprites/snaketailright.xpm"
#include "sprites/snaketailup.xpm"
#include "sprites/snaketurndownleft.xpm"
#include "sprites/snaketurndownright.xpm"
#include "sprites/snaketurnupleft.xpm"
#include "sprites/snaketurnupright.xpm"
#include "i8042.h"
#include "apple.h"

xpm_image_t snakebodyhorizontal_img,snakebodyvertical_img;
xpm_image_t snakeheaddown_img,snakeheadright_img,snakeheadup_img,snakeheadleft_img;
xpm_image_t snaketaildown_img,snaketailleft_img,snaketailright_img,snaketailup_img;
xpm_image_t snaketurndownleft_img,snaketurndownright_img,snaketurnupleft_img,snaketurnupright_img;

typedef struct {
    int x;   
    int y;    
    //xpm_row_t *xmap; /* snake é estruturado por xmp rows */
    xpm_image_t img;
} snakepart;

void (create_snake)();

void (create_snake_tail)();

void (create_snake_body)();

void (create_snake_head)();

void (draw_snake)();

void (check_snake_apple_collision)(apple* apple1);

void (define_snake_tail_sprite)();

void snake_set_direction(uint8_t key);

void(snake_update_movement)();

void (define_snake_body_sprite)();


