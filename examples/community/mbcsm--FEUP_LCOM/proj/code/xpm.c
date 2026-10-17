#include <lcom/lcf.h>

#include "xpm.h"

void draw_xpm(int x, int y, uint16_t *sprite, xpm_image_t img, const uint16_t transp){
    int h_res = get_h_res();
    void *video_mem = get_video_mem();

    for (int i = y; i < y + img.height; i++){
        for (int j = x; j < x + img.width; j++) {
            uint16_t *ptr_VM = (uint16_t*)video_mem;
            ptr_VM += (i * h_res + j);
            if(*sprite != transp)
                *ptr_VM = *sprite;
            sprite++;
        }
    }
}

uint16_t getTransparency(){
    return xpm_transparency_color(XPM_5_6_5);
}

int align_x_center(xpm_image_t img){
    return 0 + (get_h_res() / 2) - (img.width / 2);
}

