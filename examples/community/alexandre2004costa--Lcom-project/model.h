
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "devices/serialPort.h"
#include "classes/Object.h"
#include "classes/Bomb.h"
#include "devices/graphic.h"
#include "controller.h"
#include "xpm/player.xpm"
#include "xpm/player2.xpm"
#include "xpm/back.xpm"
#include "xpm/healthBar.xpm"
#include "xpm/menu.xpm"
#include "xpm/mouse.xpm"
#include "xpm/mouseClick.xpm"
#include "xpm/multi.xpm"
#include "xpm/multiP1.xpm"
#include "xpm/multiP2.xpm"
#include "xpm/scores.xpm"
#include <unistd.h>


#define MOUSE_CODE_SERIAL 'M'
#define CHOOSE_P2 'P'
#define CHOOSE_P1 'T'
#define CHOOSE_REMOVE 'R'
#define START_GAME 'S'
#define LOST_LIFE 'L'



int (enableInterrupts)();

void (waitForInterrupt)();

int (EndDevices)();

void (loadSprites)();

void (movements)();

void (draws)();

void (manageOutInput)();

 void (mouseMovement)();

 void (leftMouseClick)(bool mine);

void (prepareGame)(bool solo);

void (freqActions)();
