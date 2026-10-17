#include "waitingViewer.h"


int (drawWaiting) (bool isPlayer1) {

    uint16_t x = 200;
    uint16_t y = 300;

    if(isPlayer1) {

        if(vg_draw_rectangle(0, 0, 1152, 864, 0x000000)!=0)return 1;

        if(show_xpm(player1ships,x,y)!=0)return 1;
    }

    else {

        if(vg_draw_rectangle(0, 0, 1152, 864, 0x000000)!=0)return 1;

        if(show_xpm(player2ships,x,y)!=0)return 1;
    }


    return 0;
}

