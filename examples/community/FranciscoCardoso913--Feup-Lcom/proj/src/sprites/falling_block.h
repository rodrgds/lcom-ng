#ifndef __FALLING_BLOCK_H__
#define __FALLING_BLOCK_H__

#include "sprite.h"
#include "../xpm/falling_block.xpm"

typedef struct {
    Sprite* sprite[3];
    Sprite* rope[10];
    int block_position;
    bool is_cut,is_on_ground;
    int n_rope;

} Falling_Block;






#endif /* __FALLING_BLOCK_H__ */
