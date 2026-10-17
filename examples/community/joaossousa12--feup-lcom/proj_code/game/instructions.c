//
// Created by Bernardo on 24/05/2023.
//

#include "instructions.h"

extern int gameMode;
extern uint8_t *frame_buffer;
extern vbe_mode_info_t mode_info;
extern Sprite *sprites[MAX_SPRITES];


int drawInstructionsPage(){
    drawSprite(sprites[1], 0, 0);
    return 0;
}

void process_keyboard_instructions(uint8_t scancode){
    switch (scancode) {
        case MAKE_ESC:
            gameMode = MENU_MODE;
            break;
        default:
            break;

    }
}


