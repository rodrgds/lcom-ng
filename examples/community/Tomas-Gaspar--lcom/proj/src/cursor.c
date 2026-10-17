#include <lcom/lcf.h>

#include "cursor.h"
#include "controllers/video/macro_video.h"
#include "controllers/video/video_gr.h"

void move_cursor(Cursor* cursor, int16_t dx, int16_t dy) {
    uint16_t x = cursor->x, y = cursor->y;

    if (dx < 0 && abs(dx) > x)
        x = 0;
    else {
        x += dx;
        if (x > cursor->width - 1) x = cursor->width - 1;
    }

    if (dy > 0 && dy > y)
        y = 0;
    else {
        y -= dy;
        if (y > cursor->height - 1) y = cursor->height - 1;
    }
    
    cursor->x = x;
    cursor->y = y;
}

bool is_on_limits(Cursor* cursor, uint16_t xi, uint16_t xf, uint16_t yi, uint16_t yf) {
    return (cursor->x >= xi && cursor->x <= xf && cursor->y >= yi && cursor->y <= yf);
}

void draw_and_move_cursor(Cursor* cursor, int16_t dx, int16_t dy, uint32_t color) {
    uint16_t xi = cursor->x, yi = cursor->y;
    move_cursor(cursor, dx, dy);
    uint16_t x = cursor->x;
    uint16_t y = cursor->y;
    x = x < 4 ? 4 : x;
    x = x > 795 ? 795 : x;
    y = y < 114 ? 114 : y;
    y = y > 595 ? 595 : y;
    vg_draw_line(xi, yi, x, y, color, cursor->pencilSize);
}

Cursor create_cursor(uint16_t width, uint16_t height, uint32_t pencilColor, char** cursorMap, uint8_t xpmCode) {
    Cursor cursor = {
        .width = width,
        .height = height,
        .x = 300,
        .y = 400,
        .pencilColor = pencilColor,
        .cursorMap = cursorMap,
        .pencilSize = 2,
        .xpmCode = xpmCode
    };
    return cursor;
}
