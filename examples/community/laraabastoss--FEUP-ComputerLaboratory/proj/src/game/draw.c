#include "draw.h"
#include <stdio.h>

extern Sprite *mouse;
extern Sprite *hand;
extern Sprite *_1computer_button;
extern Sprite *_2computers_button;
extern Sprite *exit_button;
extern Sprite *fireboywatergirl;
extern Sprite *levels_button;
extern Sprite *background;
extern Sprite *levelBackground;
extern Sprite *NoBorder;
extern Sprite *Top;
extern Sprite *Left;
extern Sprite *Right;
extern Sprite *Bottom;
extern Sprite *LeftRight;
extern Sprite *TopBottom;
extern Sprite *RightBottom;
extern Sprite *LeftBottom;
extern Sprite *TopLeft;
extern Sprite *TopRight;
extern Sprite *LeftBottomRight;
extern Sprite *TopLeftBottom;
extern Sprite *LeftTopRight;
extern Sprite *TopRightBottom;
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
extern Sprite *FireHoleCenter;
extern Sprite *FireHoleRight;
extern Sprite *FireHoleLeft;
extern Sprite *WaterHoleCenter;
extern Sprite *WaterHoleRight;
extern Sprite *WaterHoleLeft;
extern Sprite *ToxicHoleCenter;
extern Sprite *ToxicHoleRight;
extern Sprite *ToxicHoleLeft;
extern Sprite *ButtonLeft;
extern Sprite *ButtonRight;
extern Sprite *ButtonPressedLeft;
extern Sprite *ButtonPressedRight;
extern Sprite *leftlever;
extern Sprite *rightlever;
extern Sprite *dooropen;
extern Sprite *doorred;
extern Sprite *doorblue;
extern Sprite *AllBorders; 
extern Sprite *PlaceTime;
extern Sprite *youLost;
extern Sprite *youWon;
extern Sprite *retry;
extern Sprite *retryLevel;
extern Sprite *barNumber;
extern Sprite *pontosNumber;
extern Sprite *n1;
extern Sprite *n2;
extern Sprite *n3;
extern Sprite *n4;
extern Sprite *n5;
extern Sprite *n6;
extern Sprite *n7;
extern Sprite *n8;
extern Sprite *n9;
extern Sprite *n0;
extern Sprite* num_sprites[10];
extern Sprite* waitingConnection;
extern Sprite* nextLevel;

extern int timer_interrupts;
extern RTC_structure time_info;
extern MouseInfo mouse_packet;
extern vbe_mode_info_t vbe_info;
extern GameState gameState;
extern int *levelArray;
extern bool lever;
extern bool pressed;
extern bool fireboyWin;
extern bool watergirlWin;
extern int level;


uint8_t *mainBuffer;
uint8_t *secondaryBuffer;
uint8_t *framebuffer;
uint32_t framebuffer_size;


int set_frame_buffers(uint16_t mode) {
    if (set_frame_buffer(mode, &mainBuffer)) return 1;
    int bytes_per_pixel=((vbe_info.BitsPerPixel + 7) / 8);
    framebuffer_size = vbe_info.XResolution*vbe_info.YResolution*bytes_per_pixel;
    secondaryBuffer = (uint8_t *) malloc(framebuffer_size);
    framebuffer = secondaryBuffer;
    return 0;
}

void wait_menu(){
    drawSprite(background, 0, 0);
    drawSprite(waitingConnection,0,1*vbe_info.YResolution/6);
}

void initial_menu() {
    drawSprite(background, 0, 0);
    drawSprite(fireboywatergirl, 0, 1*vbe_info.YResolution/6);
    drawSprite(_1computer_button, 0,2*vbe_info.YResolution/6);
    drawSprite(_2computers_button, 0, 3*vbe_info.YResolution/6);
    drawSprite(exit_button, 0, 4*vbe_info.YResolution/6);
}


void won_menu() {
    drawSprite(background, 0, 0);
    drawSprite(youWon, 0, 1*vbe_info.YResolution/6);
    if(level==3) drawSprite(retry, 0, 3*vbe_info.YResolution/6);
    else drawSprite(nextLevel,0,3*vbe_info.YResolution/6);
    drawSprite(exit_button,0,4*vbe_info.YResolution/6);
}

void lost_menu() {
    drawSprite(background, 0, 0);
    drawSprite(youLost, 0, 1*vbe_info.YResolution/6);
    drawSprite(retry, 0, 3*vbe_info.YResolution/6);
    drawSprite(exit_button,0,4*vbe_info.YResolution/6);
}

void game(){
    if(watergirlWin && fireboyWin && lever) {
        if(gameState==MP_GAME) sp_send_data(WON_INST);
        gameState=WON;
        new_frame();
        return;
    }
    int i, x, y;
    drawSprite(levelBackground, 0, 0);
    drawSprite(PlaceTime,0,0);
    display_time();
    drawSprite(retryLevel,40,40);

    for (i = 0; i < 1200; i++) {
        x = (i % 40) * 20;
        y = (i / 40) * 20;
        if (levelArray[i] == 0) {
        }
        else if (levelArray[i] == 1) {
            drawSprite(NoBorder, x, y);
        }
        else if (levelArray[i] == 2) {
            drawSprite(Top, x, y);
        }
        else if (levelArray[i] == 3) {
            drawSprite(Left, x, y);
        }
        else if (levelArray[i] == 4) {
            drawSprite(Right, x, y);
        }
        else if (levelArray[i] == 5) {
            drawSprite(Bottom, x, y);
        }
        else if (levelArray[i] == 6) {
            drawSprite(LeftRight, x, y);
        }
        else if (levelArray[i] == 50) {
            if(!pressed) {
                drawSprite(LeftRight, x, y);
            } 
            else levelArray[i]=0;
        }
        else if (levelArray[i] == 7) {
            drawSprite(TopBottom, x, y);
        }
        else if (levelArray[i] == 8) {
            drawSprite(RightBottom, x, y);
        }
        else if (levelArray[i] == 9) {
            drawSprite(LeftBottom, x, y);
        }
        else if (levelArray[i] == 10) {
            drawSprite(TopLeft, x, y);
        }
        else if (levelArray[i] == 11) {
            drawSprite(TopRight, x, y);
        }
        else if (levelArray[i] == 12) {
            drawSprite(LeftBottomRight, x, y);
        }
        else if (levelArray[i] == 13) {
            drawSprite(TopLeftBottom, x, y);
        }
        else if (levelArray[i] == 14) {
            drawSprite(LeftTopRight, x, y);
        }
        else if (levelArray[i] == 15) {
            drawSprite(TopRightBottom, x, y);
        }
        else if (levelArray[i] == 16) {
            checkGravityFireboy(x,y);
            updateFireboy(x,y);
        }
        else if (levelArray[i] == 17) {
            checkGravityWatergirl(x,y);
            updateWatergirl(x,y);
        }
        else if(levelArray[i]==18){
            drawSprite(FireHoleLeft,x,y);
        }
          else if(levelArray[i]==19){
            drawSprite(FireHoleCenter,x,y);
        }
          else if(levelArray[i]==20){
            drawSprite(FireHoleRight,x,y);
        }
        else if(levelArray[i]==21){
            drawSprite(WaterHoleLeft,x,y);
        }
          else if(levelArray[i]==22){
            drawSprite(WaterHoleCenter,x,y);
        }
        else if(levelArray[i]==23){
            drawSprite(WaterHoleRight,x,y);
        }
        else if(levelArray[i]==24){
            drawSprite(ToxicHoleLeft,x,y);
        }
        else if(levelArray[i]==25){
            drawSprite(ToxicHoleCenter,x,y);
        }
        else if(levelArray[i]==26){
            drawSprite(ToxicHoleRight,x,y);
        }
        else if(levelArray[i]==27){
            if(pressed)  drawSprite(ButtonPressedLeft,x,y);
             else drawSprite(ButtonLeft,x,y);
        }
        else if(levelArray[i]==28){
              if(pressed)  drawSprite(ButtonPressedRight,x,y);
             else drawSprite(ButtonRight,x,y);
        }
        else if(levelArray[i]==29){
            if(!lever)drawSprite(leftlever,x,y);
            else drawSprite(rightlever,x,y);
        }
          else if(levelArray[i]==30){
            if(!lever)drawSprite(doorred,x,y);
            else drawSprite(dooropen,x,y);
        }
         else if(levelArray[i]==31){
            if(!lever)drawSprite(doorblue,x,y);
            else drawSprite(dooropen,x,y);
        }
        else if(levelArray[i]==32){
            drawSprite(AllBorders,x,y);
        }
    }

}

void display_time() {
    drawSprite(num_sprites[time_info.day/10],205,50);
    drawSprite(num_sprites[time_info.day%10],235,50);

    drawSprite(barNumber,265,50);

    drawSprite(num_sprites[time_info.month/10],290,50);
    drawSprite(num_sprites[time_info.month%10],320,50);

    drawSprite(barNumber,350,50);

    drawSprite(num_sprites[time_info.year/10],370,50);
    drawSprite(num_sprites[time_info.year%10],400,50);
    
    drawSprite(num_sprites[time_info.hour/10],460,50);
    drawSprite(num_sprites[time_info.hour%10],490,50);

    drawSprite(pontosNumber,510,50);
    drawSprite(num_sprites[time_info.minute/10],535,50);
    drawSprite(num_sprites[time_info.minute%10],565,50);

}


void new_frame() {
    switch (gameState) {
        case START:
            initial_menu();
            break;
        case GAME:
            game();
            break;   
        case LOST:
            lost_menu();
            break;
         case WON:
            won_menu();
            break;
         case MP_GAME:
            game();
            break;
         case CONNECTING:
            wait_menu();
            break;
        default:
            break;
            
    }
     drawSprite(mouse, mouse_packet.x, mouse_packet.y);
}


