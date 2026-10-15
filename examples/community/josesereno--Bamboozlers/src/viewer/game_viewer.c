#include "game_viewer.h"

extern int minutes;
extern int seconds;
extern int counter;
extern bool darkMode;
extern Item_state item_state;

int(draw_game_menu)(){
    background->i = darkMode ? 1 : 0;
    if (draw_map()) return 1;
    if (draw_inventory()) return 1;
    if (draw_sprite(background)) return 1;
    if (draw_sprite(map)) return 1;
    if (draw_sprite(lava)) return 1;
    if (draw_sprite(bamboo)) return 1;
    if (draw_timer()) return 1;
    if (draw_sprite(home)) return 1;
    if (draw_sprite(panda)) return 1;
    if (draw_sprite(little_block)) return 1;
    if (draw_sprite(big_block)) return 1;
    if (draw_sprite(little_plank)) return 1;
    if (draw_sprite(big_plank)) return 1;
    return 0;
}

int(draw_map)(){
    if (vg_draw_rectangle(20, 20, MAP_WIDTH, BORDER_WIDTH, WHITE)) return 1;
    if (vg_draw_rectangle(20, 480, MAP_WIDTH, BORDER_WIDTH, WHITE)) return 1;
    if (vg_draw_rectangle(20, 20, BORDER_WIDTH, MAP_HEIGHT, WHITE)) return 1;
    if (vg_draw_rectangle(775, 20, BORDER_WIDTH, MAP_HEIGHT, WHITE)) return 1;
    return 0;
}

int(draw_inventory)(){
    if (draw_container(570, 500, 210)) return 1;
    if (draw_container(440, 500, 120)) return 1;
    if (draw_container(350, 500, 80)) return 1;
    if (draw_container(260, 500, 80)) return 1;
    if (draw_container(170, 500, 80)) return 1;

    return 0;
}

int(draw_container)(uint16_t x, uint16_t y, uint16_t width){
    if (vg_draw_rectangle(x, y, width, 80, GREY)) return 1;
    if (vg_draw_rectangle(x + 7, y + 7, 10, 2, LIGHT_GREY)) return 1;
    if (vg_draw_rectangle(x + 7, y + 7, 2, 10, LIGHT_GREY)) return 1;
    if (vg_draw_rectangle(x + width - 15, y + 7, 10, 2, LIGHT_GREY)) return 1;
    if (vg_draw_rectangle(x + width - 7, y + 7, 2, 10, LIGHT_GREY)) return 1;
    if (vg_draw_rectangle(x + 7, y + 71, 10, 2, LIGHT_GREY)) return 1;
    if (vg_draw_rectangle(x + 7, y + 63, 2, 10, LIGHT_GREY)) return 1;
    if (vg_draw_rectangle(x + width - 15, y + 71, 10, 2, LIGHT_GREY)) return 1;
    if (vg_draw_rectangle(x + width - 7, y + 63, 2, 10, LIGHT_GREY)) return 1;
    return 0;
}

int(draw_timer)(){

    if (counter % 30 == 0) {
        seconds--;
        if (seconds % 60 == 0) {
            minutes--;
            seconds = 60;
        }
    }
    timer[0]->i = 0;
    timer[1]->i = (seconds == 60) ? 1 : minutes % 10;
    timer[2]->i = (seconds == 60) ? 0 : seconds / 10;
    timer[3]->i = seconds % 10;

    if (vg_draw_rectangle(20, 500, 140, 80, GREY)) return 1;
    if (vg_draw_rectangle(87, 532, 4, 4, WHITE)) return 1;
    if (vg_draw_rectangle(87, 542, 4, 4, WHITE)) return 1;
    for (int i = 0; i < 4; i++) {
        if (draw_sprite(timer[i]))
            return 1;
    }
    return 0;
}

int draw_game_cursor(){
    if (item_state == INIT) {
        if ((select_item(little_block) && !is_in_map(little_block)) ||
            (select_item(little_plank) && !is_in_map(little_plank)) ||
            (select_item(big_block) && !is_in_map(big_block)) ||
            (select_item(big_plank) && !is_in_map(big_plank))) {
            cursor->i = 1;
        }
        else {
            cursor->i = 0;
        }
    }
    else if (item_state == DRAG) {
        cursor->i = 2;
    }
    return draw_sprite(cursor);
}
