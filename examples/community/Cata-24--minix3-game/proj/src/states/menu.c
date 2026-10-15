#include "../xpm/cursor.xpm"
#include "../xpm/menu/new-game.xpm"
#include "../xpm/menu/new-game-selected.xpm"
#include "../xpm/menu/resume.xpm"
#include "../xpm/menu/resume-selected.xpm"
#include "../xpm/menu/exit-selected.xpm"
#include "../xpm/menu/exit.xpm"
#include "../xpm/menu/game-start.xpm"
#include "../xpm/menu/game-over.xpm"
#include "../xpm/menu/pause.xpm"


#include <lcom/lcf.h>
#include "menu.h"

extern vbe_mode_info_t mode_info; // for the mouse
extern struct mouse_packet pack;
extern int (*map_funcs[2][4])(void);
extern bool done;
extern bool left_mouse_clicked(struct mouse_packet *pack);
extern bool mouse_hover(struct Sprite* button);

extern int state;
int substate = 0; // alterna entre menu, pausa e game over (0, 1, 2)
int over_count = 0; // tempo que fica no game over antes de trocar para o menu

struct Sprite *mouse, *exit_button, *logo, *main_button, *main_button_selected, *exit_button_selected;

int (load_menu_state)() {
    printf("Beginning of the load menu state!!");
    loadSprite(&mouse, (xpm_map_t)cursor_xpm);
    loadSprite(&exit_button, (xpm_map_t)exit_xpm);
    loadSprite(&exit_button_selected, (xpm_map_t)exit_selected_xpm);

    if (substate == 0) {
        loadSprite(&logo, (xpm_map_t)game_start_xpm);
        loadSprite(&main_button, (xpm_map_t)new_game_xpm);
        loadSprite(&main_button_selected, (xpm_map_t)new_game_selected_xpm);
    } else if (substate == 1) {
        loadSprite(&logo, (xpm_map_t)pause_xpm);
        loadSprite(&main_button, (xpm_map_t)resume_xpm);
        loadSprite(&main_button_selected, (xpm_map_t)resume_selected_xpm);
    } else if (substate == 2) {
        loadSprite(&logo, (xpm_map_t)game_over_xpm);
    }
    
    int center_x = (mode_info.XResolution - main_button->width) / 2;

    main_button->x_pos = center_x - 12;
    main_button->y_pos = 300;

    exit_button->x_pos = center_x;
    exit_button->y_pos = main_button->y_pos + main_button->height + 20;
        
    logo->x_pos = (mode_info.XResolution - logo->width) / 2;
    logo->y_pos = 80;

    main_button_selected->x_pos = main_button->x_pos;
    main_button_selected->y_pos = main_button->y_pos;

    exit_button_selected->x_pos = exit_button->x_pos;
    exit_button_selected->y_pos = exit_button->y_pos;

    return 0;
}

int (draw_menu_state)() {
    if (substate != 2) {
        if (mouse_hover(main_button)) {
            drawSprite(main_button_selected, false);
        }

        else  {
            drawSprite(main_button, false);
        }

        if (mouse_hover(exit_button)) {
            drawSprite(exit_button_selected, false);
        }
        else  {
            drawSprite(exit_button, false);
        }
    } else {
        if (over_count == 60) {
            over_count = 0;
            substate = 0;
            load_menu_state();
        }
        over_count++;
    }

    drawSprite(logo, false);
    drawSprite(mouse, false);
    
    return 0;
}

int (mouse_menu_ih)() {
    mouse->x_pos += pack.x_pos;
    mouse->y_pos += pack.y_pos;

    if (mouse->x_pos < 0) mouse->x_pos = 0;
    else if (mouse->x_pos > (int)mode_info.XResolution - 1) mouse->x_pos = mode_info.XResolution - 1;

    if (mouse->y_pos < 0) mouse->y_pos = 0;
    else if (mouse->y_pos > (int)mode_info.YResolution - 1) mouse->y_pos = mode_info.YResolution - 1;

    if (left_mouse_clicked(&pack) && substate != 2) {
        if (mouse_hover(main_button)) {
            if (substate == 0) {
                state = 1; 
                map_funcs[state][0](); 
            } else if (substate == 1) state = 1;
        } else if (mouse_hover(exit_button)) {             
            if (substate == 0) done = true;
            else if (substate == 1) {
                substate = 0;
                load_menu_state();
            }      
        }
    }

    return 0;
}

int (kbd_menu_ih)() {
    printf("Not yet implemented");
    return 0;
}
