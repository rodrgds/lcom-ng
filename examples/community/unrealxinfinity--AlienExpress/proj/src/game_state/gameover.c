#include "game_over.h"

action_t game_over_selection = AFK;

void set_selection_game_over(action_t action){
    game_over_selection = action;
}
action_t mouse_ih_game_over(){
    if(mouse_position(486, 400, 576, 415)){
        game_over_selection = START;
    }
    else if(mouse_position(506, 430, 551, 445)){
        game_over_selection = EXIT;
    }
    else{
        if(mouse_hover){
            mouse_hover = false;
            game_over_selection = AFK;
        }
    }
    if(mouse_packet.lb && mouse_hover) {
        mouse_hover = false;
        return game_over_selection;
    }
    return AFK;
}

action_t keyboard_ih_game_over(uint32_t scancode){
    if(scancode == MAKE_ENTER){
        return game_over_selection;
    }
    else if(scancode == MAKE_S){
        if(game_over_selection == AFK) game_over_selection = START;
        else if(game_over_selection == START) game_over_selection = EXIT;
    }
    else if(scancode == MAKE_W){
        if(game_over_selection == AFK)game_over_selection = START;
        else if(game_over_selection == EXIT) game_over_selection = START;
    }
    return AFK;
}

void draw_game_over(){
    counter_timer++;
    reset_movement();

    draw_game_over_tag();

    draw_game_over_words();

}
void draw_game_over_words(){
    int restart_type = 0;
    int menu_type = 0;
    switch(game_over_selection){
        case START:
            restart_type = 1;
            menu_type = 0;
            break;
        case EXIT:
            restart_type = 0;
            menu_type = 1;
            break;
        case AFK:
            break;
        default:
            break;
    }

    set_char(&r, 486, 400, restart_type);
    set_char(&e, 501, 400, restart_type);
    set_char(&s, 516, 400, restart_type);
    set_char(&t, 531, 400, restart_type);
    set_char(&a, 546, 400, restart_type);
    set_char(&r, 561, 400, restart_type);
    set_char(&t, 576, 400, restart_type);


    set_char(&m, 506, 430, menu_type);
    set_char(&e, 521, 430, menu_type);
    set_char(&n, 536, 430, menu_type);
    set_char(&u, 551, 430, menu_type);
}
void draw_game_over_tag(){
    set_char(&g, 475, 300, 1);
    set_char(&a, 490, 300, 1);
    set_char(&m, 505, 300, 1);
    set_char(&e, 520, 300, 1);

    set_char(&o, 550, 300, 1);
    set_char(&v, 565, 300, 1);
    set_char(&e, 580, 300, 1);
    set_char(&r, 595, 300, 1);
}
