#include "level_select.h"

void (create_level_select_sprites)(){
    xpm_map_t level_one_pixmaps[] = {(xpm_map_t)level1_xpm, (xpm_map_t)level1_sel_xpm};
    xpm_map_t level_two_pixmaps[] = {(xpm_map_t)level2_deny_xpm, (xpm_map_t)level2_sel_xpm, (xpm_map_t)level2_xpm};
    xpm_map_t level_three_pixmaps[] = {(xpm_map_t)level3_deny_xpm, (xpm_map_t)level3_sel_xpm, (xpm_map_t)level3_xpm};

    level_one = create_sprite_xpm(level_one_pixmaps, 2, 237, 100);
    level_two = create_sprite_xpm(level_two_pixmaps, 3, 237, 250);
    level_three = create_sprite_xpm(level_three_pixmaps, 3, 237, 400);
}

void (destroy_level_select_sprites)(){
    destroy_sprite(level_one);
    destroy_sprite(level_two);
    destroy_sprite(level_three);
}
