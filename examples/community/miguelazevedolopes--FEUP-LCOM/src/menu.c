#pragma once
#include "menu.h"
#include "../sprites/blank_screen.xpm"
#include "../sprites/menu_title.xpm"
#include "../sprites/play_button.xpm"
#include "../sprites/score_button.xpm"
#include "../sprites/exit_button_main.xpm"
#include "../sprites/pointer.xpm"
#include "../sprites/hand.xpm"
#include "../sprites/pause_screen.xpm"
#include "../sprites/pause_title.xpm"
#include "../sprites/exit_button.xpm"
#include "../sprites/continue_button.xpm"
#include "../sprites/high_score.xpm"
#include "../sprites/back_slash.xpm"
#include "../sprites/vs.xpm"
#include "../sprites/player_one_wins.xpm"
#include "../sprites/player_two_wins.xpm"
#include "game.c"
#include "mouse.c"
#include "stdbool.h"
#include "gameSprites.c"
extern uint8_t bytes[3];
extern enum xpm_image_type type;
extern int maxDay,maxMonth,maxYear;
extern unsigned int maxScoreP1,maxScoreP2,minMinutes,minSeconds;
bool clickFlag=false,hover_flag=false;
menu title,play_button,exit_main_button,score_button,blank_screen,mouse_pointer,mouse_pointer_clean,mouse_hover;
menu pause_screen,pause_title,exit_button,continue_button;
menu high_score,back_slash,seconds_object,vs;
menu playerOneWins,playerTwoWins;
extern menu colon;
extern points points_object;
extern uint8_t mouse_bitmask;
void initMenu(){
    
    title.pixmap=xpm_load(menu_title_xpm, type, &(title.img));
    title.x=x_resolution/2-title.img.width/2;
    title.y=50;
    
    play_button.pixmap=xpm_load(play_button_xpm, type, &(play_button.img));
    play_button.x=x_resolution/2-play_button.img.width/2;
    play_button.y=title.y+title.img.height+40;

    score_button.pixmap=xpm_load(score_button_xpm, type, &(score_button.img));
    score_button.x=x_resolution/2-score_button.img.width/2;
    score_button.y=play_button.y+play_button.img.height+40;

    exit_main_button.pixmap=xpm_load(exit_button_main_xpm, type, &(exit_main_button.img));
    exit_main_button.x=x_resolution/2-exit_main_button.img.width/2;
    exit_main_button.y=score_button.y+score_button.img.height+40;

    blank_screen.pixmap=xpm_load(blank_screen_xpm, type, &(blank_screen.img));
    blank_screen.x=0;
    blank_screen.y=0;

    mouse_pointer.pixmap=xpm_load(pointer_xpm,type,&(mouse_pointer.img));
    mouse_pointer.x=0;
    mouse_pointer.y=0;

    mouse_pointer_clean.pixmap=xpm_load(pointer_xpm_clean,type,&(mouse_pointer_clean.img));
    mouse_pointer_clean.x=mouse_pointer.x;
    mouse_pointer_clean.y=mouse_pointer.y;

    mouse_hover.pixmap=xpm_load(hand_xpm,type,&(mouse_hover.img));
    mouse_hover.x=mouse_pointer.y;
    mouse_hover.y=mouse_pointer.x;
    return;
}
void initPauseMenu(){
    pause_screen.pixmap=xpm_load(pause_screen_xpm,type,&(pause_screen.img));
    pause_screen.x=x_resolution/2-pause_screen.img.width/2;
    pause_screen.y=y_resolution/2-pause_screen.img.height/2;

    pause_title.pixmap=xpm_load(pause_title_xpm,type,&(pause_title.img));
    pause_title.x=x_resolution/2-pause_title.img.width/2;
    pause_title.y=pause_screen.y+10;

    exit_button.pixmap=xpm_load(exit_button_xpm,type,&(exit_button.img));
    exit_button.x=x_resolution/2-(exit_button.img.width)-50;
    exit_button.y=pause_title.y+150;

    continue_button.pixmap=xpm_load(continue_button_xpm,type,&(continue_button.img));
    continue_button.x=x_resolution/2+50;
    continue_button.y=pause_title.y+150;

}
void initGameOver(){
    playerOneWins.pixmap=xpm_load(player_one_wins_xpm,type,&(playerOneWins.img));
    playerOneWins.x=x_resolution/2-playerOneWins.img.width/2;
    playerOneWins.y=y_resolution/2-playerOneWins.img.height/2;
    playerTwoWins.pixmap=xpm_load(player_two_wins_xpm,type,&(playerTwoWins.img));
    playerTwoWins.x=x_resolution/2-playerTwoWins.img.width/2;
    playerTwoWins.y=y_resolution/2-playerTwoWins.img.height/2;
}
void drawGameOver(int winner){
    if(winner==1)
        vg_draw_xpm(playerOneWins.pixmap,playerOneWins.img,playerOneWins.x,playerOneWins.y);
    else if(winner==2)
        vg_draw_xpm(playerTwoWins.pixmap,playerTwoWins.img,playerTwoWins.x,playerTwoWins.y);

}
void drawDate(){
    vg_draw_xpm(points_object.pixmap[maxDay/10],points_object.point[maxDay/10],high_score.x-50,high_score.y+150);
    vg_draw_xpm(points_object.pixmap[maxDay%10],points_object.point[maxDay%10],high_score.x+20,high_score.y+150);
    vg_draw_xpm(back_slash.pixmap,back_slash.img,high_score.x+90,high_score.y+150);
    vg_draw_xpm(points_object.pixmap[maxMonth/10],points_object.point[maxMonth/10],high_score.x+160,high_score.y+150);
    vg_draw_xpm(points_object.pixmap[maxMonth%10],points_object.point[maxMonth%10],high_score.x+230,high_score.y+150);
    vg_draw_xpm(back_slash.pixmap,back_slash.img,high_score.x+300,high_score.y+150);
    int tempYear=maxYear;
    vg_draw_xpm(points_object.pixmap[2],points_object.point[2],high_score.x+370,high_score.y+150);
    tempYear=maxYear%1000;
    vg_draw_xpm(points_object.pixmap[tempYear/100],points_object.point[tempYear/100],high_score.x+440,high_score.y+150);
    tempYear=tempYear%100;
    vg_draw_xpm(points_object.pixmap[tempYear/10],points_object.point[tempYear/10],high_score.x+510,high_score.y+150);
    vg_draw_xpm(points_object.pixmap[tempYear%10],points_object.point[tempYear%10],high_score.x+580,high_score.y+150);
}
void initScoreMenu(){
    high_score.pixmap=xpm_load(high_score_xpm,type,&(high_score.img));
    high_score.x=x_resolution/2-(high_score.img.width)/2;
    high_score.y=high_score.y+100;
    back_slash.pixmap=xpm_load(back_slash_xpm,type,&(back_slash.img));
    vs.pixmap=xpm_load(vs_xpm,type,&(vs.img));
    vs.x=x_resolution/2-vs.img.width/2;
    vs.y=high_score.y+250;
}

void drawMaxScoreMenu(){
    vg_draw_xpm(high_score.pixmap,high_score.img,high_score.x,high_score.y);
    drawDate();
    vg_draw_xpm(vs.pixmap,vs.img,vs.x,vs.y);
    vg_draw_xpm(points_object.pixmap[maxScoreP1],points_object.point[maxScoreP1],vs.x-70,vs.y);
    vg_draw_xpm(points_object.pixmap[maxScoreP2],points_object.point[maxScoreP2],vs.x+120,vs.y);
    vg_draw_xpm(exit_main_button.pixmap,exit_main_button.img,exit_main_button.x,exit_main_button.y);
    vg_draw_xpm(colon.pixmap,colon.img,colon.x,vs.y+80);
    vg_draw_xpm(points_object.pixmap[minMinutes/10],points_object.point[minMinutes/10],colon.x-140,vs.y+80);
    vg_draw_xpm(points_object.pixmap[minMinutes%10],points_object.point[minMinutes%10],colon.x-70,vs.y+80);
    vg_draw_xpm(points_object.pixmap[minSeconds/10],points_object.point[minSeconds/10],colon.x+29,vs.y+80);
    vg_draw_xpm(points_object.pixmap[minSeconds%10],points_object.point[minSeconds%10],colon.x+99,vs.y+80);
}

void drawMenu(){
    vg_draw_xpm(title.pixmap,title.img,title.x,title.y);
    vg_draw_xpm(play_button.pixmap,play_button.img,play_button.x,play_button.y);
    vg_draw_xpm(score_button.pixmap,score_button.img,score_button.x,score_button.y);
    vg_draw_xpm(exit_main_button.pixmap,exit_main_button.img,exit_main_button.x,exit_main_button.y);
    
}

void loadMenu(){
    drawMenu();
    vg_draw_xpm(mouse_pointer.pixmap,mouse_pointer.img,mouse_pointer.x,mouse_pointer.y);
}
void loadScoreMenu(){
    drawMaxScoreMenu();
    vg_draw_xpm(mouse_pointer.pixmap,mouse_pointer.img,mouse_pointer.x,mouse_pointer.y);
}
int goBackToMainMenu(struct packet *pk){
    if((mouse_pointer.x+mouse_pointer.img.width)>=exit_main_button.x&&mouse_pointer.x<=(exit_main_button.x+exit_main_button.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=exit_main_button.y&&mouse_pointer.y<=(exit_main_button.y+exit_main_button.img.height)){
        drawMaxScoreMenu();
        vg_draw_xpm(mouse_hover.pixmap,mouse_hover.img,mouse_pointer.x,mouse_pointer.y);
    }
    if((mouse_pointer.x+mouse_pointer.img.width)>=(high_score.x-50)&&mouse_pointer.x<=(high_score.x+650)&&(mouse_pointer.y+mouse_pointer.img.height)>=high_score.y&&mouse_pointer.y<=(vs.y+150)){
        drawMaxScoreMenu();
    }
    if(pk->lb&&(mouse_pointer.x+mouse_pointer.img.width)>=exit_main_button.x&&mouse_pointer.x<=(exit_main_button.x+exit_main_button.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=exit_main_button.y&&mouse_pointer.y<=(exit_main_button.y+exit_main_button.img.height)){
        drawMaxScoreMenu();
        vg_draw_xpm(mouse_hover.pixmap,mouse_hover.img,mouse_pointer.x,mouse_pointer.y);
        return 1;
    }
    return 0;
}
int detectMenuOptionClick(struct packet *pk){
    if((mouse_pointer.x+mouse_pointer.img.width)>=play_button.x&&mouse_pointer.x<=(play_button.x+play_button.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=play_button.y&&mouse_pointer.y<=(play_button.y+play_button.img.height)){
        drawMenu();
        vg_draw_xpm(mouse_hover.pixmap,mouse_hover.img,mouse_pointer.x,mouse_pointer.y);
    }
    if(pk->lb&&(mouse_pointer.x+mouse_pointer.img.width)>=play_button.x&&mouse_pointer.x<=(play_button.x+play_button.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=play_button.y&&mouse_pointer.y<=(play_button.y+play_button.img.height)){
        drawMenu();
        vg_draw_xpm(mouse_hover.pixmap,mouse_hover.img,mouse_pointer.x,mouse_pointer.y);
        clickFlag=true;
        return 1;
    }
    if((mouse_pointer.x+mouse_pointer.img.width)>=score_button.x&&mouse_pointer.x<=(score_button.x+score_button.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=score_button.y&&mouse_pointer.y<=(score_button.y+score_button.img.height)){
        drawMenu();
        vg_draw_xpm(mouse_hover.pixmap,mouse_hover.img,mouse_pointer.x,mouse_pointer.y);
    }
    if(pk->lb&&(mouse_pointer.x+mouse_pointer.img.width)>=score_button.x&&mouse_pointer.x<=(score_button.x+score_button.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=score_button.y&&mouse_pointer.y<=(score_button.y+score_button.img.height)){
        drawMenu();
        vg_draw_xpm(mouse_hover.pixmap,mouse_hover.img,mouse_pointer.x,mouse_pointer.y);
        clickFlag=true;
        return 2;
    }
    if((mouse_pointer.x+mouse_pointer.img.width)>=exit_main_button.x&&mouse_pointer.x<=(exit_main_button.x+exit_main_button.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=exit_main_button.y&&mouse_pointer.y<=(exit_main_button.y+exit_main_button.img.height)){
        drawMenu();
        vg_draw_xpm(mouse_hover.pixmap,mouse_hover.img,mouse_pointer.x,mouse_pointer.y);
    }
    if(pk->lb&&(mouse_pointer.x+mouse_pointer.img.width)>=exit_main_button.x&&mouse_pointer.x<=(exit_main_button.x+exit_main_button.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=exit_main_button.y&&mouse_pointer.y<=(exit_main_button.y+exit_main_button.img.height)){
        drawMenu();
        vg_draw_xpm(mouse_hover.pixmap,mouse_hover.img,mouse_pointer.x,mouse_pointer.y);
        clickFlag=true;
        return 3;
    }
    if((mouse_pointer.x+mouse_pointer.img.width)>=title.x&&mouse_pointer.x<=(title.x+title.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=title.y&&mouse_pointer.y<=(title.y+title.img.height)){
        drawMenu();
        vg_draw_xpm(mouse_pointer.pixmap,mouse_pointer.img,mouse_pointer.x,mouse_pointer.y);
    }
    return -1;
}

void move_pointer(struct packet *pk){
    vg_draw_xpm(mouse_pointer_clean.pixmap,mouse_pointer_clean.img,mouse_pointer.x,mouse_pointer.y);
    mouse_pointer.x+=pk->delta_x;
    mouse_pointer.y-=pk->delta_y;
    vg_draw_xpm(mouse_pointer.pixmap,mouse_pointer.img,mouse_pointer.x,mouse_pointer.y);
    return;
}

void clearScreen(){
    vg_draw_xpm(blank_screen.pixmap,blank_screen.img,blank_screen.x,blank_screen.y);
}

void drawPauseMenu(){
    vg_draw_xpm(pause_screen.pixmap,pause_screen.img,pause_screen.x,pause_screen.y);
    vg_draw_xpm(pause_title.pixmap,pause_title.img,pause_title.x,pause_title.y);
    vg_draw_xpm(exit_button.pixmap,exit_button.img,exit_button.x,exit_button.y);
    vg_draw_xpm(continue_button.pixmap,continue_button.img,continue_button.x,continue_button.y);
}
void loadPauseMenu(){
    drawPauseMenu();
    vg_draw_xpm(mouse_pointer.pixmap,mouse_pointer.img,mouse_pointer.x,mouse_pointer.y);
}

int detectPauseMenuOptionClick(struct packet *pk){
    
    if((mouse_pointer.x+mouse_pointer.img.width)>=pause_screen.x&&mouse_pointer.x<=(pause_screen.x+pause_screen.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=pause_screen.y&&mouse_pointer.y<=(pause_screen.y+pause_screen.img.height)){
        if(pk->lb&&(mouse_pointer.x)>=continue_button.x&&mouse_pointer.x<=(continue_button.x+continue_button.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=continue_button.y&&mouse_pointer.y<=(continue_button.y+continue_button.img.height)){
            vg_draw_xpm(continue_button.pixmap,continue_button.img,continue_button.x,continue_button.y);
            vg_draw_xpm(mouse_hover.pixmap,mouse_hover.img,mouse_pointer.x,mouse_pointer.y);
            clickFlag=true;
            return 2;
        }   
        else if((mouse_pointer.x)>=continue_button.x&&mouse_pointer.x<=(continue_button.x+continue_button.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=continue_button.y&&mouse_pointer.y<=(continue_button.y+continue_button.img.height)){
            vg_draw_xpm(continue_button.pixmap,continue_button.img,continue_button.x,continue_button.y);
            vg_draw_xpm(mouse_hover.pixmap,mouse_hover.img,mouse_pointer.x,mouse_pointer.y);
        }
        else if(pk->lb&&(mouse_pointer.x)>=exit_button.x&&mouse_pointer.x<=(exit_button.x+exit_button.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=exit_button.y&&mouse_pointer.y<=(exit_button.y+exit_button.img.height)){
            vg_draw_xpm(exit_button.pixmap,exit_button.img,exit_button.x,exit_button.y);
            vg_draw_xpm(mouse_hover.pixmap,mouse_hover.img,mouse_pointer.x,mouse_pointer.y);
            clickFlag=true;
            return 1;
        }
        else if((mouse_pointer.x)>=exit_button.x&&mouse_pointer.x<=(exit_button.x+exit_button.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=exit_button.y&&mouse_pointer.y<=(exit_button.y+exit_button.img.height)){
            vg_draw_xpm(exit_button.pixmap,exit_button.img,exit_button.x,exit_button.y);
            vg_draw_xpm(mouse_hover.pixmap,mouse_hover.img,mouse_pointer.x,mouse_pointer.y);
        }
        else if((mouse_pointer.x+mouse_pointer.img.width)>=pause_title.x&&mouse_pointer.x<=(pause_title.x+pause_title.img.width)&&(mouse_pointer.y+mouse_pointer.img.height)>=pause_title.y&&mouse_pointer.y<=(pause_title.y+pause_title.img.height)){
            drawPauseMenu();
            vg_draw_xpm(mouse_pointer.pixmap,mouse_pointer.img,mouse_pointer.x,mouse_pointer.y);
        }
        else{
            drawPauseMenu();
            vg_draw_xpm(mouse_pointer.pixmap,mouse_pointer.img,mouse_pointer.x,mouse_pointer.y);
        }
        
    }

    return 0;
}



