#include "kbd_event_handler.h"
#include "draw.h"
#include "interrupt_manager.h"

bool changed;
Player *player1;

void handle_kbd_event(uint8_t scan_code[2]) {
    switch (state) {
        case MAIN_MENU:
            break;
        case PLAYING:
            handle_kbd_game_event(scan_code);
            break;
        case INSTRUCTIONS:
            handle_kbd_instructions_event(scan_code);
            break;
        case EXIT:
            break;
    }
}

void handle_kbd_instructions_event(uint8_t scan_code[2]) {
    if (scan_code[0] == ESC_BREAK) {
        state = MAIN_MENU;
        changed = true;
    }
}

void handle_kbd_game_event(uint8_t scan_code[2]) {
    bool two_byte = (scan_code[0] == TWO_BYTE_SC);
    uint8_t key_code = two_byte ? scan_code[1] : scan_code[0];

    if (two_byte) {
        switch (key_code) {
            case UP_ARROW:
                move_up(player1);
                break;
            case DOWN_ARROW:
                move_down(player1);
                break;
            default: break;
        }
    } else if (scan_code[0] == ESC_BREAK) {
        state = EXIT;
    }
}

void handle_kbd_menu_event(uint8_t scan_code[2]) {
    if (scan_code[0] == ESC_BREAK) {
        state = EXIT;
    }
}

