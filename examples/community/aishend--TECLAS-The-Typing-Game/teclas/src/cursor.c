#include "cursor.h"
#include "../xpm/cursor/normal.xpm"
#include "sprite.h"
#include "video.h"
#include <lcom/lcf.h>

int16_t cursor_x;
int16_t cursor_y;

static Sprite *cursor_sprite;

void(cursor_init)() {
    cursor_x = screen_width / 2;
    cursor_y = screen_height / 2;

    cursor_sprite = sprite_load((xpm_map_t)normal);
}

void(cursor_move)(int16_t dx, int16_t dy) {
    cursor_x += dx;
    cursor_y -= dy;

    if (cursor_x < 0) cursor_x = 0;

    if (cursor_y < 0) cursor_y = 0;

    if (cursor_x >= (int16_t)screen_width) cursor_x = screen_width - 1;

    if (cursor_y >= (int16_t)screen_height) cursor_y = screen_height - 1;
}

void(cursor_draw)() { sprite_draw(cursor_sprite, cursor_x, cursor_y); }

void(cursor_destroy)() {
    sprite_destroy(cursor_sprite);
    cursor_sprite = NULL;
}
