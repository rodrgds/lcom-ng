#include "game.h"

unsigned int mode = MENU_MODE;
bool is_in_game = false;

int waiting_time = 0;

extern int x, y;
extern struct packet pp;

extern uint8_t bytes[];

extern bool quit;

int (run_game)() {
    switch (mode) {
        case MENU_MODE: 
            if (display_menu_page()) {
                    return 1;
            }
            if (menu_display_mouse(x, y)) {
                return 1;
            }                                

            break;
        case GAME_MODE:
            if (!is_in_game) {
                if (init_game()) {
                    return 1;
                }
            }

            if (main_game_display_page()) {
                return 1;
            }

            if (main_game_display_mouse(x, y)) {
                    return 1;
            }

            if (main_game_control()) {
                return 1;
            }                                

            break;
        case WINNER_MODE:
            if (display_winner_page()) {
                return 1;
            }

            if (winner_display_mouse(x, y)) {
                return 1;
            }
                                
            break;
        case RECORD_MODE:
            if (display_record_page()) {
                return 1;
            }

            if (record_display_mouse(x, y)) {
                return 1;
            }

            break;
        case WAITING_MODE:
            if (main_game_display_page()) {
                return 1;
            }

            if (main_game_display_mouse(x, y)) {
                return 1;
            }

            if (waiting_time == 2) {
                waiting_time = 0;

                if (!is_in_game) {
                    destroy_all_cards();
                    mode = WINNER_MODE;
                } else {
                    mode = GAME_MODE;
                    unturn_selected_cards();
                }
            }

            break;
        case EXIT_MODE:
            quit = true;
            
            break;
    }

    copy_back_to_frame();
    erase_back_buffer();

    return 0;
}

int (game_mouse)() {
    mouse_update_position();

    if (mode == MENU_MODE) {
        menu_control_mouse(&pp, x, y);
    } else if (mode == GAME_MODE) {
        main_game_control_mouse(&pp, x, y);
    } else if (mode == WINNER_MODE) {
        if (winner_control_mouse(&pp, x, y)) {
            return 1;
        }
    } else if (mode == RECORD_MODE) {
        record_control_mouse(&pp, x, y);
    }

    return 0;
}

void (game_kbc)() {
    if (mode == MENU_MODE) {
        menu_control_kbc(bytes);
    } else if (mode == GAME_MODE) {
        main_game_control_kbc(bytes);
    } else if (mode == WINNER_MODE) {
        winner_control_kbc(bytes);
    } else if (mode == RECORD_MODE) {
        record_control_kbc(bytes);
    }
}

void (waiting_mode_timer)() {
    if (mode == WAITING_MODE) {
        waiting_time++;
    }
}
