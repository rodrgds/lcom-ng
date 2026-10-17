#include "allocations_manager.h"

extern mouse_t mouse_structure;
extern struct gamecontext context;

void manage_game_allocations(){
    allocate_mouse();
    allocate_screens();
    allocate_game_elements();
    allocate_players();
    allocate_buttons();
    allocate_numbers();
}

void manage_game_deletes(){
    delete_screens();
    delete_game_elements();
    delete_players();
    delete_buttons();
    delete_numbers();
}

void manage_start_states(){
    mouse_structure.xpos = 50;
    mouse_structure.ypos = 50;
    context.gamestatus = ON;
    context.gamestate = START_MENU; 

    context.levers_countdown = 0;
    context.startLeversCountdown = 0;

    context.map_countdown = 0;
    context.startMapCountdown = 1;
    
    reset_barrier();

    set_sprite_xpos(&context.sprites[SPRITE_BOY_Idx], 25);
    set_sprite_ypos(&context.sprites[SPRITE_BOY_Idx], 515);
    set_sprite_xpos(&context.sprites[SPRITE_GIRL_Idx], 25);
    set_sprite_ypos(&context.sprites[SPRITE_GIRL_Idx], 515);
} 
