#include "menu.h"

void menu_handle_mouse() {    
    if (state == MENU) {
        if (mouse_clicked_button(single_machine)) {
            state = GAME;
            menu_index = 0;
        }

        
        if (mouse_clicked_button(quit)) state = QUIT;
    }

    if (state == GAME) {
        if (mouse_clicked_button(return_button)) {
            state = MENU;
            menu_index = 0;
            load_board();
        }

        if (mouse_clicked_button(undo)) {
            undo_move();
        }
    }
}

void menu_handle_keyboard(uint8_t scancode) {
    if (state == MENU) {
        if (scancode == KEY_ESC) state = QUIT;

        if (scancode == KEY_UP) menu_index = (menu_index - 1) % 3;
            
        if (scancode == KEY_DOWN) menu_index = (menu_index + 1) % 3;

        if (scancode == KEY_ENTER) {
            if (menu_index == 0) state = GAME;
            if (menu_index == 1) state = QUIT;

            menu_index = 0;
        }
    }

    else if (state == GAME) {
        if (scancode == KEY_ESC) { 
            state = MENU;
            load_board();
        }

        if (scancode == KEY_Z) undo_move();

        if (scancode == KEY_UP) 
            menu_index = (menu_index - 1) % 2;

        if (scancode == KEY_DOWN)
            menu_index = (menu_index + 1) % 2;

        if (scancode == KEY_ENTER) {
            if (menu_index == 0) {
                state = MENU;
                menu_index = 0;
                load_board();
            }

            if (menu_index == 1) undo_move();
        }
    }
}

int draw_menu() {
    switch (state) {
        case MENU:
            if (draw_main_menu()) return 1;
            break;
        case GAME:
            if (draw_game_menu()) return 1;
            break;
        
        default:
            break;
    }

     return 0;
}

int draw_main_menu() {
    if (draw_sprite(logo, 908, 106)) return 1;
    
    if (draw_sprite(single_machine, 908, 342)) return 1;

    if (draw_sprite(quit, 908, 576)) return 1;

    return 0;
}

int draw_game_menu() {
    if (draw_sprite(return_button, 908, 598)) return 1;

    if (draw_sprite(undo, 908, 658)) return 1;
    
    if (draw_clock()) return 1;

    return 0;
}
