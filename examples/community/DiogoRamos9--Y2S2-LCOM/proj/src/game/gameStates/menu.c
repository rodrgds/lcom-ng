#include <lcom/lcf.h>

#include "menu.h"

int option = 0;
bool is_mouse_over = false;

void draw_menu() {
    if ((cursor->x > 265 && cursor->x < 500) && (cursor->y > 215 && cursor->y < 273)) {
        option = 0;
        is_mouse_over = true;
    }
    
    else if ((cursor->x > 265 && cursor->x < 515) && (cursor->y > 273 && cursor->y < 327)) {
        option = 1;
        is_mouse_over = true;
    }
    
    else if ((cursor->x > 265 && cursor->x < 515) && (cursor->y > 327 && cursor->y < 385)) {
        option = 2;
        is_mouse_over = true;
    }
    
    else if ((cursor->x > 265 && cursor->x < 500) && (cursor->y > 385 && cursor->y < 435)) {
        option = 3;
        is_mouse_over = true;
    }
    else if ((cursor->x > 340 && cursor->x < 415) && (cursor->y > 450 && cursor->y < 490)) {
        option = 4;
        is_mouse_over = true;
    }
    else {
        is_mouse_over = false;
    }

    switch (option) {
        case 0:
            draw_sprite(menu_start_selected, 0, 0);
            draw_sprite(leaderboard, 375, 475);
            break;
        case 1:
            draw_sprite(menu_settings_selected, 0, 0);
            draw_sprite(leaderboard, 375, 475);
            break;
        case 2:
            draw_sprite(menu_instructions_selected, 0, 0);
            draw_sprite(leaderboard, 375, 475);
            break;
        case 3:
            draw_sprite(menu_exit_selected, 0, 0);
            draw_sprite(leaderboard, 375, 475);
            break;
        case 4:
            draw_sprite(menu_none_selected, 0, 0);
            draw_sprite(leaderboard_selected, 375, 475);
            break;
    }
}

void draw_leaderboard() {
    draw_sprite(leaderboard_background, 0, 0);
}
