#include "level_select_viewer.h"

extern Level_state level_state;

int(draw_level_select_menu)(){
    if (draw_sprite(level_one)) return 1;
    if (draw_sprite(level_two)) return 1;
    if (draw_sprite(level_three)) return 1;
    return 0;
}

int(draw_level_cursor)(){
    if (select_item(level_one)) {
        level_one->i = cursor->i = 1;
        level_two->i = (level_state == LEVEL_ONE) ? 0 : 2;
        level_three->i = (level_state == LEVEL_THREE) ? 2 : 0;
    }
    else if (select_item(level_two) && level_state != LEVEL_ONE) {
        level_two->i = cursor->i = 1;
        level_one->i = 0;
        level_three->i = (level_state == LEVEL_TWO) ? 0 : 2;
    }
    else if (select_item(level_three) && level_state == LEVEL_THREE) {
        level_three->i = cursor->i = 1;
        level_one->i = 0;
        level_two->i = 2;
    }
    else {
        cursor->i = level_one->i = 0;
        level_two->i = (level_state != LEVEL_ONE) ? 2 : 0;
        level_three->i = (level_state == LEVEL_THREE) ? 2 : 0;
    }
    return draw_sprite(cursor);
}
