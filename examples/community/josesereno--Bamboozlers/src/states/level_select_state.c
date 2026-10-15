#include "menu_state.h"

extern uint8_t scancode;
extern Menu_state menu_state;

extern struct packet mouse_packet;
extern uint16_t yRes, xRes;

Level_state level_state = LEVEL_ONE;

void (update_keyboard_level)(){
    read_scancode();

    switch (scancode){
        case Q_KEY:
            menu_state = MENU;
            break;
        case ESC_BREAK:
            menu_state = EXIT;
            break;
        default:
            break;
    }
}

void (update_mouse_level)(){

    if (mouse_packet.lb) {
        if (select_item(level_one)) {
            map_set_up(0);
            menu_state = GAME;
        }

        else if (select_item(level_two) && level_state != LEVEL_ONE) {
            map_set_up(1);
            menu_state = GAME;
        }

        else if (select_item(level_three) && level_state == LEVEL_THREE) {
            map_set_up(2);
            menu_state = GAME;
        }
    }
}
