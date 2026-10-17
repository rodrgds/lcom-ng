#pragma once
#include <lcom/lcf.h>
#include "gameSprites.h"
#include <stdint.h>
#include "../sprites/vertical_bar.xpm"
#include "../sprites/ball.xpm"
#include "../sprites/zero_point.xpm"
#include "../sprites/one_point.xpm"
#include "../sprites/two_point.xpm"
#include "../sprites/three_point.xpm"
#include "../sprites/four_point.xpm"
#include "../sprites/five_point.xpm"
#include "../sprites/six_point.xpm"
#include "../sprites/seven_point.xpm"
#include "../sprites/eight_point.xpm"
#include "../sprites/nine_point.xpm"
#include "../sprites/colon.xpm"
#include "../sprites/pause_button.xpm"
#include <stdlib.h>
#include "game.c"
#include "menu.c"
bar left_bar;
bar right_bar;
ball ball_object;
points points_object;
menu colon,pause_button;
extern unsigned int timer_interrupt_counter;
enum xpm_image_type type= XPM_INDEXED;
uint8_t first_time=0;

void init_points(){
    points_object.x=(x_resolution/2)-50-70;
    points_object.y=20;
    points_object.pixmap[0]=xpm_load(zero_point_xpm, type, &(points_object.point[0]));
    points_object.pixmap[1]=xpm_load(one_point_xpm, type, &(points_object.point[1]));
    points_object.pixmap[2]=xpm_load(two_point_xpm, type, &(points_object.point[2]));
    points_object.pixmap[3]=xpm_load(three_point_xpm, type, &(points_object.point[3]));
    points_object.pixmap[4]=xpm_load(four_point_xpm, type, &(points_object.point[4]));
    points_object.pixmap[5]=xpm_load(five_point_xpm, type, &(points_object.point[5]));
    points_object.pixmap[6]=xpm_load(six_point_xpm, type, &(points_object.point[6]));
    points_object.pixmap[7]=xpm_load(seven_point_xpm, type, &(points_object.point[7]));
    points_object.pixmap[8]=xpm_load(eight_point_xpm, type, &(points_object.point[8]));
    points_object.pixmap[9]=xpm_load(nine_point_xpm, type, &(points_object.point[9]));

}

void init_timer(){
    colon.pixmap=xpm_load(colon_xpm,type,&(colon.img));
    colon.x=(x_resolution/2)-(colon.img.width/2);
    colon.y=y_resolution-90;
}
void draw_time(unsigned int minute,unsigned int second){
    uint8_t minute1,minute2,second1,second2;
    minute2=minute%10;
    minute1=(minute-minute2)/10;
    second2=second%10;
    second1=(second-second2)/10;
    vg_draw_xpm(points_object.pixmap[minute1],points_object.point[minute1],x_resolution/2-(colon.img.width/2)-140,colon.y);
    vg_draw_xpm(points_object.pixmap[minute2],points_object.point[minute2],x_resolution/2-(colon.img.width/2)-70,colon.y);
    vg_draw_xpm(colon.pixmap,colon.img,colon.x,colon.y);
    vg_draw_xpm(points_object.pixmap[second2],points_object.point[second2],x_resolution/2+colon.img.width/2+70,colon.y);
    vg_draw_xpm(points_object.pixmap[second1],points_object.point[second1],x_resolution/2+colon.img.width/2,colon.y);
}
void draw_points(){
    vg_draw_xpm(points_object.pixmap[playerOnePoints],points_object.point[playerOnePoints],points_object.x,points_object.y);
    vg_draw_xpm(points_object.pixmap[playerTwoPoints],points_object.point[playerTwoPoints],points_object.x+100+70,points_object.y);
}
void drawStartGame(){
    reset_ball();
    reset_bars();
    reset_game_elapsed_time();
    playerOnePoints=0;
    playerTwoPoints=0;
    draw_points();
}
void drawResumeGame(){
    clearScreen();
    draw_points();
    draw_bars();
    draw_ball();
}
void draw_ball(){
    vg_draw_xpm(ball_object.pixmap,ball_object.img,ball_object.x,ball_object.y);
}
void draw_bars(){
    vg_draw_xpm(left_bar.pixmap,left_bar.img,left_bar.x,left_bar.y);
    vg_draw_xpm(right_bar.pixmap,right_bar.img,right_bar.x,right_bar.y);
}
void init_bars(){
    left_bar.x=20;
    left_bar.pixmap=xpm_load(vertical_bar, type, &(left_bar.img));
    left_bar.pixmap_clean=xpm_load(vertical_bar_clean, type, &(left_bar.img_clean));
    left_bar.y=y_resolution/2-(left_bar.img.height)/2;
    
    right_bar.pixmap=xpm_load(vertical_bar, type, &(right_bar.img));
    right_bar.pixmap_clean=xpm_load(vertical_bar_clean, type, &(right_bar.img_clean));
    right_bar.x=x_resolution-(right_bar.img.width)-20;
    right_bar.y=y_resolution/2-(right_bar.img.height)/2;
}
void reset_bars(){
    vg_draw_xpm(left_bar.pixmap_clean,left_bar.img_clean,left_bar.x,left_bar.y);
    left_bar.x=20;
    left_bar.y=y_resolution/2-(left_bar.img.height)/2;
    vg_draw_xpm(left_bar.pixmap,left_bar.img,left_bar.x,left_bar.y);

    vg_draw_xpm(right_bar.pixmap_clean,right_bar.img_clean,right_bar.x,right_bar.y);
    right_bar.x=x_resolution-(right_bar.img.width)-20;
    right_bar.y=y_resolution/2-(right_bar.img.height)/2;
    vg_draw_xpm(right_bar.pixmap,right_bar.img,right_bar.x,right_bar.y);

}
void free_all(){
    free(left_bar.pixmap);
    free(right_bar.pixmap);
    free(ball_object.pixmap);
    free(ball_object.pixmap_clean);
    for (uint8_t i=0;i<10;i++){
        free(points_object.pixmap[i]);
    }
    free(mouse_pointer.pixmap);
    free(mouse_pointer_clean.pixmap);
    free(mouse_hover.pixmap);
    free(blank_screen.pixmap);
    free(score_button.pixmap);
    free(exit_button.pixmap);
    free(pause_screen.pixmap);
    free(pause_title.pixmap);
    free(pause_button.pixmap);
    free(continue_button.pixmap);
    free(colon.pixmap);
    free(title.pixmap);
    free(play_button.pixmap);
    free(exit_main_button.pixmap);
    free(back_slash.pixmap);
    free(high_score.pixmap);
}
void go_up_rbar(){
    vg_draw_xpm(right_bar.pixmap_clean,right_bar.img_clean,right_bar.x,right_bar.y);
    if(right_bar.y<9)
        right_bar.y=0;
    else{

        right_bar.y-=9;

    }
    vg_draw_xpm(right_bar.pixmap,right_bar.img,right_bar.x,right_bar.y);
}
void go_up_lbar(){
    vg_draw_xpm(left_bar.pixmap_clean,left_bar.img_clean,left_bar.x,left_bar.y);
    if(left_bar.y<9)
        left_bar.y=0;
    else{

        left_bar.y-=9;

    }
    vg_draw_xpm(left_bar.pixmap,left_bar.img,left_bar.x,left_bar.y);
}
void go_down_rbar(){
    vg_draw_xpm(right_bar.pixmap_clean,right_bar.img_clean,right_bar.x,right_bar.y);

    if(right_bar.y+9<=(y_resolution-right_bar.img.height))
        right_bar.y+=9;
    else{
        right_bar.y=y_resolution-right_bar.img.height;
    }
    
    vg_draw_xpm(right_bar.pixmap,right_bar.img,right_bar.x,right_bar.y);
}
void go_down_lbar(){
    vg_draw_xpm(left_bar.pixmap_clean,left_bar.img_clean,left_bar.x,left_bar.y);

    if(left_bar.y+9<=(y_resolution-left_bar.img.height))
        left_bar.y+=9;
    else{
        left_bar.y=y_resolution-left_bar.img.height;
    }
    
    vg_draw_xpm(left_bar.pixmap,left_bar.img,left_bar.x,left_bar.y);
}
void init_ball(){
    ball_object.pixmap=xpm_load(ball_xpm, type, &(ball_object.img));
    ball_object.pixmap_clean=xpm_load(ball_clean_xpm, type, &(ball_object.img_clean));
    ball_object.x=x_resolution/2-(ball_object.img.width)/2;
    ball_object.y=y_resolution/2-(ball_object.img.height)/2;
    ball_object.vy=2;
    ball_object.vx=4;
    uint8_t aux=(rand()%4);
    if(aux==0)
        ball_object.vy*=-1;
    else if(aux==1)
        ball_object.vx*=-1;
    else if(aux==2){
        ball_object.vy=ball_object.vy*(-1);
        ball_object.vx=ball_object.vx*(-1);
    }
}
void reset_ball(){
    first_time=0;
    vg_draw_xpm(ball_object.pixmap_clean,ball_object.img_clean,ball_object.x,ball_object.y);
    ball_object.x=x_resolution/2-(ball_object.img.width)/2;
    ball_object.y=y_resolution/2-(ball_object.img.height)/2;
    ball_object.vy=2;
    ball_object.vx=4;
    uint8_t aux=(rand()%4);
    if(aux==0)
        ball_object.vy*=-1;
    else if(aux==1)
        ball_object.vx*=-1;
    else if(aux==2){
        ball_object.vy=ball_object.vy*(-1);
        ball_object.vx=ball_object.vx*(-1);
    }
    vg_draw_xpm(ball_object.pixmap,ball_object.img,ball_object.x,ball_object.y);
}
int move_ball(){
    
    vg_draw_xpm(ball_object.pixmap_clean,ball_object.img_clean,ball_object.x,ball_object.y);
    int aux=detect_colision();
    if(aux==1){
        if(first_time==0){
            first_time++;
            ball_object.vx=8;
            ball_object.vy=4;
        }
        else if(first_time==5){
            first_time++;
            ball_object.vx=12;
            ball_object.vy=6;
        }
        else if(first_time<10){
            first_time++;
        }
        else if (first_time==10){
            first_time++;
            ball_object.vx=18;
            ball_object.vy=6;
        }
        ball_object.x=left_bar.x+left_bar.img.width;
        ball_object.vx=ball_object.vx*(-1);
        vg_draw_xpm(ball_object.pixmap,ball_object.img,ball_object.x,ball_object.y);
        return 0;
    }
    else if(aux==2){
        if(first_time==0){
            first_time++;
            ball_object.vx=8;
            ball_object.vy=4;
        }
        else if(first_time==5){
            first_time++;
            ball_object.vx=12;
            ball_object.vy=6;
        }
        else if(first_time<10){
            first_time++;
        }
        else if (first_time==10){
            first_time++;
            ball_object.vx=18;
            ball_object.vy=6;
        }
        ball_object.x=(right_bar.x)-(ball_object.img.width);
        ball_object.vx=ball_object.vx*(-1);
        vg_draw_xpm(ball_object.pixmap,ball_object.img,ball_object.x,ball_object.y);
        return 0;
    }
    else if(aux==3){
        ball_object.y=0;
        ball_object.vy=ball_object.vy*(-1);
        vg_draw_xpm(ball_object.pixmap,ball_object.img,ball_object.x,ball_object.y);
        return 0;
    }
    else if(aux==4){
        ball_object.y=y_resolution-ball_object.img.height;
        ball_object.vy=ball_object.vy*(-1);
        vg_draw_xpm(ball_object.pixmap,ball_object.img,ball_object.x,ball_object.y);
        return 0;
    }
    else if(aux==5){
        return 1;
    }
    else if (aux==6){
        return 2;
    }
    else{
        vg_draw_xpm(ball_object.pixmap,ball_object.img,ball_object.x,ball_object.y);
        return 0;
    }
    
}
int detect_colision(){
    if(ball_object.y<=(points_object.y+70)&&(ball_object.y+ball_object.img.height)>=(points_object.y)&&(ball_object.x+ball_object.img.width)>=points_object.x&&ball_object.x<=(points_object.x+140+100)){
        draw_points();
        
        vg_draw_xpm(ball_object.pixmap,ball_object.img,ball_object.x,ball_object.y);
        vg_draw_xpm(ball_object.pixmap_clean,ball_object.img_clean,ball_object.x,ball_object.y);
    }
    if(ball_object.vy<0&&ball_object.y<abs(ball_object.vy)){
        return 3;
    }
    else if(ball_object.vy>0&&abs(ball_object.y+ball_object.img.height+ball_object.vy)>abs(y_resolution)){
        return 4;
    }
    if(ball_object.x<=abs(ball_object.vx)){
        return 6;
    }
    ball_object.x=ball_object.x+ball_object.vx;
    ball_object.y=ball_object.y+ball_object.vy;
    if((ball_object.y+ball_object.img.height)>left_bar.y&&ball_object.y<(left_bar.y+left_bar.img.height)&&(ball_object.x<=left_bar.x+left_bar.img.width)){
        return 1;
    }
    else if(((ball_object.y+ball_object.img.height)>right_bar.y&&ball_object.y<(right_bar.y+right_bar.img.height)&&(ball_object.x+ball_object.img.width)>=(x_resolution-(right_bar.img.width)-20))){
        return 2;
    }
    else if(ball_object.x>=x_resolution){
        return 5;
    }

    
    return 0;
}


