#include "cursor.h"

void cursor_get_position(uint16_t* x_pos, uint16_t* y_pos) {
    *x_pos = x;
    *y_pos = y;
}

void cursor_move(int x_delta, int y_delta) {
    x += x_delta;
    y -= y_delta;

    if (x > CURSOR_X_LIMIT) x = CURSOR_X_LIMIT; if (y > CURSOR_Y_LIMIT) y = CURSOR_Y_LIMIT;
}
