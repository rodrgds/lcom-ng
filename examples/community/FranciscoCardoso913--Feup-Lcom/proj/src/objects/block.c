#include "block.h"

Block* (create_block)(int x_, int y_){
    Block* block = (Block*)malloc(sizeof(Block));

    if (block == NULL) return NULL;
    block->sprite = create_sprite((xpm_map_t)(block_xpm), x_, y_, 0, 0);
    block->sprite->y+= -block->sprite->height + 32 ;
    return block;

}
int (push)(Block * block,int x,int y){
    block->sprite->x=x;
    block->sprite->y=y;
    return 0;
}
