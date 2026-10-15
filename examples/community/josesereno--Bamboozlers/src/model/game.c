#include "game.h"

void (create_game_sprites)(){
    xpm_map_t panda_pixmaps[] = {
        (xpm_map_t)panda1_xpm,
        (xpm_map_t)panda2_xpm,
        (xpm_map_t)panda3_xpm,
        (xpm_map_t)panda4_xpm,
        (xpm_map_t)panda5_xpm,
        (xpm_map_t)panda6_xpm,
        (xpm_map_t)panda7_xpm,
        (xpm_map_t)panda8_xpm,
    };
  
    xpm_map_t little_plank_pixmaps[] = {
        (xpm_map_t)little_plank_xpm, 
        (xpm_map_t)little_plank_r1_xpm, 
        (xpm_map_t)little_plank_r2_xpm, 
        (xpm_map_t)little_plank_r3_xpm, 
        (xpm_map_t)little_plank_r4_xpm, 
        (xpm_map_t)little_plank_r5_xpm, 
        (xpm_map_t)little_plank_r6_xpm, 
        (xpm_map_t)little_plank_r7_xpm
    };

    xpm_map_t big_plank_pixmaps[] = {
        (xpm_map_t)big_plank_xpm, 
        (xpm_map_t)big_plank_r1_xpm, 
        (xpm_map_t)big_plank_r2_xpm, 
        (xpm_map_t)big_plank_r3_xpm, 
        (xpm_map_t)big_plank_r4_xpm, 
        (xpm_map_t)big_plank_r5_xpm, 
        (xpm_map_t)big_plank_r6_xpm, 
        (xpm_map_t)big_plank_r7_xpm
    };
    
    xpm_map_t little_block_pixmaps[] = {(xpm_map_t)little_block_xpm};
    xpm_map_t big_block_pixmaps[] = {(xpm_map_t)big_block_xpm};
    xpm_map_t background_pixmaps[] = {(xpm_map_t)background_xpm, (xpm_map_t)background_dark_xpm};
    xpm_map_t home_pixmaps[] = {(xpm_map_t)home_xpm};
    xpm_map_t bamboo_pixmaps[] = {(xpm_map_t)bamboo_xpm};

    xpm_map_t maps_pixmaps[] = {
        (xpm_map_t)grama_xpm, 
        (xpm_map_t)rock_xpm, 
        (xpm_map_t)sand_xpm
    };
    
    xpm_map_t lava_pixmaps[] = {
        (xpm_map_t)lava_xpm, 
        (xpm_map_t)lava2_xpm, 
        (xpm_map_t)lava3_xpm
    };
    
    xpm_map_t timer_pixmaps[] = {
        (xpm_map_t)number_0_xpm, 
        (xpm_map_t)number_1_xpm, 
        (xpm_map_t)number_2_xpm, 
        (xpm_map_t)number_3_xpm, 
        (xpm_map_t)number_4_xpm, 
        (xpm_map_t)number_5_xpm, 
        (xpm_map_t)number_6_xpm, 
        (xpm_map_t)number_7_xpm, 
        (xpm_map_t)number_8_xpm, 
        (xpm_map_t)number_9_xpm
    };


    background = create_sprite_xpm(background_pixmaps, 2, 25, 25);
    
    lava = create_sprite_xpm(lava_pixmaps, 3, 25, 25);
    map = create_sprite_xpm(maps_pixmaps, 3, 25, 25);
    home = create_sprite_xpm(home_pixmaps, 1, 680, 250);
    panda = create_sprite_xpm(panda_pixmaps, 8, 40, 100);
    little_block = create_sprite_xpm(little_block_pixmaps, 1, 360, 520);
    big_block = create_sprite_xpm(big_block_pixmaps, 1, 410, 510);
    little_plank = create_sprite_xpm(little_plank_pixmaps, 8, 480, 535);
    big_plank = create_sprite_xpm(big_plank_pixmaps, 8, 585, 535);
    bamboo = create_sprite_xpm(bamboo_pixmaps, 1, 360, 370);

    for (int i = 0; i < 4 ; i++){
        timer[i] = create_sprite_xpm(timer_pixmaps, 10, 40 + i*30, 525);
    }
  
}

void (destroy_game_sprites)(){
    destroy_sprite(background);
    destroy_sprite(lava);
    destroy_sprite(map);
    destroy_sprite(panda);
    destroy_sprite(home);
    destroy_sprite(little_block);
    destroy_sprite(big_block);
    destroy_sprite(little_plank);
    destroy_sprite(big_plank);
    destroy_sprite(bamboo);
    for (int i = 0; i < 4; i++)
        destroy_sprite(timer[i]);
}
