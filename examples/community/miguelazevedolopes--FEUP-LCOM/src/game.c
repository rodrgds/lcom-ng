#pragma once
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include "stdio.h"
#include "game.h"
#include "string.h"
#include "v.c"
#include "timer.c"
#include "menu.c"
#include "rtc.c"

/**
 * @brief Self explanatory, refers to the players points
 * 
 */
uint8_t playerOnePoints=0;
uint8_t playerTwoPoints=0;

/**
 * @brief Used to subscribe interrupts
 * 
 */
uint8_t kb_interrupt_request,timer_interrupt_request,mouse_bitmask;


/**
 * @brief Keyboard scancode
 * 
 */
extern uint8_t scancode;

uint8_t size = 0;
/**
 * @brief Holds the mouse packet
 * 
 */
uint8_t bytes[3];
/**
 * @brief Used to store max score info
 * 
 */
int maxDay,maxMonth,maxYear;
/**
 * @brief Used to store max score info
 * 
 */
unsigned int maxScoreP1,maxScoreP2,minMinutes,minSeconds;
/**
 * @brief Flag used to control game/menu flow
 * 
 */
bool gameStarted=false,initialMenuFlag=true,pauseMenuFlag=false,firstTime=true,maxScoreMenuFlag=false;

int handlePoints(int playerWhoWon){
    if(playerWhoWon==1){
        playerOnePoints++;
        draw_points();
        sleep(1);
        return 2;
    }
    else if(playerWhoWon==2){
        playerTwoPoints++;
        draw_points();
        sleep(1);
        return 2;
    }
    if(playerOnePoints==5||playerTwoPoints==5){
        if((minutes<minMinutes)||(minutes==minMinutes&&seconds<minSeconds)){
            read_date();
            maxDay=date.day_month;
            maxMonth=date.month;
            maxYear=date.year;
            minMinutes=minutes;
            minSeconds=seconds;
            maxScoreP1=playerOnePoints;
            maxScoreP2=playerTwoPoints;
        }
        if(playerOnePoints==5){
            gameStarted=false;
            drawGameOver(1);
            sleep(2);
            initialMenuFlag=true;
            firstTime=true;
            clearScreen();
        }
        else{
            gameStarted=false;
            drawGameOver(2);
            sleep(2);
            initialMenuFlag=true;
            firstTime=true;
            clearScreen();
        }
        return 1;
    }
    return 0;
}



void initStartGame(){
    init_bars();
    init_ball();
    init_points();
    init_timer();
}


void startGame(){
    initStartGame();
    mouse_subscribe_int(&mouse_bitmask);
    write_command(MOUSE_WRITE_COMMAND,ENABLE_DATA_REPORT);
    keyboard_subscribe_int(&kb_interrupt_request);
    timer_subscribe_int(&timer_interrupt_request);
    //ESC_flag is true if ESC breakcode is read from the buffer
    bool ESC_flag = false;
    int ipc_status;
    message msg;
    int r;
    keyboard_state state_player1 = null;
    keyboard_state state_player2 = null;
    int response=-1;
    //stops when the ESC breakcode is written in the console
    while (!ESC_flag) {
        // Get a request message
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
        }
        if (is_ipc_notify(ipc_status)) { // received notification
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:                                 // hardware interrupt notification
            if (msg.m_notify.interrupts &timer_interrupt_request) {
                timer_ih();
                if(firstTime&&gameStarted){
                    state_player1=null;
                    state_player2=null;
                    drawStartGame();
                    firstTime=false;
                }
                else if(firstTime&&pauseMenuFlag){
                    clearScreen();
                    loadPauseMenu();
                    firstTime=false;
                }
                else if(firstTime&&initialMenuFlag){
                    loadMenu();
                    firstTime=false;
                }
                else if(firstTime&&maxScoreMenuFlag){
                    clearScreen();
                    loadScoreMenu();
                    firstTime=false;
                }
                if (gameStarted){
                    update_game_elapsed_time();
                    draw_time(minutes,seconds);
                    keyboard_move_bar(state_player1, true);
                    keyboard_move_bar(state_player2, false);
                    response=handlePoints(move_ball());
                    if(response==2){
                        reset_bars();
                        reset_ball();
                    };
                }        
            }
            if (msg.m_notify.interrupts & kb_interrupt_request) { // subscribed interrupt
                kbc_ih(); //C interrupt handler
                update_state(&state_player1,scancode,true);
                update_state(&state_player2,scancode,false);
                if(gameStarted){
                    if(scancode==ESC_KEY){
                        pauseMenuFlag=true;
                        firstTime=true;
                        gameStarted=false;
                    }
                }
            }
            if (msg.m_notify.interrupts & mouse_bitmask) { 
                mouse_ih();
                if(initialMenuFlag){
                    if (size==0&&(data&BIT(3))==0){
                        continue;
                    }
                    else if(size<=2){
                        bytes[size]=data;
                        size++;
                    }
                    if (size==3){
                        struct packet pk;
                        mouse_handle_packet(bytes,&pk,size);
                        move_pointer(&pk);
                        size=0;
                        response=detectMenuOptionClick(&pk);     
                        if(response==3){
                            ESC_flag=true;
                        }
                        else if(response==1){
                            clearScreen();
                            initialMenuFlag=false;
                            pauseMenuFlag=false;
                            firstTime=true;         
                            gameStarted=true;
                        }
                        else if(response==2){
                            firstTime=true;
                            initialMenuFlag=false;
                            maxScoreMenuFlag=true;     
                        }
                    }
                }
                else if (maxScoreMenuFlag){
                    if (size==0&&(data&BIT(3))==0){
                        continue;
                    }
                    else if(size<=2){
                        bytes[size]=data;
                        size++;
                    }
                    if (size==3){
                        struct packet pk;
                        mouse_handle_packet(bytes,&pk,size);
                        move_pointer(&pk);
                        size=0;   
                        response=goBackToMainMenu(&pk);
                        if(response){
                            maxScoreMenuFlag=false;
                            clearScreen();
                            initialMenuFlag=true;
                        }
                    }
                }
                else if(pauseMenuFlag){
                    if (size==0&&(data&BIT(3))==0){
                        continue;
                    }
                    else if(size<=2){
                        bytes[size]=data;
                        size++;
                    }
                    if (size==3){
                        struct packet pk;
                        mouse_handle_packet(bytes,&pk,size);
                        move_pointer(&pk);
                        size=0;
                        response=detectPauseMenuOptionClick(&pk);
                        if(response==1){
                            initialMenuFlag=true;
                            pauseMenuFlag=false;
                            firstTime=true;
                            clearScreen();
                        }
                        else if(response==2){
                            clearScreen();
                            state_player1=null;
                            state_player2=null;
                            drawResumeGame();
                            pauseMenuFlag=false;
                            gameStarted=true;
                            firstTime=false;
                        }
                    }
                    
                }
                
            }
            break;

            default:
            break; // no other notifications expected: do nothing
        }
        }
        else { //received a standard message, not a notification
        // no standard messages expected: do nothing
        }
    }
    write_command(MOUSE_WRITE_COMMAND,DISABLE_DATA_REPORT);
    timer_unsubscribe_int();
    keyboard_unsubscribe_int();
    mouse_unsubscribe_int();
    endGame();

}
void readData(){
    char content[5];
    FILE * fp;
    fp = fopen("/home/lcom/labs/proj/src/file1.txt", "r");
    fgets(content,5,fp);
    if(content[1]>=48&&content[1]<58)
        maxDay=(content[0]-48)*10+content[1]-48;
    else
        maxDay=content[0]-48;
    fgets(content,5,fp);
    if(content[1]>=48&&content[1]<58)
        maxMonth=(content[0]-48)*10+content[1]-48;
    else
        maxMonth=content[0]-48;
    fgets(content,5,fp);
    if(content[1]>=48&&content[1]<58)
        maxYear=(content[0]-48)*10+content[1]-48;
    else
        maxYear=content[0]-48;
    fgets(content,5,fp);
    if(content[1]>=48&&content[1]<58)
        minMinutes=(content[0]-48)*10+content[1]-48;
    else
        minMinutes=content[0]-48;
    fgets(content,5,fp);
    if(content[1]>=48&&content[1]<58)
        minSeconds=(content[0]-48)*10+content[1]-48;
    else
        minSeconds=content[0]-48;
    fgets(content,5,fp);
    maxScoreP1=content[0]-48;
    fgets(content,5,fp);
    maxScoreP2=content[0]-48;
    fclose(fp);    
}
void saveData(){
    FILE * fp;
    fp = fopen("/home/lcom/labs/proj/src/file1.txt", "w");
    fprintf(fp,"%d%s%d%s%d%s",maxDay,"\n",maxMonth,"\n",maxYear,"\n");
    fprintf(fp,"%d%s%d%s" ,minMinutes,"\n",minSeconds,"\n");
    fprintf(fp,"%d%s%d%s" ,maxScoreP1,"\n",maxScoreP2,"\n");
    fclose(fp);
}


void endGame(){
    vg_exit();
    saveData();
    free_all();
}
