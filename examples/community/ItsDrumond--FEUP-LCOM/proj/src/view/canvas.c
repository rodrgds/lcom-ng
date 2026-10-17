#include <lcom/lcf.h>

#include "canvas.h"
#include "devices/video.h"
#include "devices/keyboard.h"
#include "xpm/xpm_id.h"

uint32_t walls_color;
uint32_t arena_color;


int (canvas_draw_menu)(Mouse *mouse){
    vg_draw_xpm(TITLE_XPM_ID, 310, 200);

    if(mouse->x >= 500 && mouse->x <=700 && mouse->y >= 400 && mouse->y <= 500){
        vg_draw_xpm(START_YES_XPM_ID,500,400);
        vg_draw_xpm(INSTRUCT_NO_XPM_ID,400, 600);
    }
    else if(mouse->x >= 400 && mouse->x <=800 && mouse->y >= 600 && mouse->y <= 680){
        vg_draw_xpm(START_NO_XPM_ID,500,400);
        vg_draw_xpm(INSTRUCT_YES_XPM_ID,400, 600);
    }
    else{
        vg_draw_xpm(START_NO_XPM_ID,500,400);
        vg_draw_xpm(INSTRUCT_NO_XPM_ID,400, 600);
    }

    return 0;
}

int (canvas_draw_instructions)(Mouse *mouse){
    vg_draw_xpm(TITLE_XPM_ID, 310, 200);
    vg_draw_xpm(INSTRUCTEXT_XPM_ID,400,400);
    if(mouse->x >= 450 && mouse->x <=650 && mouse->y >= 900 && mouse->y <= 1000){
        vg_draw_xpm(BACK_YES_XPM_ID,500, 900);
    }
    else{
        vg_draw_xpm(BACK_NO_XPM_ID,500, 900);
    }
    return 0;
}



int (canvas_draw_game_over)(Mouse *mouse){
    vg_draw_xpm(TITLE_XPM_ID, 310, 200);
    vg_draw_xpm(GAMEOVER_XPM_ID, 400, 400);
    if(mouse->x >= 400 && mouse->x <=750 && mouse->y >= 600 && mouse->y <= 700){
        vg_draw_xpm(PLAYAGAIN_YES_XPM_ID, 400, 600);
    }
    else{
        vg_draw_xpm(PLAYAGAIN_NO_XPM_ID, 400, 600);
    }
    return 0;
}

void (draw_background)(bool day){
    if(day)
        vg_draw_xpm(BACKGROUND_DAY_XPM_ID,0,0);
    else
        vg_draw_xpm(BACKGROUND_NIGHT_XPM_ID,0,0);
}

void draw_mouse(Mouse *mouse){
    vg_draw_xpm(TARGET_XPM_ID, mouse->x-18, mouse->y-18);
}

int draw_numbers(uint8_t number,uint16_t x, uint16_t y){
    if(number < 0 || number > 60) return 1;
    uint8_t units = number%10;
    uint8_t dozens = number/10;
    uint8_t numbers [10] = {32,33,34,35,36,37,38,39,40,41};

    if(vg_draw_xpm(numbers[dozens],x,y)) return 1;
    if(vg_draw_xpm(numbers[units],x + 70,y)) return 1;

    return 0;
}

int draw_month(uint8_t month, uint16_t x, uint16_t y){
    if( month < 1 || month > 12) return 1;
    uint8_t months [12] = {43,44,45,46,47,48,49,50,51,52,53,54};

    if(vg_draw_xpm(months[month-1],x,y)) return 1;

    return 0; 
}

int draw_double_dots(uint16_t x, uint16_t y){
    if(vg_draw_xpm(DOUBLE_DOTS,x,y)) return 1;
    return 0;

}


int draw_date(uint8_t hours, uint8_t minutes,uint8_t seconds, uint8_t day, uint8_t month){
    if(draw_numbers(hours,100,800)) return 1;
    if(draw_double_dots(250,800)) return 1;
    if(draw_numbers(minutes,320,800)) return 1;
    if(draw_double_dots(470,800)) return 1;
    if(draw_numbers(seconds,540,800)) return 1;
    if(draw_numbers(day, 800, 800)) return 1;
    if(draw_month(month, 950, 790)) return 1;
    return 0;
}

