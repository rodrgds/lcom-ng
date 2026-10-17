#include "menu.h"

#include <lcom/lcf.h>
#include "images/piece.xpm"
#include <stdint.h>

int start_button_x;
int start_button_y;
int quit_button_x;
int quit_button_y;
extern bool is_on_menu;


int display_menu() {
    if (vg_draw_rectangle(0,0,800,600,0xFFFFFF)) return 1;   
        
    if(draw_xpm((xpm_map_t) game_name_xpm, 100, 0)) return 1;
    if(draw_xpm((xpm_map_t) startbutton_xpm, 150, 300)) return 1;
    if(draw_xpm((xpm_map_t) quitbutton_xpm, 450, 300)) return 1;
    
    return 0;
}
