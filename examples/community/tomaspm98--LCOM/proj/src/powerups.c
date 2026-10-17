#include "powerups.h"

int powerup_x;
int powerup_y;
bool powerup;
bool no_pos = false;
bool catched_left;
bool catched_right;
extern int ball_x;
extern int ball_y;
extern int ball_var_y;
extern int ball_var_x;
extern int timer_irq_counter;
int powerup_number = -1;
bool powerup_number_bool = false;

int draw_powerup(){
    powerup = true;
    if (!no_pos){
        powerup_x = rand() % 528 + 130;
        powerup_y = rand() % 501 + 35;
        no_pos = true;
    }
    if (!powerup_number_bool){
        powerup_number = rand() % 2;
        powerup_number_bool = true;
    }
    
    if (powerup_number == 0){
        if (draw_xpm((xpm_map_t) powerup_piece_velocity__xpm, powerup_x,powerup_y)) return 1;
    } else if (powerup_number == 1){
        if (draw_xpm((xpm_map_t) powerup_limits__xpm, powerup_x,powerup_y)) return 1;
    }

    catched_right = false;  
    catched_left = false;  

    return 0;
}

int catch_powerup(){
    if (ball_var_x>0){  
        if ((ball_x+22>=powerup_x) && (ball_x<=powerup_x+30) && (ball_y+22>=powerup_y) && (ball_y<=powerup_y+30)){
            catched_left = true;
            timer_irq_counter = 0;
            powerup=false;
        }
    }
    else if (ball_var_x<0){
        if ((ball_x+22>=powerup_x) && (ball_x<=powerup_x+30) && (ball_y+22>=powerup_y) && (ball_y<=powerup_y+30)){
            catched_right = true;
            timer_irq_counter = 0;
            powerup=false;
        }
    }


    return 0;
}
