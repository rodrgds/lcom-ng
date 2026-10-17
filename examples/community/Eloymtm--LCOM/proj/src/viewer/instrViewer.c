#include "instrViewer.h"


int drawInstr() {

    uint16_t x = 200;
    uint16_t y = 200;

    if(vg_draw_rectangle(0, 0, 1152, 864, 0xFFFFFF)!=0)return 1;

    if(show_xpm(rules,x,y)!=0)return 1;

    return 0;
}

