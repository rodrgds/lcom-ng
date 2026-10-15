#ifndef __graphic_h
#define __graphic_h

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include <machine/int86.h>
#include "keyboard.h"
#include "mouse.h"



int (change_graphic_mode)(uint16_t mode);
int (map_memory)(uint16_t mode);
int (pixel_color)(uint16_t x, uint16_t y, uint32_t color);
void (fillBackground)(uint32_t *cor);
int (rotateBuffer)();
void (freeVideoBuffers)();

#endif
