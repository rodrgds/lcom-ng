#include "game_over_controller.h"
#include "state/state.h"
#include "drivers/keyboard/mkbd.h"
#include "model/game_over/game_over.h"

void game_over_kbd(uint8_t scan_code[2], int size){
    if (size == 1){
        switch(scan_code[0]){
        case ESC:
            set_state(MENU);
            break;
        case ENTER:
            set_state(GAME);
            break;
        default:
            break;
        }
    }
}

void game_over_cursor(CursorInput* input){

    if (input->leftClick){

        if (game_over_clicked_play(input)){
            set_state(GAME);
        }
        else if (game_over_clicked_exit(input)){
            set_state(MENU);
        }
    }

}

