#include "menu.h"

void (create_menu_sprites)(){
    xpm_map_t instructions_pixmaps[] = {(xpm_map_t)instructions_xpm, (xpm_map_t)instructions_sel_xpm};
    xpm_map_t levels_pixmaps[] = {(xpm_map_t)levels_xpm, (xpm_map_t)level_sel_xpm};
    xpm_map_t logo_pixmaps[] = {(xpm_map_t)logo_xpm};


    instructions = create_sprite_xpm(instructions_pixmaps,2, 405,430);
    levels = create_sprite_xpm(levels_pixmaps, 2, 120, 430);
    logo = create_sprite_xpm(logo_pixmaps, 1, 150, 50);
}

void (destroy_menu_sprites)(){
    destroy_sprite(instructions);
    destroy_sprite(levels);
    destroy_sprite(logo);
}
