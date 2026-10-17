#include <lcom/lcf.h>

#include "piece.h"

int piece_1_x = 30;
int piece_1_y = 255;
int piece_2_x = 757;
int piece_2_y = 255;
extern bool catched_left;
extern bool catched_right;
extern int timer_irq_counter;
extern int powerup_number;

int move_piece1_up(){
    if (catched_right && powerup_number == 0 && timer_irq_counter<=1000){
        piece_1_y-=10;
    }
    else {
        piece_1_y-=20;
    }
    return 0;
}

int move_piece1_down(){
    if (catched_right && powerup_number == 0 && timer_irq_counter<=1000){
        piece_1_y+=10;
    }
    else {
        piece_1_y+=20;
    }
    return 0;
}

int move_piece2_up(){
    if (catched_left && powerup_number == 0 && timer_irq_counter<=1000){
        piece_2_y-=10;
    }
    else {
        piece_2_y-=20;
    }
    return 0;
}

int move_piece2_down(){
    if (catched_left && powerup_number == 0 && timer_irq_counter<=1000){
        piece_2_y+=10;
    }
    else {
        piece_2_y+=20;
    }
    return 0;
}

int draw_piece_1(){
    if (draw_xpm((xpm_map_t) piece_xpm,piece_1_x,piece_1_y)) return 1; 
    return 0;
}

int draw_piece_2(){
    if (draw_xpm((xpm_map_t) piece_xpm,piece_2_x,piece_2_y)) return 1;
    return 0;
}
