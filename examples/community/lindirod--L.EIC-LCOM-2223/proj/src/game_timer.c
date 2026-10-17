// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>

#include "menu.h"
#include "vbe_graphics.h"

#include "xpm/numbers/zero.xpm"
#include "xpm/numbers/one.xpm"
#include "xpm/numbers/two.xpm"
#include "xpm/numbers/three.xpm"
#include "xpm/numbers/four.xpm"
#include "xpm/numbers/five.xpm"
#include "xpm/numbers/six.xpm"
#include "xpm/numbers/seven.xpm"
#include "xpm/numbers/eight.xpm"
#include "xpm/numbers/nine.xpm"
#include "xpm/numbers/colon.xpm"


int game_timer;

xpm_image_t zero, one, two, 
            three, four, five,
            six, seven, eight, nine, colon;

xpm_image_t *zero_draw = NULL;
xpm_image_t *one_draw = NULL;
xpm_image_t *two_draw = NULL;
xpm_image_t *three_draw = NULL;
xpm_image_t *four_draw = NULL;
xpm_image_t *five_draw = NULL;
xpm_image_t *six_draw = NULL;
xpm_image_t *seven_draw = NULL;
xpm_image_t *eight_draw = NULL;
xpm_image_t *nine_draw = NULL;
xpm_image_t *colon_draw = NULL;



void (timer_files_init)(){
    zero.bytes = xpm_load(zero_xpm, XPM_8_8_8, &zero);
    one.bytes = xpm_load(one_xpm, XPM_8_8_8, &one);
    two.bytes = xpm_load(two_xpm, XPM_8_8_8, &two);
    three.bytes = xpm_load(three_xpm, XPM_8_8_8, &three);
    four.bytes = xpm_load(four_xpm, XPM_8_8_8, &four);
    five.bytes = xpm_load(five_xpm, XPM_8_8_8, &five);
    six.bytes = xpm_load(six_xpm, XPM_8_8_8, &six);
    seven.bytes = xpm_load(seven_xpm, XPM_8_8_8, &seven);
    eight.bytes = xpm_load(eight_xpm, XPM_8_8_8, &eight);
    nine.bytes = xpm_load(nine_xpm, XPM_8_8_8, &nine);
    colon.bytes = xpm_load(colon_xpm, XPM_8_8_8, &colon);
}


int game_counter(){
    zero_draw = &zero;
    one_draw = &one;
    two_draw = &two;
    three_draw = &three;
    four_draw = &four;
    five_draw = &five;
    six_draw = &six;
    seven_draw = &seven;
    eight_draw = &eight;
    nine_draw = &nine;
    colon_draw = &colon;

    int number;
    int x = 450;
    int y = 530;
    int timerCounter = game_timer;

    // Calculate minutes and seconds
    int minutes = timerCounter / 60;
    int seconds = timerCounter % 60;

    // Draw minutes
    number = minutes / 10;
    switch(number){
        case 0:
            draw_xpm(zero_draw, x, y);
            break;
        case 1:
            draw_xpm(one_draw, x, y);
            break;
        case 2:
            draw_xpm(two_draw, x, y);
            break;
        default:
            break;
    }
    x += 20;

    number = minutes % 10;
    switch(number){
        case 0:
            draw_xpm(zero_draw, x, y);
            break;
        case 1:
            draw_xpm(one_draw, x, y);
            break;
        case 2:
            draw_xpm(two_draw, x, y);
            break;
        case 3:
            draw_xpm(three_draw, x, y);
            break;
        case 4:
            draw_xpm(four_draw, x, y);
            break;
        case 5:
            draw_xpm(five_draw, x, y);
            break;
        case 6:
            draw_xpm(six_draw, x, y);
            break;
        case 7:
            draw_xpm(seven_draw, x, y);
            break;
        case 8:
            draw_xpm(eight_draw, x, y);
            break;
        case 9:
            draw_xpm(nine_draw, x, y);
            break;
        default:
            break;
    }
    x += 20;

    // Draw colon
    draw_xpm(colon_draw, x, y);
    x += 20;

    // Draw seconds
    number = seconds / 10;
    switch(number){
        case 0:
            draw_xpm(zero_draw, x, y);
            break;
        case 1:
            draw_xpm(one_draw, x, y);
            break;
        case 2:
            draw_xpm(two_draw, x, y);
            break;
        case 3:
            draw_xpm(three_draw, x, y);
            break;
        case 4:
            draw_xpm(four_draw, x, y);
            break;
        case 5:
            draw_xpm(five_draw, x, y);
            break;
        default:
            break;
    }
    x += 20;

    number = seconds % 10;
    switch(number){
        case 0:
            draw_xpm(zero_draw, x, y);
            break;
        case 1:
            draw_xpm(one_draw, x, y);
            break;
        case 2:
            draw_xpm(two_draw, x, y);
            break;
        case 3:
            draw_xpm(three_draw, x, y);
            break;
        case 4:
            draw_xpm(four_draw, x, y);
            break;
        case 5:
            draw_xpm(five_draw, x, y);
            break;
        case 6:
            draw_xpm(six_draw, x, y);
            break;
        case 7:
            draw_xpm(seven_draw, x, y);
            break;
        case 8:
            draw_xpm(eight_draw, x, y);
            break;
        case 9:
            draw_xpm(nine_draw, x, y);
            break;
        default:
            break;
    }
 
    return 0;
}

