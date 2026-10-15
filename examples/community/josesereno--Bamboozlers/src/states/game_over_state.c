#include "menu_state.h"

extern uint8_t scancode;
extern Menu_state menu_state;

extern struct packet mouse_packet;
extern uint16_t yRes, xRes;

void (update_keyboard_game_over)(){
    read_scancode();

    switch (scancode){
    case ENTER: case Q_KEY:
        menu_state = MENU;
        break;
    case ESC_BREAK:
        menu_state = EXIT;
        break;
    }
}

void (update_mouse_game_over)(){
   if (mouse_packet.lb){
        if (select_item(play_again)) menu_state = LEVEL_SELECTION;
        else if (select_item(game_exit)) menu_state = EXIT;
    }
}
