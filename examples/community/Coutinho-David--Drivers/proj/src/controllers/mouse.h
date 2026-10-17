#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "utils.h"
#include "../game.h"

extern int mouse_x,mouse_y;

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
    GameStates state;
    void (*onClick)();
} Button;

void select_start(); 
void select_exit();
void select_a_bet();
void select_BET(); 
void select_stand();
void select_hit();
void select_double();

void (mouse_ih)();
int (mouse_init)();
#endif //MOUSE_H
