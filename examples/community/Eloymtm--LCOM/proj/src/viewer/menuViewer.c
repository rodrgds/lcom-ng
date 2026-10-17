#include "menuViewer.h"

int drawMenu() {
    reset_video();
    if(drawTitle()!=0)return 1;
    if(drawPlayButton()!=0)return 1;
    if(drawInstrButton()!=0)return 1;
    if(drawQuitButton()!=0)return 1;
    if(drawSelected(0, 0x000000)!=0)return 1;

    return 0;
}

int drawTitle() {

    uint16_t x = 415;
    uint16_t y = 100;

    if(vg_draw_rectangle(0, 0, 1152, 864, 0xFFFFFF)!=0)return 1;

    if(show_xpm(batalhaNaval,x,y)!=0)return 1;

    return 0;
}



int drawPlayButton() {

    uint16_t x = 500;
    uint16_t y = 410;


    if(show_xpm(play,x,y)!=0)return 1;

    return 0;
}


int drawInstrButton() {

    uint16_t x = 500;
    uint16_t y = 550;


    if(show_xpm(instr,x,y)!=0)return 1;

    return 0;
}


int drawQuitButton() {


    uint16_t x = 500;
    uint16_t y = 687;


    if(show_xpm(quit,x,y)!=0)return 1;

    return 0;
}

int drawSelected(int selectedMenu, uint32_t color){

    if(vg_draw_vline(503 ,413 + (selectedMenu * 137), 104, color)!=0)return 1;
    if(vg_draw_vline(502 ,413 + (selectedMenu * 137), 104, color)!=0)return 1;
    if(vg_draw_vline(686 ,413 + (selectedMenu * 137), 104, color)!=0)return 1;
    if(vg_draw_vline(687 ,413 + (selectedMenu * 137), 104, color)!=0)return 1;

    if(vg_draw_hline(502 ,413 + (selectedMenu * 137), 184, color)!=0)return 1;
    if(vg_draw_hline(502 ,414 + (selectedMenu * 137), 184, color)!=0)return 1;
    if(vg_draw_hline(502 ,515 + (selectedMenu * 137), 184, color)!=0)return 1;
    if(vg_draw_hline(502 ,516 + (selectedMenu * 137), 184, color)!=0)return 1;

    return 0;
}
