#ifndef DYNAMICSPRITE_H
#define DYNAMICSPRITE_H

#include "sprite.h"
#include <lcom/lcf.h>
#include <stdbool.h>
#include "../gpu.h"

typedef struct DynamicSprite DynamicSprite;

struct DynamicSprite {
    Sprite sprite;            
    uint32_t *hoverMap; 
    uint32_t *originalMap;
    bool isHover;  
    void (*destroy)(DynamicSprite* self);
    int (*draw)(DynamicSprite* self);
    void(*setHover)(DynamicSprite *self,bool hover);
    bool(*isInside)(DynamicSprite *self,int x,int y);
};

DynamicSprite *create_dynamicSprite(const char *xpm[],
                                    int x, int y,int xspeed, int yspeed, uint32_t fromColor,uint32_t toColor);

#endif
