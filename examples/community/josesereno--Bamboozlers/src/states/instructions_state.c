#include "menu_state.h"

extern uint8_t scancode;
extern Menu_state menu_state;
extern Sprite *mouse;

extern struct packet mouse_packet;
extern uint16_t yRes, xRes;

void (update_keyboard_instructions)(){
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


