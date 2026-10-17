#include <stdio.h>
#include <stdlib.h>

#include "elements.h"

int xOffsetFireboy = 0;
int yOffsetFireboy = 0;
int xOffsetWatergirl = 0;
int yOffsetWatergirl = 0;
int fireboyState = 0; // 0 for standing still, 1 for left, 2 for right
bool fireboyInAir = false;
int watergirlState=0;
bool watergirlInAir=false;
int UpFrameFireboy = 0; 
int UpFrameWatergirl = 0; 
int yPlatformBlockOffset = 0;
bool upwardMotionFireboy = false;
bool upwardMotionWatergirl = false;
bool lever=false;
bool fireboyOnTopOfPlatform = false;
bool watergirlOnTopOfPlatform = false;
bool platformFinished = false;
bool two_player=true;
bool pressed=false;
bool watergirlWin=false; 
bool fireboyWin=false;
int animationFrameWatergirl = 1;
int animationFrameFireboy = 1;
int *levelArray;
extern Sprite *Fireboy;
extern Sprite *Watergirl;
extern Sprite *FireboyLeft1;
extern Sprite *FireboyLeft2;
extern Sprite *FireboyRight1;
extern Sprite *FireboyRight2;
extern Sprite *WatergirlLeft1;
extern Sprite *WatergirlLeft2;
extern Sprite *WatergirlRight1;
extern Sprite *WatergirlRight2;
extern GameState gameState;
extern int timer_break;

void updateFireboy(int x, int y){

    int down_index_collision;
    down_index_collision = (((x+ xOffsetFireboy)/20) + ((y + yOffsetFireboy)/20) * 40) + 40;

    if (
        (
            (levelArray[down_index_collision + 80] == 32) ||
            (levelArray[down_index_collision + 40] == 32)  
        )){
        fireboyOnTopOfPlatform = true;
    }
    else {
        fireboyOnTopOfPlatform = false;
    }



    if (fireboyInAir){

        int up_index_collision = (((x+ xOffsetFireboy)/20) + ((y + yOffsetFireboy)/20) * 40);
        if (
            ((levelArray[up_index_collision] != 0) 
            && (levelArray[up_index_collision] != 17)
            && (levelArray[up_index_collision] != 16)
            && (levelArray[up_index_collision] != 27)
            && (levelArray[up_index_collision] != 28)
            && (levelArray[up_index_collision] != 30)
            && (levelArray[up_index_collision] != 31)
            ))
            {// if bloco de cima não é vazio
            UpFrameFireboy = 0;
            upwardMotionFireboy = false;
        }
        if  (UpFrameFireboy> 0 && upwardMotionFireboy == true){
            UpFrameFireboy--;
            yOffsetFireboy-=20;
            if (fireboyState == LEFT){
                xOffsetFireboy -= 20;
            }
            else if (fireboyState == RIGHT){
                xOffsetFireboy += 20;
            }
        }
        else 
        {
            upwardMotionFireboy = false;
        }

        if (upwardMotionFireboy == false){
            down_index_collision = (((x+ xOffsetFireboy)/20) + ((y + yOffsetFireboy)/20) * 40) + 40;
             if (
                ((levelArray[down_index_collision] == 0) || (levelArray[down_index_collision] == 17) || 
                (levelArray[down_index_collision] == 16) || (levelArray[down_index_collision] == 30) || 
                (levelArray[down_index_collision] == 31) || (levelArray[down_index_collision] == 27) 
                || (levelArray[down_index_collision] == 28)) &&
                ((levelArray[down_index_collision + 40] == 0) || (levelArray[down_index_collision + 40] == 17) || 
                (levelArray[down_index_collision + 40] == 16) || (levelArray[down_index_collision + 40] == 30) ||
                (levelArray[down_index_collision + 40] == 31) || (levelArray[down_index_collision + 40] == 27)
                || (levelArray[down_index_collision + 40] == 28)) &&
                ((levelArray[down_index_collision + 80] == 0) || (levelArray[down_index_collision + 80] == 17) || 
                (levelArray[down_index_collision + 80] == 16) || (levelArray[down_index_collision + 80] == 30) ||
                (levelArray[down_index_collision + 80] == 31) || (levelArray[down_index_collision + 80] == 27)
                || (levelArray[down_index_collision + 80] == 28))
                ){
                    yOffsetFireboy+=20;
                    if (fireboyState == LEFT){
                        xOffsetFireboy-=20;
                    }
                    else if (fireboyState == RIGHT){
                        xOffsetFireboy+=20;
                    }
                }
            else{
                fireboyInAir = false;
            }
        }
    }

    int j;
    j = ((x+ xOffsetFireboy)/20) + ((y+yOffsetFireboy)/20) * 40;

    if ((
        (levelArray[j] != 0) && 
        (levelArray[j] != 17) && 
        (levelArray[j] != 16) &&
        (levelArray[j] != 30) &&
        (levelArray[j] != 31) &&
        (levelArray[j] != 27) &&
        (levelArray[j] != 28)) ||
        ((levelArray[j+40] != 0) && 
        (levelArray[j+40] != 17) && 
        (levelArray[j+40] != 16) &&
        (levelArray[j+40] != 30) &&
        (levelArray[j+40] != 31) &&
        (levelArray[j+40] != 27) &&
        (levelArray[j+40] != 28)) ||
        ((levelArray[j+80] != 0) && 
        (levelArray[j+80] != 17) && 
        (levelArray[j+80] != 16) &&
        (levelArray[j+80] != 30) && 
        (levelArray[j+80] != 31) && 
        (levelArray[j+80] != 27) &&
        (levelArray[j+80] != 28)) ||
        ((levelArray[j+1] != 0) &&
        (levelArray[j+1] != 17) &&
        (levelArray[j+1] != 16) &&
        (levelArray[j+1] != 27) &&
        (levelArray[j+1] != 30) &&
        (levelArray[j+1] != 31) &&
        (levelArray[j+1] != 28)) ||
        ((levelArray[j+41] != 0) &&
        (levelArray[j+41] != 17) &&
        (levelArray[j+41] != 16) &&
        (levelArray[j+41] != 27) &&
        (levelArray[j+41] != 28) &&
        (levelArray[j+41] != 30) &&
        (levelArray[j+41] != 31)) ||
        ((levelArray[j+81] != 0) &&
        (levelArray[j+81] != 17) &&
        (levelArray[j+81] != 16) &&
        (levelArray[j+81] != 27) &&
        (levelArray[j+81] != 28) &&
        (levelArray[j+81] != 30) &&
        (levelArray[j+81] != 31))
        )
        {
        if (fireboyState==RIGHT){
            xOffsetFireboy -= 20;
        }
        else if (fireboyState==LEFT){
            xOffsetFireboy += 20;
        }
        upwardMotionFireboy = false; 
    }

    if((levelArray[j+120] == 21) ||
        (levelArray[j+120] == 22) || 
        (levelArray[j+120] == 23) ||
        (levelArray[j+120] == 24) ||
        (levelArray[j+120] == 25) || 
        (levelArray[j+120] == 26) ){
        if (two_player) sp_send_data(10);
        gameState=LOST;
     }
    else if(levelArray[j-40]==30){
        fireboyWin=true;
    }

    if (fireboyState == STOPPED) drawSprite(Fireboy, x + xOffsetFireboy, y + yOffsetFireboy);
    else if ((animationFrameFireboy % 2) == 0){
        if (fireboyState == LEFT){
            drawSprite(FireboyLeft1, x + xOffsetFireboy, y + yOffsetFireboy);
        }
        else{
            drawSprite(FireboyRight1, x + xOffsetFireboy, y + yOffsetFireboy);
        }
        animationFrameFireboy++; 
    }
    else{
        if (fireboyState == LEFT){
            drawSprite(FireboyLeft2, x + xOffsetFireboy, y + yOffsetFireboy);
        }
        else{
            drawSprite(FireboyRight2, x + xOffsetFireboy, y + yOffsetFireboy);
        }
        animationFrameFireboy++;
    }  
    

}



void updateWatergirl(int x, int y){

    int down_index_collision;
    down_index_collision = (((x+ xOffsetFireboy)/20) + ((y + yOffsetFireboy)/20) * 40) + 40;

    if (
        (
            (levelArray[down_index_collision + 80] == 32) ||
            (levelArray[down_index_collision + 40] == 32)  
        )){
        watergirlOnTopOfPlatform = true;
    }
    else {
        watergirlOnTopOfPlatform = false;
    }


    if (watergirlInAir){
        int up_index_collision = (((x+ xOffsetWatergirl)/20) + ((y + yOffsetWatergirl)/20) * 40);
        if (
            ((levelArray[up_index_collision] != 0) 
            && (levelArray[up_index_collision] != 17)
            && (levelArray[up_index_collision] != 16)
            && (levelArray[up_index_collision] != 27)
            && (levelArray[up_index_collision] != 28)
            && (levelArray[up_index_collision] != 30)
            && (levelArray[up_index_collision] != 31)
            ))
            {// if bloco de cima vazio
            UpFrameWatergirl = 0;
            upwardMotionWatergirl = false;
        }
        if  (UpFrameWatergirl> 0 && upwardMotionWatergirl == true){
            UpFrameWatergirl--;
            yOffsetWatergirl-=20;
            if (watergirlState == LEFT){
                xOffsetWatergirl -= 20;
            }
            else if (watergirlState == RIGHT){
                xOffsetWatergirl += 20;
            }
        }
        else 
        {
            upwardMotionWatergirl = false;
        }

        if (upwardMotionWatergirl == false){
            down_index_collision = (((x+ xOffsetWatergirl)/20) + ((y + yOffsetWatergirl)/20) * 40) + 40;
             if (
                ((levelArray[down_index_collision] == 0) || (levelArray[down_index_collision] == 17) || 
                (levelArray[down_index_collision] == 16) || (levelArray[down_index_collision] == 30) || 
                (levelArray[down_index_collision] == 31) || (levelArray[down_index_collision] == 27) 
                || (levelArray[down_index_collision] == 28)) &&
                ((levelArray[down_index_collision + 40] == 0) || (levelArray[down_index_collision + 40] == 17) || 
                (levelArray[down_index_collision + 40] == 16) || (levelArray[down_index_collision + 40] == 30) ||
                (levelArray[down_index_collision + 40] == 31) || (levelArray[down_index_collision + 40] == 27)
                || (levelArray[down_index_collision + 40] == 28)) &&
                ((levelArray[down_index_collision + 80] == 0) || (levelArray[down_index_collision + 80] == 17) || 
                (levelArray[down_index_collision + 80] == 16) || (levelArray[down_index_collision + 80] == 30) ||
                (levelArray[down_index_collision + 80] == 31) || (levelArray[down_index_collision + 80] == 27)
                || (levelArray[down_index_collision + 80] == 28))
                ){
                    yOffsetWatergirl+=20;
                    if (watergirlState == LEFT){
                        xOffsetWatergirl-=20;
                    }
                    else if (watergirlState == RIGHT){
                        xOffsetWatergirl+=20;
                    }
                }
            else {
                watergirlInAir = false;
            }
        }
    }

    int j;
    j = ((x+ xOffsetWatergirl)/20) + ((y+yOffsetWatergirl)/20) * 40;

    if ((
        (levelArray[j] != 0) && 
        (levelArray[j] != 17) && 
        (levelArray[j] != 16) &&
        (levelArray[j] != 30) &&
        (levelArray[j] != 31) &&
        (levelArray[j] != 27) &&
        (levelArray[j] != 28)) ||
        ((levelArray[j+40] != 0) && 
        (levelArray[j+40] != 17) && 
        (levelArray[j+40] != 16) &&
        (levelArray[j+40] != 30) &&
        (levelArray[j+40] != 31) &&
        (levelArray[j+40] != 27) &&
        (levelArray[j+40] != 28)) ||
        ((levelArray[j+80] != 0) && 
        (levelArray[j+80] != 17) && 
        (levelArray[j+80] != 16) &&
        (levelArray[j+80] != 30) && 
        (levelArray[j+80] != 31) && 
        (levelArray[j+80] != 27) &&
        (levelArray[j+80] != 28)) ||
        ((levelArray[j+1] != 0) &&
        (levelArray[j+1] != 17) &&
        (levelArray[j+1] != 16) &&
        (levelArray[j+1] != 27) &&
        (levelArray[j+1] != 30) &&
        (levelArray[j+1] != 31) &&
        (levelArray[j+1] != 28)) ||
        ((levelArray[j+41] != 0) &&
        (levelArray[j+41] != 17) &&
        (levelArray[j+41] != 16) &&
        (levelArray[j+41] != 27) &&
        (levelArray[j+41] != 28) &&
        (levelArray[j+41] != 30) &&
        (levelArray[j+41] != 31)) ||
        ((levelArray[j+81] != 0) &&
        (levelArray[j+81] != 17) &&
        (levelArray[j+81] != 16) &&
        (levelArray[j+81] != 27) &&
        (levelArray[j+81] != 28) &&
        (levelArray[j+81] != 30) &&
        (levelArray[j+81] != 31))
        ){
        if (watergirlState==RIGHT){
            xOffsetWatergirl -= 20;
        }
        else if (watergirlState==LEFT){
            xOffsetWatergirl += 20;
        }
        upwardMotionWatergirl = false;   
    }

    if((levelArray[j+120] == 18) ||
        (levelArray[j+120] == 19) || 
        (levelArray[j+120] == 20)  ||
        (levelArray[j+120] == 24) ||
        (levelArray[j+120] == 25) || 
        (levelArray[j+120] == 26) ){
        if (two_player) sp_send_data(10); // random number 10 just to clear the SP data
        gameState=LOST;
     }
     else if (((levelArray[j+80]==27) ||(levelArray[j+80]==28))){
        pressed=true;

     }
     else if(levelArray[j-40]==31){
        watergirlWin=true;
    }

    if (watergirlState == STOPPED) drawSprite(Watergirl, x + xOffsetWatergirl, y + yOffsetWatergirl);
    else if ((animationFrameWatergirl % 2) == 0){
        if (watergirlState == LEFT){
            drawSprite(WatergirlLeft1, x + xOffsetWatergirl, y + yOffsetWatergirl);
        }
        else{
            drawSprite(WatergirlRight1, x + xOffsetWatergirl, y + yOffsetWatergirl);
        }
        animationFrameWatergirl++; 
    }
    else{
        if (watergirlState == LEFT){
            drawSprite(WatergirlLeft2, x + xOffsetWatergirl, y + yOffsetWatergirl);
        }
        else{
            drawSprite(WatergirlRight2, x + xOffsetWatergirl, y + yOffsetWatergirl);
        }
        animationFrameWatergirl++;  
    }
}



void checkGravityFireboy(int x, int y){
    int down_index_collision = (((x+ xOffsetFireboy)/20) + ((y + yOffsetFireboy)/20) * 40) + 40;
    if (!fireboyInAir){
        if (
        ((levelArray[down_index_collision] == 0) || (levelArray[down_index_collision] == 17) ||
         (levelArray[down_index_collision] == 16) || (levelArray[down_index_collision] == 30) ||
         (levelArray[down_index_collision] == 31))
        ){
            if ((levelArray[down_index_collision + 40] != 32)) fireboyInAir = true;
        }
    }
}


void checkGravityWatergirl(int x, int y){
    int down_index_collision = (((x+ xOffsetWatergirl)/20) + ((y + yOffsetWatergirl)/20) * 40) + 40;
    if (!watergirlInAir){
        if (
        ((levelArray[down_index_collision] == 0) || (levelArray[down_index_collision] == 17) ||
         (levelArray[down_index_collision] == 16) || (levelArray[down_index_collision] == 30) ||
         (levelArray[down_index_collision] == 31))
        ){
            if ((levelArray[down_index_collision + 40] != 32)) watergirlInAir = true;
        }
    }
}

void updateArrayWithLevel(int level) {
    char filename[50];
    levelArray[0] = 5;
    sprintf(filename, "/home/lcom/labs/proj/src/levels/level%d.txt", level);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }
    int i = 0;
    while (!feof(fp) && i < (40 * 30)) {
        fscanf(fp, "%d", &levelArray[i]);
        i++;
    }
    fclose(fp);
}

void alocateLevelArrayMemory(){
    levelArray = malloc(40 * 30 * sizeof(int));
}

void freeLevelArray() {
    free(levelArray);
}


void checkPlatformLogic(int max_height){
    if (yPlatformBlockOffset < max_height){
        int i,x,y;
        for (i = 0; i < 1200; i++) {
            x = (i % 40) * 20;
            y = (i / 40) * 20;
            if (levelArray[i] == 32) {
                levelArray[i] = 0;
                levelArray[i-40] = 32; 
            }
        }
        if (fireboyOnTopOfPlatform) yOffsetFireboy -=20;
        yPlatformBlockOffset++;

    }
    else{
        fireboyOnTopOfPlatform = false;
        platformFinished = true;
    }   
}


