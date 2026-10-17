#ifndef _LCOM_MODEL_H_
#define _LCOM_MODEL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/keyboard/KBC.h"
#include "controller/mouse/mouse.h"
#include "controller/graphics/graphics.h"
#include "controller/rtc/rtc.h"
#include "model/sprite.h"
#include "game/game.h"
#include "configs.h"
#include "xpm/mouse.xpm"
#include "xpm/menu.xpm"
#include "xpm/player1.xpm"
#include "xpm/player2.xpm"
#include "xpm/p1_up.xpm"
#include "xpm/p1_left.xpm"
#include "xpm/p1_right.xpm"
#include "xpm/p2_up.xpm"
#include "xpm/p2_left.xpm"
#include "xpm/p2_right.xpm"
#include "xpm/arena.xpm"
#include "xpm/num0.xpm"
#include "xpm/num1.xpm"
#include "xpm/num2.xpm"
#include "xpm/num3.xpm"
#include "xpm/num4.xpm"
#include "xpm/num5.xpm"
#include "xpm/num6.xpm"
#include "xpm/num7.xpm"
#include "xpm/num8.xpm"
#include "xpm/num9.xpm"
#include "xpm/percentage.xpm"
#include "xpm/twopoints.xpm"
#include "xpm/bar.xpm"
#include "xpm/p1win.xpm"
#include "xpm/p2win.xpm"
#include "xpm/controls.xpm"
#include "view/view.h"
#include "player.h"

#define A_KEY 30
#define W_KEY 17
#define S_KEY 31
#define D_KEY 32
#define Q_KEY 16
#define R_KEY 19
#define ENTER_KEY 28
#define M_KEY 50
#define I_KEY 23
#define J_KEY 36
#define K_KEY 37
#define L_KEY 38
#define UP_ARROW_KEY 0x48
#define LEFT_ARROW_KEY 0x4B
#define RIGHT_ARROW_KEY 0x4D
#define DOWN_ARROW_KEY 0x50
#define C_KEY 46

/**
 * @brief Enum that represents the different states of the system.
*/
typedef enum {
    RUN,
    QUIT,
} SystemState;

/**
 * @brief Enum that represents the different states of the menu.
*/
typedef enum {
    MENU,
    END,
    PLAY,
    CONTROLS,
} MenuState;

/**
 * @brief Struct that represents the information of the arena.
*/
typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t** map;
} Arena;

/**
 * @brief Function that initializes all sprites.
*/
void (setup_sprites)();

/**
 * @brief Function that frees all sprites.
*/
void (free_sprites)();

/**
 * @brief Function that, in case of double buffering, copies the buffer to the VRAM and increments a global counter, used to control the time of the game.
*/
void (change_timer_state)();

/**
 * @brief Function that changes the state of the RTC, incrementing the global counter, used to control the current date.
*/
void (change_rtc_state)();

/**
 * @brief Function that changes the state of the keyboard, taking into account the keys pressed. In case of pressing Enter, it initializes the game, always with the same initial conditions.
*/
void (change_kb_state)();

/**
 * @brief Function that changes the state of the mouse. In case of pressing the left button, in game mode, it changes the direction of the player according to the mouse movement and in menu mode, it changes the menu state.
*/
void (change_mouse_state)();

/**
 * @brief Function that creates the initial player area, with the dimensions given as arguments.
 * @param width Width of the player area.
 * @param height Height of the player area.
*/
void (create_player_area)(uint16_t width, uint16_t height);

#endif
