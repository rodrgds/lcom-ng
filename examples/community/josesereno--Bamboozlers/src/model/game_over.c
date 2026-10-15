#include "game_over.h"


void (create_game_over_sprites)(){
    xpm_map_t game_over_pixmaps[] = {(xpm_map_t)game_over_xpm};
    xpm_map_t exit_pixmaps[] = {(xpm_map_t)exit_xpm,(xpm_map_t)exit_sel_xpm};
    xpm_map_t play_again_pixmaps[] = {(xpm_map_t)play_again_xpm,(xpm_map_t)play_again_sel_xpm};
    
    game_over = create_sprite_xpm(game_over_pixmaps, 1, 132, 170);
    play_again = create_sprite_xpm(play_again_pixmaps, 2, 100, 350);
    game_exit = create_sprite_xpm(exit_pixmaps, 2, 450, 350);
}

void (destroy_game_over_sprites)(){
    destroy_sprite(game_over);
    destroy_sprite(play_again);
    destroy_sprite(game_exit);
}
