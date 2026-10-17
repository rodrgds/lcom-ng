#ifndef _MOUSECONTROLLER_H_
#define _MOUSECONTROLLER_H_

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdint.h>

#include "../Devices/Mouse_Keyboard/Mouse/mouse.h"
#include "../../Model/Mouse/mouseModel.h"
#include "../GameController/gameController.h"
#include "../../View/GameView/gameView.h"

extern bool firstMove;
extern uint8_t flags;
extern bool exploded;

void handlePacket(Mouse* mouse);
void updatePosition(Mouse* mouse);
void updatePositionX(Mouse* mouse);
void updatePositionY(Mouse* mouse);
uint8_t getRowInGame(Mouse* mouse);
uint8_t getColInGame(Mouse* mouse);
void handleRightClick(Mouse* mouse);
void handleLeftClick(Mouse* mouse);

#endif
