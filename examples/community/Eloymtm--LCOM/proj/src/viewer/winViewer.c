#include "winViewer.h"


int (drawWinPlayer) (bool isPlayer1) {

    uint16_t x = 275;
    uint16_t y = 250;

    if(vg_draw_rectangle(0, 0, 1152, 864, 0x000000)!=0)return 1;

    if(isPlayer1){
        if(show_xpm(win1,x,y)!=0)return 1;
    }

    else{
        if(show_xpm(win2,x,y)!=0)return 1;
    }

    return 0;
}
