#include "game.h"

extern int gameMode;
extern uint8_t *frame_buffer;
extern uint8_t *auxiliar_buffer;
extern vbe_mode_info_t mode_info;
extern uint16_t width;
extern uint16_t height;
extern unsigned bytesPerPixel;

extern int x;
extern int y;

extern Sprite *sprites[MAX_SPRITES];

int xBall;
int yBall;
int direction;
int xHole = 680;
int yHole = 45;

int counter;
int tempx;
int tempy;


void (drawMouse)(){
    if(gameMode == GAME_MODE)
        drawSprite(sprites[7], x, y);
}

int drawGame(){
    direction = 0;
    int i = 0;
    xBall = 50;
    yBall = 550;
    while(i < mode_info.YResolution){
        if(i <= 10 || i >= (mode_info.YResolution - 11)){
            for(int j = 0; j < mode_info.XResolution; j++){
                drawSprite(sprites[2], j, i); // top/bottom wall
            }
        } else{
            for(int j = 0; j < 11; j++){ // left wall
                drawSprite(sprites[2], j, i);
            }

            int l = 11;
            while(l < mode_info.XResolution - 11){
                if((i <= 100 && (l >= 300 && l <= 380))){
                    drawSprite(sprites[4], l, i);
                } else if((i > 100 && i <= 140) && ((l >= 100 && l <= 225) || (l >= 300 && l <= 380) || (l >= 550))){
                    drawSprite(sprites[4], l, i);
                } else if((i > 140 && i <= 240) && ((l >= 100 && l <= 225) || (l >= 300 && l <= 380) || (l >= 670 && l <= 730))){
                    drawSprite(sprites[4], l, i);
                } else if((i > 240 && i <= 280) && ((l >= 100 && l <= 225) || (l >= 300 && l <= 380))){
                    drawSprite(sprites[4], l, i);
                } else if((i > 280 && i <= 340) && (l >= 100 && l <= 225)){
                    drawSprite(sprites[4], l, i);
                } else if((i > 350 && i <= 420) && ((l >= 320 && l <= 590) || (l >= 630 && l <= 740))){
                    drawSprite(sprites[4], l, i);
                } else if((i > 460 && i <= 520) && ((l <= 225) || (l >= 400 && l <= 610))){
                    drawSprite(sprites[4], l, i);
                } 
                else{
                    drawSprite(sprites[3], l, i);
                }
                l++;
            }
            
            for(int j = 0; j < 11; j++){ // right wall
                drawSprite(sprites[2], mode_info.XResolution - j - 1, i);
            }
        }
        i++;
    }
    drawSprite(sprites[5], 680, 45);
    memcpy(auxiliar_buffer,frame_buffer,width * height * bytesPerPixel);
    return 0;
}

void process_keyboard_game(uint8_t scancode){
    switch (scancode) {
        case MAKE_ESC:
            gameMode = MENU_MODE;
            break;
        default:
            break;

    }
}

void checkIfBallInHole(){
    if((xBall-xHole <=18 && xBall-xHole >=-18) && (yBall-yHole <=18 && yBall - yHole >=-18)){
        gameMode = MENU_MODE;
        memset(auxiliar_buffer, 0 ,width * height * bytesPerPixel);
    }
}

bool checkColisiony(int xbola, int ybola){
    if((ybola <= 10 || ybola + 10 >= (mode_info.YResolution - 11))) return true;
    else if((ybola == 462 || ybola == 522) && ((xbola >= 10 && xbola <= 225) || (xbola >= 400 && xbola <= 610))) return true;
    else if((ybola == 422 || ybola == 350) && ((xbola >= 320 && xbola <= 590) || (xbola >= 630 && xbola <= 740))) return true;
    else if(ybola == 342 && (xbola >= 100 && xbola <= 225)) return true;
    else if(ybola == 282 && (xbola >= 300 && xbola <= 380)) return true;
    else if(ybola == 242 && (xbola >= 670 && xbola <= 730)) return true;
    else if(ybola == 142 && (xbola >= 550 && xbola <= 790)) return true;
    else if(ybola == 102 && ((xbola >= 550 && xbola <= 790) || (xbola >= 100 && xbola <= 225))) return true;
    else return false;
}

bool checkColisionx(int xbola, int ybola){
    if((xbola <= 10 || xbola + 32 >= (mode_info.XResolution - 11))) return true;
    else if((xbola == 102 || xbola == 226 || xbola == 402 || xbola == 610) && (ybola >= 460 && ybola <= 520)) return true;
    else if((xbola == 302 || xbola == 590 || xbola == 630 || xbola == 740) && (ybola >= 350 && ybola <= 420))return true;
    else if((xbola == 102 || xbola == 226) && (ybola >= 100 && ybola <= 340))return true;
    else if((xbola == 302 || xbola == 382) && (ybola >= 10 && ybola <= 280))return true;
    else if(xbola == 550 && (ybola >= 100 && ybola <= 140)) return true;
    else if((xbola == 670 || xbola == 730) && (ybola >= 100 && ybola <= 140))return true;
    else return false;
}

void process_tacada_MOUSE (){
    direction++;
    if(direction==4) direction=0;
}
