#include "name_input.h"
#include "font.h"
#include "v_card.h"
#include <string.h>

// current player name buffer 
static char player_name[MAX_NAME_LEN + 1];
static int name_len = 0;

// scancode to ASCII lookup table (makecodes only)
// maps keyboard scancodes to uppercase letters 
static char scancode_to_char(uint8_t sc) {
    switch (sc) {
        case 0x10: return 'Q';
        case 0x11: return 'W';
        case 0x12: return 'E';
        case 0x13: return 'R';
        case 0x14: return 'T';
        case 0x15: return 'Y';
        case 0x16: return 'U';
        case 0x17: return 'I';
        case 0x18: return 'O';
        case 0x19: return 'P';
        case 0x1E: return 'A';
        case 0x1F: return 'S';
        case 0x20: return 'D';
        case 0x21: return 'F';
        case 0x22: return 'G';
        case 0x23: return 'H';
        case 0x24: return 'J';
        case 0x25: return 'K';
        case 0x26: return 'L';
        case 0x2C: return 'Z';
        case 0x2D: return 'X';
        case 0x2E: return 'C';
        case 0x2F: return 'V';
        case 0x30: return 'B';
        case 0x31: return 'N';
        case 0x32: return 'M';
        default:   return 0;
    }
}

void name_input_init() {
    memset(player_name, 0, sizeof(player_name));
    name_len = 0;
}

bool name_input_on_key(uint8_t scancode) {
    // ignore breakcodes (bit 7 set) 
    if (scancode & 0x80) return false;

    // Enter key (scancode 0x1C) - confirm name/
    if (scancode == 0x1C) {
        if (name_len > 0) return true;  // name confirmed 
        return false;
    }

    // Backspace (scancode 0x0E) - delete last char
    if (scancode == 0x0E) {
        if (name_len > 0) {
            name_len--;
            player_name[name_len] = '\0';
        }
        return false;
    }

    // try to convert scancode to a letter 
    char c = scancode_to_char(scancode);
    if (c != 0 && name_len < MAX_NAME_LEN) {
        player_name[name_len] = c;
        name_len++;
        player_name[name_len] = '\0';
    }

    return false;
}

void name_input_render() {
    // clear screen with dark background 
    vg_draw_rectangle(0, 0, 800, 600, 0x1A1A2E);

    // title 
    font_draw_string("ENTER YOUR NAME", 310, 180, 0x00FF00);

    // draw input box /
    vg_draw_rectangle(200, 280, 400, 50, 0x333355);

    //mdraw current name
    if (name_len > 0) {
        font_draw_string(player_name, 220, 295, 0xFFFFFF);
    }

    // blinking cursor 
    int cursor_x = 220 + name_len * 12;
    vg_draw_rectangle(cursor_x, 295, 2, 20, 0xFFFFFF);

    // instructions 
    font_draw_string("TYPE YOUR NAME AND PRESS ENTER", 220, 380, 0x888888);
    font_draw_string("BACKSPACE TO DELETE", 286, 410, 0x888888);
}

const char* name_input_get_name() {
    return player_name;
}
