#pragma once
#include <lcom/lcf.h>
#include "sprites/grass.xpm"
#include "video_gr.h"

typedef struct {
    int x;   
    int y;    
    xpm_image_t img;
} grass;

void (create_grass)();

void (draw_grass)();
