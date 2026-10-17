#ifndef LOGIC_H
#define LOGIC_H


#include <lcom/lcf.h>

#include "Game.h"
#include "devices/serialPort.h"

#define WIN_POS_X 785
#define WIN_POS_Y 550

int (loadSprites)();
int (gameStateInit)(bool * running);
int (keyboardLogic)();
int (mouseLogic)();
int (timerLogic)();

#endif


