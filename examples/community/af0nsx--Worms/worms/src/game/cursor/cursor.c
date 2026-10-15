#include "cursor.h"

#include <stdlib.h>

#include "../../config.h"

struct Cursor {
    int16_t x;
    int16_t y;
    bool left_pressed;
    bool left_clicked;
};

Cursor *cursor_create(void) {
    Cursor *cursor = malloc(sizeof(Cursor));
    if (cursor == NULL) return NULL;

    cursor->x = SCREEN_W / 2;
    cursor->y = SCREEN_H / 2;
    cursor->left_pressed = false;
    cursor->left_clicked = false;

    return cursor;
}

void cursor_destroy(Cursor *cursor) {
    free(cursor);
}

void cursor_set_position(Cursor *cursor, int16_t x, int16_t y) {
    if (cursor == NULL) return;

    cursor->x = x;
    cursor->y = y;
}

void cursor_set_left_pressed(Cursor *cursor, bool pressed) {
    if (cursor == NULL) return;

    if (pressed && !cursor->left_pressed) cursor->left_clicked = true;
    cursor->left_pressed = pressed;
}

int16_t cursor_get_x(const Cursor *cursor) {
    if (cursor == NULL) return 0;
    return cursor->x;
}

int16_t cursor_get_y(const Cursor *cursor) {
    if (cursor == NULL) return 0;
    return cursor->y;
}

bool cursor_is_left_pressed(const Cursor *cursor) {
    if (cursor == NULL) return false;
    return cursor->left_pressed;
}

bool cursor_was_left_clicked(const Cursor *cursor) {
    if (cursor == NULL) return false;
    return cursor->left_clicked;
}

bool cursor_consume_left_click(Cursor *cursor) {
    if (cursor == NULL || !cursor->left_clicked) return false;

    cursor->left_clicked = false;
    return true;
}
