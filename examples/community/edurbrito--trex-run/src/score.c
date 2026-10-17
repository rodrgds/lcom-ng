#include <lcom/lcf.h>

#include "main_menu.h"
#include "single_player.h"
#include "score.h"
#include <string.h>

extern uint8_t scancodes[2];

extern Score score;
extern MainMenuStM stm;
extern Interface background;
static HiScores hiscores;

int readScores(){
    FILE *fptr  = NULL;
    fptr = fopen("/home/lcom/labs/proj/src/highscores.txt","r");
    
    if(fptr == NULL) return NOT_OK;            

    char line[256];

    hiscores.score1.score = 0;
    hiscores.score1.day = 0;
    hiscores.score1.month = 0;
    hiscores.score1.year = 0;
    hiscores.score1.sec = 0;
    hiscores.score1.min = 0;
    hiscores.score1.hour = 0;
    hiscores.score2.score = 0;
    hiscores.score2.day = 0;
    hiscores.score2.month = 0;
    hiscores.score2.year = 0;
    hiscores.score2.sec = 0;
    hiscores.score2.min = 0;
    hiscores.score2.hour = 0;
    hiscores.score3.score = 0;
    hiscores.score3.day = 0;
    hiscores.score3.month = 0;
    hiscores.score3.year = 0;
    hiscores.score3.sec = 0;
    hiscores.score3.min = 0;
    hiscores.score3.hour = 0;

    Score scores[] = {hiscores.score1,hiscores.score2,hiscores.score3};
    int scrs = 0;

    while (fgets(line, sizeof(line), fptr)) {

        char delim[] = ",";

	    char *ptr = strtok(line, delim);
        int nums = 0;
        int numsA[6] = {-1,-1,-1,-1,-1,-1};
        while(ptr != NULL)
        {
            if(*ptr != '\n' && nums < 6){
                numsA[nums] = atoi(ptr);
                nums += 1;
            }
            ptr = strtok(NULL, delim);
        } 
        bool null = false;
        for(int i = 0; i < 6; i++){
            if(numsA[i] == -1) null = true;
        }

        if(!null && scrs < 3){
            scores[scrs].score = numsA[0];
            scores[scrs].day = numsA[1];
            scores[scrs].month = numsA[2];
            scores[scrs].year = numsA[3];
            scores[scrs].hour = numsA[4];
            scores[scrs].min = numsA[5];
            scrs++;
        }
    }
                                  
    fclose(fptr);
    
    hiscores.score1 = scores[0];
    hiscores.score2 = scores[1];
    hiscores.score3 = scores[2];
    
    return OK;
}

void updateScores(){

    if(score.day == 0) return;

    if(score.score > hiscores.score3.score){
        if(score.score > hiscores.score2.score){
            if(score.score > hiscores.score1.score){
                hiscores.score3 = hiscores.score2;
                hiscores.score2 = hiscores.score1;
                hiscores.score1 = score;
            }
            else{
                hiscores.score3 = hiscores.score2;
                hiscores.score2 = score;
            }
        }
        else{
            hiscores.score3 = score;
        }
    }

}

int writeScores(){

    FILE *fptr  = NULL;
    fptr = fopen("/home/lcom/labs/proj/src/highscores.txt","w");
    
    if(fptr == NULL) return NOT_OK;

    Score scores[] = {hiscores.score1,hiscores.score2,hiscores.score3};
  
    for(int i = 0 ; i < 3 ; i++){
        fprintf(fptr,"%d,%d,%d,%d,%d,%d\n",scores[i].score,scores[i].day,scores[i].month,scores[i].year,scores[i].hour,scores[i].min);
    }

    fclose(fptr);
    return OK;
}

int stSCORES(){ 
    drawBitmap(background.scoresBtn_Hoover,H(2,6),V(8,10), ALIGN_CENTER,REAL_BUFFER);
    drawBitmap(stm.mouse,stm.xi,stm.yi, ALIGN_LEFT,REAL_BUFFER); 
    tickdelay(micros_to_ticks(150000));
    
    drawBitmap(background.scoresBtn,H(2,6),V(8,10), ALIGN_CENTER,REAL_BUFFER);
    drawBitmap(stm.mouse,stm.xi,stm.yi, ALIGN_LEFT,REAL_BUFFER); 
    tickdelay(micros_to_ticks(1000000));

    vg_draw_rectangle2(0,0,H(1,1),V(1,1),0xffffff,(void *) get_double_buffer());
    
    change_buffers();

    stm.state = SCORES;
    return OK;
}

void ScoreMouseEventHandler(struct packet p){

    // Parsing and calculating Mouse Position
    if(stm.xi + p.delta_x >= 0 && stm.xi + p.delta_x + stm.mouse->bitmapInfoHeader.width <= H(1,1)) 
        stm.xi += p.delta_x;
    if(stm.yi - p.delta_y >= 0 && stm.yi - p.delta_y + stm.mouse->bitmapInfoHeader.height <= V(1,1)) 
        stm.yi -= p.delta_y;
    
    // Detecting EXIT Event with Mouse Position
    if(stm.xi < H(4,6) + background.exitBtn->bitmapInfoHeader.width/2 && stm.xi > H(4,6) - background.exitBtn->bitmapInfoHeader.width/2 && stm.yi < V(8,10) + background.exitBtn->bitmapInfoHeader.height && stm.yi > V(8,10)){   
        if(p.lb){
            stm.event = EXBTN;
        }
    }

}

void ScoreKbdEventHandler(){
    
    // Detecting EXIT Event with Keyboard Buttons
    if(scancodes[0] == ESC_BREAK_CODE){
        stm.event = EXBTN;
    }
    
}

void drawDigits(int value,int h, int v, int n){
    Bitmap * numbers[]    = {background.n0,background.n1,background.n2,background.n3,background.n4,background.n5,background.n6,background.n7,background.n8,background.n9};
    
    int counterDgs = 0;
    while (counterDgs < n) {
        int digit = 0;
        if(value > 0){
            digit = value % 10;
            value /= 10;
        }
        drawBitmap(numbers[digit],h - 20*counterDgs,v, ALIGN_CENTER,SECOND_BUFFER);
        counterDgs++;
    }
}

void ScoresDrawInterface(){
    vg_draw_rectangle2(0,0,H(1,1),V(1,1),0xffffff,(void *) get_double_buffer());
    drawBitmap(background.title,H(1,2),V(1,10), ALIGN_CENTER,SECOND_BUFFER);

    Score scores[] = {hiscores.score1,hiscores.score2,hiscores.score3};
    for(int i = 0 ; i < 3 ; i++){
        // Drawing the scores on the Screen
        drawDigits(scores[i].score,H(1,2) - 80,V(4+i,10),8);
        drawDigits(scores[i].day,H(1,2) + 0,V(4+i,10),2);
        drawBitmap(background.bar,H(1,2) + 15,V(4+i,10), ALIGN_CENTER,SECOND_BUFFER);
        drawDigits(scores[i].month,H(1,2) + 50,V(4+i,10),2);
        drawBitmap(background.bar,H(1,2) + 65,V(4+i,10), ALIGN_CENTER,SECOND_BUFFER);
        drawDigits(scores[i].year,H(1,2) + 100,V(4+i,10),2);
        drawDigits(scores[i].hour,H(1,2) + 160,V(4+i,10),2);
        drawBitmap(background.time,H(1,2) + 175,V(4+i,10), ALIGN_CENTER,SECOND_BUFFER);
        drawDigits(scores[i].min,H(1,2) + 210,V(4+i,10),2);
    
    }

    drawBitmap(background.exitBtn,H(4,6),V(8,10), ALIGN_CENTER,SECOND_BUFFER);
    drawBitmap(stm.mouse,stm.xi,stm.yi, ALIGN_LEFT,SECOND_BUFFER); 
    
    change_buffers();

    if(stm.event == EXBTN){
        drawBitmap(background.exitBtn_Hoover,H(4,6),V(8,10), ALIGN_CENTER,REAL_BUFFER);
        drawBitmap(stm.mouse,stm.xi,stm.yi, ALIGN_LEFT,REAL_BUFFER); 
        tickdelay(micros_to_ticks(150000));
        
        drawBitmap(background.exitBtn,H(4,6),V(8,10), ALIGN_CENTER,REAL_BUFFER);
        drawBitmap(stm.mouse,stm.xi,stm.yi, ALIGN_LEFT,REAL_BUFFER); 
        tickdelay(micros_to_ticks(1000000));
        stm.state = SPLASHSCREEN;
        stm.event = NOTH;
    }
    
}
