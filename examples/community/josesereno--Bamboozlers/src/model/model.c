#include "model.h"

void (create_sprites)(){
    create_mouse_sprites();
    create_menu_sprites();
    create_game_sprites();
    create_instructions_sprites();
    create_game_over_sprites();
    create_level_select_sprites();
}

void (destroy_sprites)(){
    destroy_menu_sprites();
    destroy_game_sprites();
    destroy_instructions_sprites();
    destroy_game_over_sprites();
    destroy_level_select_sprites();
    destroy_sprite(cursor);
}

void (create_mouse_sprites)(){
    xpm_map_t pixmaps[] = {(xpm_map_t)cursor_xpm, (xpm_map_t)hand_xpm, (xpm_map_t) dragging_xpm};
    cursor = create_sprite_xpm(pixmaps, 3, 300, 400);
}
