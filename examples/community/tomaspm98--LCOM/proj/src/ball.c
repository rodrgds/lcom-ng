#include "ball.h"

#include <lcom/lcf.h>


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


int ball_x = 390;
int ball_y = 290;
int ball_var_y;
int ball_var_x;
bool startBall = true;
int side_start=-1;
bool goal = false;
int goal_left = 0;
int goal_right=0;
extern int piece_1_x;
extern int piece_1_y;
extern int piece_2_x;
extern int piece_2_y;
bool not_random = false;
extern bool catched_left;
extern bool catched_right;
extern int timer_irq_counter;
extern bool no_pos;
extern bool powerup_number_bool;
extern int powerup_number;
extern int timer_irq_counter;

int start_ball(){
    if (goal){
        goal = false;
        ball_x = 390;
        ball_y = 290;
        ball_var_x = 0;
        ball_var_y = 0;
        piece_1_x = 30;
        piece_1_y = 255;
        piece_2_x = 757;
        piece_2_y = 255;
        tickdelay(100);
        catched_right = false;
        catched_left = false;
        no_pos=false;
        powerup_number_bool = false;
        timer_irq_counter=0;
    }
    if (!not_random){
        srand(time(NULL));
        side_start = rand() % 2;
        not_random = true;
    }
         if (side_start==0){
                ball_var_x=-5;
         }
         else {
            if(side_start==1){
                ball_var_x=5;
                
         }
    }
    if ((ball_x==45 || ball_x==735) && ((ball_y+21>=piece_1_y && ball_y<=piece_1_y+89) || (ball_y+21>=piece_2_y && ball_y<=piece_2_y+89))){ 
        startBall = false;
    }

    return 0;
}

int collision_ball_piece1(){
    if (ball_x == 45){    
    int meio = piece_1_y + 46-11;
    int topo_1 = piece_1_y-11+36;
    int topo_2 = piece_1_y-11+26;
    int topo_3 = piece_1_y-11+16;
    int topo_4 = piece_1_y-11+6;
    int topo_5 = piece_1_y-15;
    int topo_6 = piece_1_y -21;
    int baixo_1 = piece_1_y-11+46;
    int baixo_2 = piece_1_y-11+56;
    int baixo_3 = piece_1_y-11+66;
    int baixo_4 = piece_1_y-11+76;
    int baixo_5 = piece_1_y-11+86;
    int baixo_6 = piece_1_y-11+96; 

    if (ball_y == meio){
        ball_var_y=0;
        ball_var_x=5;
    }
    else if (ball_y<meio && ball_y>=topo_1){
        ball_var_x=5;
        ball_var_y=-1; 
    }

    else if (ball_y<topo_1 && ball_y>=topo_2){
        ball_var_x=5;
        ball_var_y=-2; 
    }

    else if (ball_y<topo_2 && ball_y>=topo_3){
        ball_var_x=5;
        ball_var_y=-3; 
    }
    else if (ball_y<topo_3 && ball_y>=topo_4){
        ball_var_x=5;
        ball_var_y=-4;
    }
    else if (ball_y<topo_4 && ball_y>=topo_5){
        ball_var_x=5;
        ball_var_y=-5; 
    }
    else if (ball_y<topo_5 && ball_y>=topo_6){
        ball_var_x=5;
        ball_var_y=-6; 
    }
    else if (ball_y>meio && ball_y<=baixo_1){
        ball_var_x=5;
        ball_var_y=1; 
    }
    else if (ball_y>baixo_1 && ball_y<=baixo_2){
        ball_var_x=5;
        ball_var_y=2; 
    }
    else if (ball_y>baixo_2 && ball_y<=baixo_3){
        ball_var_x=5;
        ball_var_y=3; 
    }
    else if (ball_y>baixo_3 && ball_y<=baixo_4){
        ball_var_x=5;
        ball_var_y=4; 
    }
    else if (ball_y>baixo_4 && ball_y<=baixo_5){
        ball_var_x=5;
        ball_var_y=5; 
    }
    else if (ball_y>baixo_5 && ball_y<=baixo_6){
        ball_var_x=5;
        ball_var_y=6; 
    }    
   }

   return 0;
}

int collision_ball_piece2(){
    if (ball_x == 735){
    int meio = piece_2_y +46-11;
    int topo_1 = piece_2_y-11+36;
    int topo_2 = piece_2_y-11+26;
    int topo_3 = piece_2_y-11+16;
    int topo_4 = piece_2_y-11+6;
    int topo_5 = piece_2_y-15;
    int topo_6 = piece_2_y -21;
    int baixo_1 = piece_2_y-11+46;
    int baixo_2 = piece_2_y-11+56;
    int baixo_3 = piece_2_y-11+66;
    int baixo_4 = piece_2_y-11+76;
    int baixo_5 = piece_2_y-11+86;
    int baixo_6 = piece_2_y-11+96; 
    if (ball_y == meio){
        ball_var_y=0;
        ball_var_x=-5;
    }
    else if (ball_y<meio && ball_y>=topo_1){
        ball_var_x=-5;
        ball_var_y=-1; 
    }

    else if (ball_y<topo_1 && ball_y>=topo_2){
        ball_var_x=-5;
        ball_var_y=-2; 
    }

    else if (ball_y<topo_2 && ball_y>=topo_3){
        ball_var_x=-5;
        ball_var_y=-3; 
    }

    else if (ball_y<topo_3 && ball_y>=topo_4){
        ball_var_x=-5;
        ball_var_y=-4; 
    }
    else if (ball_y<topo_4 && ball_y>=topo_5){
        ball_var_x=-5;
        ball_var_y=-5; 
    }
    else if (ball_y<topo_5 && ball_y>=topo_6){
        ball_var_x=-5;
        ball_var_y=-6; 
    }
    else if (ball_y>meio && ball_y<=baixo_1){
        ball_var_x=-5;
        ball_var_y=1; 
    }
    else if (ball_y>baixo_1 && ball_y<=baixo_2){
        ball_var_x=-5;
        ball_var_y=2; 
    }
    else if (ball_y>baixo_2 && ball_y<=baixo_3){
        ball_var_x=-5;
        ball_var_y=3; 
    }
    else if (ball_y>baixo_3 && ball_y<=baixo_4){
        ball_var_x=-5;
        ball_var_y=4; 
    }
    else if (ball_y>baixo_4 && ball_y<=baixo_5){
        ball_var_x=-5;
        ball_var_y=5; 
    }
    else if (ball_y>baixo_5 && ball_y<=baixo_6){
        ball_var_x=-5;
        ball_var_y=6; 
    }    
   }

   if (((ball_y>=99 && ball_y<=105) || (ball_y >= 475 && ball_y<=481)) && powerup_number == 1 && (catched_left || catched_right) && timer_irq_counter<=1000){
    if(ball_var_x<0){
        ball_var_x = -5;
        ball_var_y = -ball_var_y;
    }
    else {
        ball_var_x = 5;
        ball_var_y = -ball_var_y;
    }
   }
   else if (((ball_y>=34 && ball_y<=40) || (ball_y >= 540 && ball_y<=546))){
    if(ball_var_x<0){
        ball_var_x = -5;
        ball_var_y = -ball_var_y;
    }
    else {
        ball_var_x = 5;
        ball_var_y = -ball_var_y;
    }
   }


   return 0;
}

int goal_leftPiece(){
    goal = true;
    startBall = true;
    not_random = true;
    goal_left++;
    if (draw_xpm((xpm_map_t) letter_g_xpm,327,284)) return 1;
    if (draw_xpm((xpm_map_t) letter_o_xpm,365,284)) return 1;
    if (draw_xpm((xpm_map_t) letter_a_xpm,403,284)) return 1;
    if (draw_xpm((xpm_map_t) letter_l_xpm,441,284)) return 1;

    return 0;
}

int goal_rightPiece(){
    goal = true;
    startBall = true;
    not_random = true;
    goal_right++;
    if (draw_xpm((xpm_map_t) letter_g_xpm,327,284)) return 1;
    if (draw_xpm((xpm_map_t) letter_o_xpm,365,284)) return 1;
    if (draw_xpm((xpm_map_t) letter_a_xpm,403,284)) return 1;
    if (draw_xpm((xpm_map_t) letter_l_xpm,441,284)) return 1;

    return 0;
}

int ball_pos_variation(){
    ball_x+=ball_var_x;
    ball_y+=ball_var_y;

    return 0; 
}
int ball_movement(){
   if (startBall){ 
    if (start_ball()) return 1;
   }

   collision_ball_piece1();

   collision_ball_piece2();

   if (ball_x == 0){
        goal_leftPiece();
    }
    else if (ball_x == 770){
        goal_rightPiece();
    }
   
   ball_pos_variation();

   return 0;
}

int draw_ball(){
    if (!goal){
        if (draw_xpm((xpm_map_t) ball_xpm,ball_x,ball_y)) return 1;
   }
   return 0;
}

int score(){
    switch(goal_right){
        case 0:
            if(draw_xpm((xpm_map_t)zero_xpm,365,40)) return 1;
            break;
        case 1:
            if(draw_xpm((xpm_map_t)one_xpm,365,40)) return 1;
            break;
        case 2:
            if(draw_xpm((xpm_map_t)two_xpm,365,40)) return 1;
            break;
        case 3:
            if(draw_xpm((xpm_map_t)three_xpm,365,40)) return 1;
            break; 
        default:
            return 1;       
    }

    switch(goal_left){
        case 0:
            if(draw_xpm((xpm_map_t)zero_xpm,405,40)) return 1;
            break;
        case 1:
            if(draw_xpm((xpm_map_t)one_xpm,405,40)) return 1;
            break;
        case 2:
            if(draw_xpm((xpm_map_t)two_xpm,405,40)) return 1;
            break;
        case 3:
            if(draw_xpm((xpm_map_t)three_xpm,405,40)) return 1;
            break; 
        default:
            return 1;       
    }

    return 0;

}
