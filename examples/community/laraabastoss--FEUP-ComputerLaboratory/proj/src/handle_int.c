#include <stdio.h>
#include <stdlib.h>

#include "handle_int.h"

extern uint8_t scancode;
extern MouseInfo mouse_packet;
extern vbe_mode_info_t vbe_info;
extern RTC_structure time_info;
extern uint8_t sp_data;


ProjState projState = EXECUTING;
GameState gameState = START;
int level =1; 
int player=1;
int timer_break = 0;


extern int xOffsetFireboy;
extern int yOffsetFireboy ;
extern int xOffsetWatergirl;
extern int yOffsetWatergirl;
extern int fireboyState; // 0 for standing still, 1 for left, 2 for right
extern bool fireboyInAir;
extern int watergirlState;
extern bool watergirlInAir;
extern int UpFrameFireboy; 
extern int UpFrameWatergirl; 
extern int yPlatformBlockOffset;
extern bool upwardMotionFireboy;
extern bool upwardMotionWatergirl;
extern bool lever;
extern bool fireboyOnTopOfPlatform;
extern bool watergirlOnTopOfPlatform;
extern bool platformFinished;
extern bool two_player;
extern bool pressed;
extern bool watergirlWin;
extern bool fireboyWin;
extern int *levelArray;

extern uint8_t *mainBuffer;
extern uint8_t *secondaryBuffer;
extern uint32_t framebuffer_size;

void handle_int_timer() {
    memcpy(mainBuffer, secondaryBuffer, framebuffer_size);
    timer_break++;
    if (fireboyInAir && ((timer_break % 5) == 0)){
        new_frame();
    }
    else if (watergirlInAir && ((timer_break % 5) == 0)){
        new_frame();
    }
    else if (!platformFinished && pressed && ((timer_break % 30) == 0)){
        checkPlatformLogic(6); 
        new_frame();
    }
}

void handle_int_keyboard() { 
    switch (scancode) {
        case ESC_KEY:
            projState = EXIT;
            break;
        case D_KEY:
            if (!fireboyInAir){
                fireboyState = RIGHT;
                xOffsetFireboy+=20;
            }
            break;
        case A_KEY:
            if (!fireboyInAir){
                fireboyState = LEFT;
                xOffsetFireboy-=20;
            }
            break;
        case W_KEY:
            if (!fireboyInAir && (!fireboyOnTopOfPlatform || platformFinished)){
                fireboyInAir = true;
                upwardMotionFireboy = true;
                UpFrameFireboy = 5;
            }
            break;
        case LEFTARROW_KEY:
            if(!watergirlInAir){
                watergirlState = LEFT;
                xOffsetWatergirl -= 20;
            }
            break;
        case RIGHTARROW_KEY:
            if(!watergirlInAir){
                watergirlState = RIGHT;
                xOffsetWatergirl += 20;
            }
            break;

        case UPARROW_KEY:
            if(!watergirlInAir){
                watergirlInAir =true;
                upwardMotionWatergirl=true;
                UpFrameWatergirl=5;
            }
        case A_BREAK:
            if (!fireboyInAir){
                fireboyState = STOPPED;
            }
            break;
        case D_BREAK:
            if (!fireboyInAir){
                fireboyState = STOPPED;
            }
            break;
        case LEFTARROW_BREAK:
            if (!watergirlInAir){
                watergirlState = STOPPED;
            }
            break;
        case RIGHTARROW_BREAK:
            if (!watergirlInAir){
                watergirlState = STOPPED;
            }
            break;
        default: break;
    }
    new_frame();
}

void handle_sp_data_multiplayer(){
    switch (sp_data) {
        case ESC_KEY:
            projState = EXIT;
            break;
        case D_KEY:
            if (!fireboyInAir && player==2){
                fireboyState = RIGHT;
                xOffsetFireboy+=20;
            }
            break;
        case A_KEY:
            if (!fireboyInAir && player==2){
                fireboyState = LEFT;
                xOffsetFireboy-=20;
            }
            break;
        case W_KEY:
            if (!fireboyInAir && (!fireboyOnTopOfPlatform || platformFinished) && player==2){
                fireboyInAir = true;
                upwardMotionFireboy = true;
                UpFrameFireboy = 5;
            }
            break;
        case LEFTARROW_KEY:
            if(!watergirlInAir && player==1){
                watergirlState = LEFT;
                xOffsetWatergirl -= 20;
            }
            break;
        case RIGHTARROW_KEY:
            if(!watergirlInAir && player==1){
                watergirlState = RIGHT;
                xOffsetWatergirl += 20;
            }
            break;

        case UPARROW_KEY:
            if(!watergirlInAir && player==1){
                watergirlInAir =true;
                upwardMotionWatergirl=true;
                UpFrameWatergirl=5;
            }
        case A_BREAK:
            if (!fireboyInAir  && player==2){
                fireboyState = STOPPED;
            }
            break;
        case D_BREAK:
            if (!fireboyInAir  && player==2){
                fireboyState = STOPPED;
            }
            break;
        case LEFTARROW_BREAK:
            if (!watergirlInAir && player==1 ){
                watergirlState = STOPPED;
            }
            break;
        case RIGHTARROW_BREAK:
            if (!watergirlInAir && player==1){
                watergirlState = STOPPED;
            }
            break;
        case LEVER_LEFT:
            lever = false;
            break;
        case LEVER_RIGHT:
            lever = true;
            break;
        case WON_INST:
            printf("RECEIVED WON_INST\n");
            resetVariables();
            gameState = WON;
            break;
        case RESET_VAR:
            resetVariables();
            break;
        case RESET_VAR_PLUS_LEVEL:
            level++;
            resetVariables();
            break;
        case RESET_VAR_LEVEL_ONE:
            level = 1;
            resetVariables();
            break;
        case EXIT_GAME:
            projState = EXIT;
            break;
        default: 
            break;
    }
    new_frame();
}

void handle_int_keyboard_multiplayer_transmit() { 
    switch (scancode) {
        case ESC_KEY:
            projState = EXIT;
            break;
        case D_KEY:
            if (!fireboyInAir && player==1){
                fireboyState = RIGHT;
                xOffsetFireboy+=20;
            }
            break;
        case A_KEY:
            if (!fireboyInAir && player==1){
                fireboyState = LEFT;
                xOffsetFireboy-=20;
            }
            break;
        case W_KEY:
            if (!fireboyInAir && (!fireboyOnTopOfPlatform || platformFinished) && player==1){
                fireboyInAir = true;
                upwardMotionFireboy = true;
                UpFrameFireboy = 5;
            }
            break;
        case LEFTARROW_KEY:
            if(!watergirlInAir && player==2){
                watergirlState = LEFT;
                xOffsetWatergirl -= 20;
            }
            break;
        case RIGHTARROW_KEY:
            if(!watergirlInAir && player==2){
                watergirlState = RIGHT;
                xOffsetWatergirl += 20;
            }
            break;

        case UPARROW_KEY:
            if(!watergirlInAir && player==2){
                watergirlInAir =true;
                upwardMotionWatergirl=true;
                UpFrameWatergirl=5;
            }
        case A_BREAK:
            if (!fireboyInAir  && player==1){
                fireboyState = STOPPED;
            }
            break;
        case D_BREAK:
            if (!fireboyInAir  && player==1){
                fireboyState = STOPPED;
            }
            break;
        case LEFTARROW_BREAK:
            if (!watergirlInAir && player==2 ){
                watergirlState = STOPPED;
            }
            break;
        case RIGHTARROW_BREAK:
            if (!watergirlInAir && player==2){
                watergirlState = STOPPED;
            }
            break;
        default: break;
    }
    sp_send_data(scancode);
    new_frame();
}

void handle_int_mouse_click() {
    int i=mouse_packet.x/20;
    int j=mouse_packet.y/20;
    int position=(j*40)+i;
    if (gameState==GAME || gameState == MP_GAME){
        if(levelArray[position]==29){
            if(lever==false && mouse_packet.delta_x>0 && mouse_packet.positive_x==0){
                if(gameState==MP_GAME){
                    sp_send_data(LEVER_RIGHT);
                } 
                lever=true;
            }
            else if(lever==true && mouse_packet.delta_x<0 && mouse_packet.positive_x!=0){
                if(gameState==MP_GAME){
                    sp_send_data(LEVER_LEFT);
                } 
                lever=false;
            }
        } 
    }
    if(gameState==START){
    
        if (mouse_packet.x >= 0 && mouse_packet.y < 3*vbe_info.YResolution/6 && mouse_packet.y > 2*vbe_info.YResolution/6 && mouse_packet.x < 800){
            gameState=GAME;
            two_player=false;
            return;
        }
        else if (mouse_packet.x >= 0 && mouse_packet.y < 4*vbe_info.YResolution/6 && mouse_packet.y > 3*vbe_info.YResolution/6 && mouse_packet.x < 800){
            gameState=CONNECTING;
            wait_menu();
            timer_break = 0;
            if(sys_outb(0x3F8 + 1, BIT(0)) != 0) return;
            while (!sp_process_data()){}; //clear
            sp_send_data(SP_FIRST_INT);
        }

        else if (mouse_packet.x >= 0 && mouse_packet.y < 5*vbe_info.YResolution/6 && mouse_packet.y > 4*vbe_info.YResolution/6 && mouse_packet.x < 800){
            projState=EXIT;
        }
    }

    else if(gameState==WON){
        if (mouse_packet.x >= 0 && mouse_packet.y < 4*vbe_info.YResolution/6 && mouse_packet.y > 3*vbe_info.YResolution/6 && mouse_packet.x < 800){
                if(level==3){
                level=1;
                resetVariables();
                if (two_player) sp_send_data(RESET_VAR_LEVEL_ONE);
                new_frame();
                }
                else{
                    level++;
                    resetVariables();
                    if (two_player) sp_send_data(RESET_VAR_PLUS_LEVEL);
                    new_frame();
                }
                
        }

        else if (mouse_packet.x >= 0 && mouse_packet.y < 5*vbe_info.YResolution/6 && mouse_packet.y > 4*vbe_info.YResolution/6 && mouse_packet.x < 800){
            if (two_player) sp_send_data(EXIT_GAME);
            projState=EXIT;
        }
    }

    else if(gameState==LOST){
        if (mouse_packet.x >= 0 && mouse_packet.y < 4*vbe_info.YResolution/6 && mouse_packet.y > 3*vbe_info.YResolution/6 && mouse_packet.x < 800){
                resetVariables();
                if (two_player) sp_send_data(RESET_VAR);
                new_frame();
            
        }

        else if (mouse_packet.x >= 0 && mouse_packet.y < 5*vbe_info.YResolution/6 && mouse_packet.y > 4*vbe_info.YResolution/6 && mouse_packet.x < 800){
            if (two_player) sp_send_data(EXIT_GAME);
            projState=EXIT;
        }
    }
    else if(gameState==GAME){
        if(mouse_packet.x>=40 && mouse_packet.x<=80 && mouse_packet.y>=40 && mouse_packet.y<=80){
                resetVariables();
                new_frame();   
        }
    }
    else if (gameState==MP_GAME){
            if(mouse_packet.x>=40 && mouse_packet.x<=80 && mouse_packet.y>=40 && mouse_packet.y<=80){
                resetVariables();
                sp_send_data(RESET_VAR);
                new_frame();   
        }
    }
}


void handle_int_rtc() {
    if ((timer_break % 60 == 0) && (gameState == MP_GAME || gameState == GAME)){
        fill_rtc_structure();
        new_frame();
    }
}

void handle_int_sp(){
    unsigned int ser_state = 0;
    if (sp_data == SP_FIRST_INT){
        timer_break=0;
        player = 2;
        sp_send_data(SP_CLEAR); // random number to clear read data from FIRST_INT
        ser_state = SP_FIRST_INT;
    }
    if (sp_data == SP_CLEAR){
        ser_state = SP_CLEAR;
    }
    if (ser_state == 0) return;
    if (ser_state == SP_FIRST_INT){
        player=2;
        gameState = MP_GAME;
    }
    if (ser_state == SP_CLEAR){
        gameState = MP_GAME;
    }
    return;
}

void resetVariables(){
    lever=false;
    pressed=false;
    xOffsetFireboy=0;
    xOffsetWatergirl=0;
    yOffsetFireboy=0;
    yOffsetWatergirl=0;
    fireboyWin=false;
    watergirlWin=false;
    upwardMotionFireboy = false;
    upwardMotionWatergirl = false;
    watergirlState=STOPPED;
    fireboyState=STOPPED;
    fireboyOnTopOfPlatform = false;
    platformFinished=false;
    yPlatformBlockOffset=0;
    updateArrayWithLevel(level);
    if(two_player) gameState=MP_GAME;
    else gameState=GAME;
}
