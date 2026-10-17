#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "../../Controller/Devices/VideoCard/video_graphics.h"
#include <lcom/lcf.h>
#include <stdio.h>

uint8_t* background;

int createBackground();
void setBackground(uint32_t* img);

#endif
