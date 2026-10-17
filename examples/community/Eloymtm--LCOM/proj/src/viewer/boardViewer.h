#ifndef BOARDVIEWER_H
#define BOARDVIEWER_H

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "../model/board.h"
#include "../model/structGame.h"
#include "../labs/video.h"
#include "../xpm/batalhaNaval1.h"
#include "../xpm/player1.h"
#include "../xpm/player2.h"
#include "../model/ship.h"
#include "../macros.h"

int (drawBoard)(bool ships);

int (drawGrid)(uint16_t xi);
int (drawShips)();
int (drawShip)(int x, int y, int n, bool vertical,uint32_t color);

int (paintBoard1)(Board *board);
int (paintBoard2)(Board *board);

void (drawHit)(Board *board,int scancode);

int (drawSelected_ship)(int selectedShip, uint32_t color);
int (drawSelected_shipBoard)(bool isPlayer1, Ship ship, uint32_t color);

Coordinate (convertCoord)(bool isPlayer1, Coordinate coor);
bool (setUpDone)(Board *board);

#endif // BOARDVIEWER_H
