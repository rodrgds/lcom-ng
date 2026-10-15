#include "menu_state.h"

extern uint8_t scancode;
extern Menu_state menu_state;
extern Sprite *mouse;

extern struct packet mouse_packet;
extern uint16_t yRes, xRes;

void (update_keyboard_menu)(){
    read_scancode();

    switch (scancode) {
        case ENTER:
            menu_state = LEVEL_SELECTION;
            break;
        case ESC_BREAK:
            menu_state = EXIT;
            break;
        default:
            break;
    }
}

void (update_mouse_menu)(){
    if (mouse_packet.lb) {
        if (select_item(levels)) menu_state = LEVEL_SELECTION;
        else if (select_item(instructions)) menu_state = INSTRUCTIONS;
    }
}
