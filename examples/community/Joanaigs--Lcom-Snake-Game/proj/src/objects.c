#include "objects.h"
#include "images/game_background.xpm"
#include "graphics.h"


void drawBackground(){
     background.map = xpm_load((xpm_map_t)game_background_xpm, XPM_8_8_8, &background.img);
    draw_xpm(background.img, background.map, 0, 0);
}
