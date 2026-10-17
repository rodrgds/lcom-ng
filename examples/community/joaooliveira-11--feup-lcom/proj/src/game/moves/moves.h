#ifndef MOVESH
#define MOVESH

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "../interrupts_actions/interrupts.h"


///@brief Checks if the boy sprite can move up by 5 pixels.
///@return 1 if there is a collision, 0 otherwise.
int check_FBmoveUP();

///@brief Checks if the boy sprite can move down by 5 pixels.
///@return 1 if there is a collision, 0 otherwise.
int check_FBmoveDOWN();

///@brief Checks if the boy sprite can move right by 5 pixels.
///@return 1 if there is a collision, 0 otherwise.
int check_FBmoveRIGHT();

///@brief Checks if the boy sprite can move left by 5 pixels.
///@return 1 if there is a collision, 0 otherwise.
int check_FBmoveLEFT();

///@brief Checks if the girl sprite can move up by 5 pixels.
///@return 1 if there is a collision, 0 otherwise.
int check_WGmoveUP();

///@brief Checks if the girl sprite can move down by 5 pixels.
///@return 1 if there is a collision, 0 otherwise.
int check_WGmoveDOWN();

///@brief Checks if the girl sprite can move right by 5 pixels.
///@return 1 if there is a collision, 0 otherwise.
int check_WGmoveRIGHT();

///@brief Checks if the girl sprite can move left by 5 pixels.
///@return 1 if there is a collision, 0 otherwise.
int check_WGmoveLEFT();

///@brief Moves the boy sprite up by 5 pixels.
void move_FBup();

///@brief Moves the boy sprite down by 5 pixels.
void move_FBdown();

///@brief Moves the boy sprite right by 5 pixels.
void move_FBright();

///@brief Moves the boy sprite left by 5 pixels.
void move_FBleft();

///@brief Moves the girl sprite up by 5 pixels.
void move_WGup();

///@brief Moves the girl sprite down by 5 pixels.
void move_WGdown();

///@brief Moves the girl sprite right by 5 pixels.
void move_WGright();

///@brief Moves the girl sprite left by 5 pixels.
void move_WGleft();

///@brief Handles mouse clicks in the MENU state.
void mouse_MENU();

///@brief Handles mouse clicks in the INSTRUCTIONS state.
void mouse_INSTRUCTIONS();

///@brief Handles mouse clicks in the MENU state.
void mouse_PLAYING();

///@brief Handles mouse clicks in the GAMEOVER state.
void mouse_GAMEOVER();

///@brief Handles mouse clicks in the GAMEWIN state.
void mouse_GAMEWIN();

///@brief Checks for collisions between the target position and walls, traps, levers, and barriers.
///@param targetXPOS The target X position.
///@param targetYPOS The target Y position.
///@param PlayerType The player type (0 for boy, 1 for girl).
///@return 1 if there is a collision, 0 otherwise.
int check_colisions(uint16_t targetXPOS, uint16_t targetYPOS, int PlayerType);

///@brief Checks for mouse clicks and performs the corresponding actions.
void check_mouse_clicks();

///@brief Checks if the players have reached the winning position.
///If they have, changes the gamestate to GAMEWIN_MENU.
void check_gameWin();

///@brief Checks if the player is near a lever.
///@param lever The lever to check.
///@param PlayerType The player type (0 for boy, 1 for girl).
///@return 0 if the player is near the lever, 1 otherwise.
int check_nearBY(lever_t lever, int PlayerType);

///@brief Resets the state of the barriers, levers, and lever countdown.
void reset_barrier();

///@brief Resets the state of the mouse clicks.
void reset_mouseClicks();


typedef void (*funct1_t)();
static funct1_t funcs1[] = {
    mouse_MENU,
    mouse_GAMEWIN,
    mouse_GAMEOVER,
    mouse_INSTRUCTIONS,
    mouse_PLAYING,
};

#endif
