#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"

static xpm_row_t const cursor_xpm[] = {
"12 18 2 1",
"  c None",
". c #FFFFFF",
".           ",
"..          ",
"...         ",
"....        ",
".....       ",
"......      ",
".......     ",
"........    ",
".........   ",
"..........  ",
"........... ",
".......     ",
".... ..     ",
"...   ..    ",
"..    ..    ",
".      ..   ",
"       ..   ",
"        ..  "
};

static AnimSprite cursor_sprite;

int load_cursor_sprite() {
    return sprite_load_single(&cursor_sprite, (xpm_map_t) cursor_xpm);
}

void free_cursor_sprite() {
    sprite_free_frames(&cursor_sprite);
}

AnimSprite* get_cursor_sprite() {
    return &cursor_sprite;
}
