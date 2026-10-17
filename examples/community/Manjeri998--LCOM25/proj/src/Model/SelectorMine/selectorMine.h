#ifndef _SELECTORMINE_H_
#define _SELECTORMINE_H_

#include "../position.h"
#include "../Sprite/sprite.h"
#include "../XPMS/mine.xpm"
#include <stdint.h>
#include <stdio.h>
#include <minix/sysutil.h>

#define INIT_POS_X 496
#define INIT_POS_Y 250


typedef struct SMine{
    Position pos;
    Sprite* sprite;
} SMine; 

SMine createSMine();
void destroySMine( SMine* mine);
void setSMinePosition( SMine* mine, uint16_t x, uint16_t y);

#endif
