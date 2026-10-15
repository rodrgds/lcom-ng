#include "menu_viewer.h"

int(draw_main_menu)(){
    if (draw_sprite(logo)) return 1;
    if (draw_sprite(instructions)) return 1;
    if (draw_sprite(levels)) return 1;
    return 0;
}

int(draw_menu_cursor)(){
    if (select_item(levels)) {
        cursor->i = levels->i = 1;
        instructions->i = 0;
    }
    else if (select_item(instructions)) {
        cursor->i = instructions->i = 1;
        levels->i = 0;
    }
    else {
        cursor->i = instructions->i = levels->i = 0;
    }
    return draw_sprite(cursor);
}
