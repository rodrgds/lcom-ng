#include <lcom/lcf.h>

#include <stdint.h>

#include "keyboard_controller.h"
#include "../pacman_movement_controller/pacman_movement_controller.h"
#include "../timer_controller/timer_controller.h"
#include "../../devices/kbc/keyboard/keyboard.h"
#include "../../devices/kbc/i8042.h"
#include "../../view/pacman_view/pacman_view.h"
#include "../../view/menu_view/menu_view.h"
#include "../../view/initialize_pixmaps.h"

extern bool make_code;
extern int num_bytes;
extern uint8_t scan_bytes[2];
extern bool full_scancode;

extern bool game_is_on;
extern bool pause_is_on;

void (pause_handler)() {
    pause_is_on = !pause_is_on;
    if (pause_is_on) {
        draw_pause_text();
        pause_game();
    } else {
        continue_game();
        draw_game_elements();
    }   
}

void (process_key)() {
    if (num_bytes == 1) {
        switch (scan_bytes[0]) {
            case ESC_MCODE:
                game_is_on = false;
                break;
            case SPACEBAR_MCODE:
                pause_handler();
                break;
        }
    }

    else if (num_bytes == 2 && scan_bytes[0] == SCAN_MSB && !pause_is_on) {
        switch (scan_bytes[1]) {
            case ARROW_UP_MCODE: 
                erase_pacman(); 
                pacmanMoveUp();
                draw_pacman();
                break;
            case ARROW_DOWN_MCODE: 
                erase_pacman(); 
                pacmanMoveDown(); 
                draw_pacman();
                break;
            case ARROW_RIGHT_MCODE: 
                erase_pacman(); 
                pacmanMoveRight(); 
                draw_pacman();
                break;
            case ARROW_LEFT_MCODE: 
                erase_pacman(); 
                pacmanMoveLeft(); 
                draw_pacman();
                break;
        }
    }

    if (full_scancode)
        num_bytes = 0;
}
