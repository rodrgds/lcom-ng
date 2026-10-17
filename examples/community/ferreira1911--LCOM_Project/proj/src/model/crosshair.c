#include <lcom/lcf.h>

#include "crosshair.h"

#include "devices/video.h"

Crosshair crosshair;

extern unsigned h_res;
extern unsigned v_res;

void (crosshair_init)(int16_t x, int16_t y){
    crosshair.x = x;
    crosshair.y = y;
    crosshair.width = 15;
    crosshair.height = 15;
}

void (crosshair_update_position)(int16_t delta_x, int16_t delta_y){
    crosshair.x += delta_x;
    crosshair.y -= delta_y;

    if (crosshair.x < 0){
        crosshair.x = 0;
    } else if (crosshair.x + crosshair.width > (int16_t) h_res) {
        crosshair.x = (int16_t) h_res - crosshair.width;
    }

    if (crosshair.y < 0) {
        crosshair.y = 0;
    }
    else if (crosshair.y + crosshair.height > (int16_t) v_res) {
        crosshair.y = (int16_t) v_res - crosshair.height;
    }
}
