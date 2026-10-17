#ifndef __BLOCK_H
#define __BLOCK_H

#include <lcom/lcf.h>

#include "../drivers/graphics/graphics.h"
#include "../sprites/sprite.h"
#include "../xpm/background/block.xpm"



#define GRAVITY 1


typedef struct {
   Sprite * sprite;

} Block;

Block* (create_block)( int x_, int y_);
int (push)(Block * block,int x,int y);

#endif 
