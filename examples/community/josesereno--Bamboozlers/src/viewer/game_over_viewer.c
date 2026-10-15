#include "game_over_viewer.h"

int(draw_game_over_menu)(){

    if (draw_sprite(game_over)) return 1;
    if (draw_sprite(play_again)) return 1;
    if (draw_sprite(game_exit)) return 1;

    return 0;
}

int(draw_game_over_cursor)(){
    if (select_item(play_again)){
        cursor->i = play_again->i = 1;
        game_exit->i = 0;
    }
    else if (select_item(game_exit)){
        cursor->i = game_exit->i = 1;
        play_again->i = 0;
    }
    else{
        cursor->i = game_exit->i = play_again->i = 0;
    }
    return draw_sprite(cursor);
}
