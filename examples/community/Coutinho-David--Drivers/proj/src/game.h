#ifndef GAME_H
#define GAME_H

#include "controllers/timer.h"

typedef enum {
    MENU_START,
    MENU_EXIT,
    bet5,
    bet10,
    bet25,
    bet50,
    BET,
    STAND,
    HIT,
    DOUBLE,
    DEALER_BET,
    DEALER_EXIT,
    PLAYER_BET,
    PLAYER_EXIT,
    PUSH_BET,
    PUSH_EXIT,
    BJ_BET,
    BJ_EXIT
} GameStates;

extern GameStates state;
extern int score[2];
extern int money, bet;
extern int result;
extern int fakeScore;

void game_start();
void change_bet(); 
void game_player_hit();
void game_player_stand();
void game_player_double();
void game_dealer();
void game_end();

#endif
