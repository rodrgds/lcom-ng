#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdint.h>

#include "Controller/Devices/VideoCard/video_graphics.h"
#include "../../Model/Square/squareModel.h"
#include "../../View/GameView/gameView.h"

#define NUMBER_OF_BOMBS 35
#define GREEN_SQUARE_1 0x1BB302
#define GREEN_SQUARE_2 0x1ECC02
#define DIRT_SQUARE_1 0xB48818
#define DIRT_SQUARE_2 0x936A00

Square gameBoard[15][15];
bool firstMove;

void buildGameBoard();
void setProximityAlert(uint8_t bomb_col, uint8_t bomb_row);
bool positionIsRestricted(uint8_t pos_x, uint8_t pos_y, uint8_t x, uint8_t y);
void dig(uint8_t col, uint8_t row);
void deployBombs(uint8_t dig_x, uint8_t dig_y);

#endif
