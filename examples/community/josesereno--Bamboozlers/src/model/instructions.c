#include "instructions.h"

void (create_instructions_sprites)(){
    xpm_map_t instructions_text_1_pixmaps[] = {(xpm_map_t)instructions_text_1_xpm};
    xpm_map_t instructions_text_2_pixmaps[] = {(xpm_map_t)instructions_text_2_xpm};
    xpm_map_t instructions_text_3_pixmaps[] = {(xpm_map_t)instructions_text_3_xpm};
    xpm_map_t instructions_text_4_pixmaps[] = {(xpm_map_t)instructions_text_4_xpm};
    xpm_map_t instructions_text_5_pixmaps[] = {(xpm_map_t)instructions_text_5_xpm};
    xpm_map_t instructions_text_6_pixmaps[] = {(xpm_map_t)instructions_text_6_xpm};
    instructions_text_1 = create_sprite_xpm(instructions_text_1_pixmaps, 1, 115, 100);
    instructions_text_2 = create_sprite_xpm(instructions_text_2_pixmaps, 1, 143, 175);
    instructions_text_3 = create_sprite_xpm(instructions_text_3_pixmaps, 1, 90, 250);
    instructions_text_4 = create_sprite_xpm(instructions_text_4_pixmaps, 1, 50, 325);
    instructions_text_5 = create_sprite_xpm(instructions_text_5_pixmaps, 1, 47, 400);
    instructions_text_6 = create_sprite_xpm(instructions_text_6_pixmaps, 1, 212, 475);
}

void (destroy_instructions_sprites)(){
    destroy_sprite(instructions_text_1);
    destroy_sprite(instructions_text_2);
    destroy_sprite(instructions_text_3);
    destroy_sprite(instructions_text_4);
    destroy_sprite(instructions_text_5);
    destroy_sprite(instructions_text_6);   
}
