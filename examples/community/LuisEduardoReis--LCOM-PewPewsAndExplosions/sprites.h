#ifndef __SPRITES_H_
#define __SPRITES_H_

#include "graphics.h"

int init_font(char* path);
int freeFont();
int init_sprites(char* path);
int freeSprites();

int drawText(char* text, int x, int y);
int drawTextBlend(char* text, int x, int y, int blend);



#endif
