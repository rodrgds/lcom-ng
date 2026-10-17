#ifndef _SQUAREMODEL_H_
#define _SQUAREMODEL_H_

#include "../position.h"
#include "../Sprite/sprite.h"
#include <stdint.h>
#include <stdio.h>
#include <minix/sysutil.h>

typedef struct Square{
    Position pos;
    uint32_t color;
    uint32_t dirt_color;
    bool hasMine;
    uint8_t nCloseMines;
    bool isFlagged;
    bool full;
} Square; 

#endif


