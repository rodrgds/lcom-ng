#include "objects.h"
#include "snake.h"
#include "graphics.h"
#include "macros.h"
#include <time.h>
#include "apple.h"
#include "images/maca.xpm"
#include "images/maca_castanha.xpm"
#include "images/maca_preta.xpm"

void initAppleXPM(){
    redAppleXpm.map = xpm_load((xpm_map_t)maca_xpm, XPM_8_8_8, &(redAppleXpm.img));
    blackAppleXpm.map = xpm_load((xpm_map_t)maca_preta_xpm, XPM_8_8_8, &(blackAppleXpm.img));
    brownAppleXpm.map = xpm_load((xpm_map_t)maca_castanha_xpm, XPM_8_8_8, &(brownAppleXpm.img));
}

void initApple(Apple *apple, int x_pos, int y_pos, enum appleType type){
    apple->type = type;
    apple->appleXpm = get_xpm(type);
    apple->x_pos = x_pos;
    apple->y_pos = y_pos;

    applesArray[numApples] = *apple;
    numApples++;

}

void initRandomApple(Apple *apple, enum appleType type){
    apple->type = type;
    apple->appleXpm = get_xpm(type);

    int x;
    int y;
    do {
        srand(time(0));
        x = ((rand() % ((h_res - 40 + 1 - 40) / 40)) * 40 )+ 40;
        srand(time(0));
        y = ((rand() % ((v_res - 40 + 1 - 120) / 40)) * 40 )+ 120;

    } while (isApple(x,y) != -1 || isSnake(x,y) == 1);


    apple->x_pos = x;
    apple->y_pos = y;

    applesArray[numApples] = *apple;
    numApples++;

}

object get_xpm(enum appleType type){
    switch (type) {
        case red: return redAppleXpm;
        case brown: return brownAppleXpm;
        case black: return blackAppleXpm;
    }
}

void drawApple(Apple apple){
    draw_xpm(apple.appleXpm.img, apple.appleXpm.map, apple.x_pos, apple.y_pos);
}


int isApple(int x, int y) {
    for (int i = 0; i < numApples; i++) {
        if (applesArray[i].x_pos == x && applesArray[i].y_pos == y) {
            return i;
        }
    }
    return -1;
}

void placeApple(int x, int y, enum appleType type){
    Apple apple;
    int h=x/40;
    int v=y/40;
    int x_final, y_final;

    if(x%40>=20)
        x_final=h*40+40;
    else
        x_final=h*40;
    if(v%40>=20)
        y_final=v*40+40;
    else
        y_final=v*40;
    if(isApple(x_final, y_final)!=-1 || isSnake(x_final, y_final))
        return;
    initApple(&apple, x_final, y_final, type);
    drawApple(apple);
}
