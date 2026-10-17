#include "apple.h"

apple* apple1;

void (create_apple)(){
    apple1 = malloc(sizeof(apple));
    apple1->x = 600;
    apple1->y = 500;
    apple1->counter=0;
    xpm_load(apple_xpm,XPM_8_8_8,&apple1->img);
}


void (draw_apple)(){
    apple1->counter++;
    if(apple1->counter%60==0){
        move_apple();
    }
    draw_xpm(apple1->img.bytes,&apple1->img,apple1->x,apple1->y);
    
}

void(move_apple)(){
    apple1->x = (rand() % 40) * 20;
    apple1->y = ((rand() % 22) + 8) * 20;
}


