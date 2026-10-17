#include "menu.h"


extern int gameMode;

extern uint8_t *frame_buffer;
extern vbe_mode_info_t mode_info;
extern uint16_t width,height;
extern unsigned bytesPerPixel;

extern Sprite *sprites[MAX_SPRITES];


int drawMenu(){
    memset(frame_buffer, 0xFFFFFF, width * height * bytesPerPixel);
    if(drawSprite(sprites[0], 0, 0)) return 1;
    return 0;
}

void process_keyboard_menu(uint8_t scancode){
    switch (scancode) {
        case MAKE_1:
            gameMode = GAME_MODE;
            drawGame();
            break;
        case MAKE_2:
            gameMode = INSTRUCTIONS_MODE;
            break;
        case MAKE_3:
            gameMode = EXIT_MODE;
            break;
        default:
            break;
    }
}



