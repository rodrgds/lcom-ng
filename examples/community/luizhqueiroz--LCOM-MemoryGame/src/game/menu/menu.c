#include "menu.h"

bool menu_clicked = false;

extern unsigned int mode;

int (display_menu_page)() {
    if (display_sprite(background, 0, 0)) {
        return 1;
    }

    if (display_sprite(game_title, 214, 80)) {
        return 1;
    }

    if (display_sprite(start_option, 300, 300)) {
        return 1;
    }

    if (display_sprite(exit_option, 300, 420)) {
        return 1;
    }

    if (display_sprite(record_option, 735, 530)) {
        return 1;
    }

    return 0;
}

void (menu_control_kbc)(uint8_t *bytes) {
    switch (bytes[0]) {
        case ESC_BREAKCODE:
            mode = EXIT_MODE;
            break;
        case ONE_BREAKCODE:
            mode = GAME_MODE;
            break;
        case TWO_BREAKCODE:
            mode = RECORD_MODE;
            break;
    
        default:
            break;
    }
}

void (menu_control_mouse)(struct packet *pp, int x, int y) {
    if (x >= 300 && x < 500 && y >= 300 && y < 390) {
        if (!menu_clicked) {
            if (pp->lb) {
                menu_clicked = true;
            }
        }
        if (menu_clicked) {
            if (!pp->lb) {
                mode = GAME_MODE;
                menu_clicked = false;
            }
        }
    } else if (x >= 300 && x < 500 && y >= 420 && y < 510) {
        if (!menu_clicked) {
            if (pp->lb) {
                menu_clicked = true;
            }
        }
        if (menu_clicked) {
            if (!pp->lb) {
                mode = EXIT_MODE;
                menu_clicked = false;
            }
        }
    } else if (x >= 735 && x < 795 && y >= 530 && y < 590) {
        if (!menu_clicked) {
            if (pp->lb) {
                menu_clicked = true;
            }
        }
        if (menu_clicked) {
            if (!pp->lb) {
                mode = RECORD_MODE;
                menu_clicked = false;
            }
        }
    } else {
        menu_clicked = false;
    }
}


int (menu_display_mouse)(int x, int y) {
    if ((x >= 300 && x < 500 && y >= 300 && y < 390) || (x >= 300 && x < 500 && y >= 420 && y < 510) || (x >= 735 && x < 795 && y >= 530 && y < 590)) {
        if (display_sprite(select_mouse_cursor, x, y)) {
            return 1;
        }
    } else {
        if (display_sprite(mouse_cursor, x, y)) {
            return 1;
        }
    }   

    return 0;
}
