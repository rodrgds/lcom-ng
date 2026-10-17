#ifndef _MOUSEMODEL_H_
#define _MOUSEMODEL_H_

#include "../position.h"
#include "../Sprite/sprite.h"
#include "../XPMS/mouse.xpm"
#include <stdint.h>
#include <stdio.h>
#include <minix/sysutil.h>


typedef struct Mouse{
    Position pos;
    Sprite* sprite;
} Mouse; 

Mouse createMouse( uint16_t x, uint16_t y);
void destroyMouse(Mouse m);
#endif
