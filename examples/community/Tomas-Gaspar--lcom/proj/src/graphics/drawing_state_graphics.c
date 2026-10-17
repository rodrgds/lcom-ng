#include <lcom/lcf.h>

#include "drawing_state_graphics.h"
#include "paint_window.h"
#include "../cursor.h"
#include "../getters.h"
#include "../img/cursor.xpm"

static drawing_movement move = NL_DRAW;

button (click_on_button)(Cursor* cursor) {
    if (is_on_limits(cursor, 419, 436, 70, 86)) return B_RED;
    if (is_on_limits(cursor, 441, 458, 70, 86)) return B_YELLOW;
    if (is_on_limits(cursor, 463, 480, 70, 86)) return B_BLUE;
    if (is_on_limits(cursor, 485, 502, 70, 86)) return B_GREEN;
    if (is_on_limits(cursor, 507, 524, 70, 86)) return B_BLACK;
    if (is_on_limits(cursor, 529, 546, 70, 86)) return B_WHITE;
    if (is_on_limits(cursor, 380, 403, 66, 89)) return B_CLEAR;
    if (is_on_limits(cursor, 335, 357, 67, 88)) return B_LARGE;
    if (is_on_limits(cursor, 295, 319, 67, 88)) return B_MEDIUM;
    if (is_on_limits(cursor, 258, 279, 66, 87)) return B_SMALL;
    return NO_BUTTON;
}

void (drawing_gesture_handler)(Cursor* cursor, bool lb, int16_t delta_x, int16_t delta_y) {
    if (lb) {
        if (is_on_limits(cursor, 4, 795, 114, 595)) {
            move = L_DRAW;
        } else if (click_on_button(cursor) != NO_BUTTON) {
            move = L_BUTTON;
        }
        else {
            move = L_OTHER;
        }
    } else {
        if (is_on_limits(cursor, 4, 795, 114, 595)) {
            move = NL_DRAW;
        } else if (click_on_button(cursor) != NO_BUTTON) {
            move = NL_BUTTON;
        }
        else {
            move = NL_OTHER;
        }
    }
  
    if (move == L_DRAW || move == NL_DRAW) {
        cursor->xpmCode = 2;
        if (get_color_theme() == LIGHTMODE) cursor->cursorMap = precise_day;
        else cursor->cursorMap = precise_night;
    } else if (move == L_BUTTON || move == NL_BUTTON) {
        cursor->xpmCode = 1;
        if (get_color_theme() == LIGHTMODE) cursor->cursorMap = hand_day;
        else cursor->cursorMap = hand_night;
    } else {
        cursor->xpmCode = 0;
        if (get_color_theme() == LIGHTMODE) cursor->cursorMap = cursor_day;
        else cursor->cursorMap = cursor_night;
    }
}

void (drawing_change_state)(drawing_state* state) {
    switch (*state) {
    case INIT_DRAWING:
        if (move == L_DRAW) {
            *state = DRAW;
        } else if (move == L_BUTTON) {
            *state = BUTTON;
        } else if (move == L_OTHER) {
            *state = IDLE_OTHER;
        }
        break;
    case IDLE_OTHER:
        if (move == NL_DRAW || move == NL_BUTTON || move == NL_OTHER) {
            *state = INIT_DRAWING;
        }
        break;
    case IDLE_DRAW:
        if (move == L_OTHER || move == L_BUTTON) {
            *state = IDLE_DRAW;
        } else if (move == NL_DRAW || move == NL_BUTTON || move == NL_OTHER) {
            *state = INIT_DRAWING;
        } else {
            *state = DRAW;
        }
        break;
    case DRAW:
        if (move == L_DRAW) {
            *state = DRAW;
        } else if (move == L_OTHER || move == L_BUTTON) {
            *state = IDLE_DRAW;
        } else if (move == NL_DRAW || move == NL_BUTTON || move == NL_OTHER) {
            *state = INIT_DRAWING;
        }
        break;
    case BUTTON:
        if (move == L_BUTTON) {
            *state = BUTTON;
        } else if (move == NL_BUTTON) {
            *state = B_PAINT;
        } else {
            *state = IDLE_OTHER;
        }
        break;
    case B_PAINT:
        break;
    default:
        break;
    }
}

void (drawing_interpret_state)(Cursor* cursor, drawing_state* state) {
    if (*state == DRAW) draw_and_move_cursor(cursor, get_packet().delta_x, get_packet().delta_y, cursor->pencilColor);
    else if (*state == B_PAINT) {
        switch (click_on_button(cursor)) {
        case B_RED:
            cursor->pencilColor = 0xff0000;
            break;
        case B_YELLOW:
            cursor->pencilColor = 0xebff00;
            break;
        case B_BLUE:
            cursor->pencilColor = 0x00f0ff;
            break;
        case B_GREEN:
            cursor->pencilColor = 0x05ff00;
            break;
        case B_BLACK:
            cursor->pencilColor = 0x000000;
            break;
        case B_WHITE:
            cursor->pencilColor = 0xffffff;
            break;
        case B_CLEAR:
            vg_draw_rectangle(4, 114, 792, 482, get_color_theme() == LIGHTMODE ? 0xffffff : 0x121212);
            break;
        case B_SMALL:
            cursor->pencilSize = 2;
            break;
        case B_MEDIUM:
            cursor->pencilSize = 3;
            break;
        case B_LARGE:
            cursor->pencilSize = 4;
            break;
        default:
            break;
        }
        *state = IDLE_OTHER;
    } else move_cursor(cursor, get_packet().delta_x, get_packet().delta_y);
}
