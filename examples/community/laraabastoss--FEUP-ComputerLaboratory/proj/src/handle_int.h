#ifndef _LCOM_HANDLE_INT_H_
#define _LCOM_HANDLE_INT_H_
#define RIGHT 2
#define LEFT 1
#define STOPPED 0
#define JUMP_OVER 999
#define LEVER_LEFT 111
#define LEVER_RIGHT 112
#define WON_INST 113
#define RESET_VAR 114
#define RESET_VAR_PLUS_LEVEL 115
#define RESET_VAR_LEVEL_ONE 116
#define EXIT_GAME 117


#include <minix/sysutil.h>
#include <lcom/lcf.h>


#include "devices/KBC.h"
#include "devices/mouse/mouse.h"
#include "devices/video/graphics.h"
#include "devices/rtc/rtc.h"
#include "devices/sp/serial_port.h"
#include "game/draw.h"
#include "game/elements.h"


typedef enum {
    EXECUTING,
    EXIT,
} ProjState;

typedef enum {
    START,   
    GAME,
    LOST,
    WON,
    MP_GAME,
    CONNECTING
} GameState;

/// @brief interacts with the game everytime there is a timer interrupt, used for the characters jumps and to move the plataform 
void handle_int_timer();

/// @brief interacts with the game everytime there is a keyboard interrupt, moving the characters according to the received scancode
void handle_int_keyboard();

/// @brief interacts with the game everytime there is a serial port interrupt in the multiplayer mode, processing the 8 bit variable received and, accordingly, moving the characters, pushes the lever, finishes or wins the level
void handle_sp_data_multiplayer();

/// @brief interacts with the game everytime there is a keyboard interrupt in the multiplayer mode, moving the characters according to the received scancode if it´s its own character, and sending the scancode to the serial port if it´s the other character
void handle_int_keyboard_multiplayer_transmit();

/// @brief interacts with the game everytime there is a mouse interrupt and the right button is clicked, used for clicking the buttons and pushing the lever
void handle_int_mouse_click();

/// @brief interacts with the game everytime there is a rtc interrupt, changing the data displayed on top of the screen
void handle_int_rtc();

/// @brief interacts with the game everytime there is a serial port interrupt, checking wahtºs the current state of the serial port
void handle_int_sp();

/// @brief resets all the variables used for the characters and the obstacules movements everytime a level is won or lost
void resetVariables();

#endif

