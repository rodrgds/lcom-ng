#include "entity.h"
#include "graphics/video_gr.h"
#include <lcom/lcf.h>


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int draw_xpm(xpm_map_t xpm, int x, int y){
    xpm_image_t pic;
    uint32_t* colors = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &pic);
    for (int i=0;i<pic.height;i++){
        for (int j=0;j<pic.width;j++){
            if (vg_drawpixel(x+j,y+i,colors[i*pic.width + j])) return 1;
        }
    }
    return 0;
}





