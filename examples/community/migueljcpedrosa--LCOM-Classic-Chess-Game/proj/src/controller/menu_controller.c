#include "menu_controller.h"
#include "state/state.h"
#include "drivers/keyboard/mkbd.h"
#include "model/menu/menu.h"

void menu_kbd(uint8_t scan_code[2], int size){
    if (size == 1){
        switch(scan_code[0]){
        case ESC:
            set_state(EXIT);
            break;
        case ENTER:
            set_state(WRITE_NAME);
            break;
        default:
            break;
        }
    }
}

void menu_cursor(CursorInput* input){

    if (input->leftClick){

        if (menu_clicked_play(input)){
            set_state(WRITE_NAME);
        }
        else if (menu_clicked_exit(input)){
            set_state(EXIT);
        }
    }

}

