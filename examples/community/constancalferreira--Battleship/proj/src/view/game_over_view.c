#include "game_over_view.h"

extern uint8_t *current_buffer;
extern uint8_t *game_over_buffer;
extern unsigned int frame_size;
extern vbe_mode_info_t mode_info;

extern Sprite *game_over;
extern Sprite *menu_option;
extern Sprite *exit_option;
extern Sprite *menu_shipR;

extern Sprite *player1;
extern Sprite *player2;
extern Sprite *wins;

extern PlayerTurn current_player;


void draw_game_over_background(uint8_t *buffer) {

    fill_screen(TEAL, buffer);

    draw_game_over_winner(buffer);

    if (game_over != NULL) {
        draw_sprite(game_over, (mode_info.XResolution - game_over->width) / 2, 50, buffer);
    }
    
    //draw_game_over_options(buffer);
}

void draw_game_over_options(uint8_t *buffer) {
    GameOverOption selected = get_game_over_option();
    
    if (menu_option != NULL) {
        if (selected == GO_MENU) {
            draw_sprite_recolor(menu_option, OPTIONS_X, OPTION_MENU_Y, HOVER_COLOR, buffer);
        } else {
            draw_sprite(menu_option, OPTIONS_X, OPTION_MENU_Y, buffer);
        }
    }
    
    if (exit_option != NULL) {
        if (selected == GO_EXIT) {
            draw_sprite_recolor(exit_option, OPTIONS_X, OPTION_EXIT_Y, HOVER_COLOR, buffer);
        } else {
            draw_sprite(exit_option, OPTIONS_X, OPTION_EXIT_Y, buffer);
        }
    }

    if (menu_shipR != NULL) {
        if (selected == GO_MENU) {
            int ship_y = OPTION_MENU_Y + (menu_option->height / 2) - (menu_shipR->height / 2);
            draw_sprite(menu_shipR, OPTIONS_X - menu_shipR->width, ship_y, buffer);
        } else {
            int ship_y = OPTION_EXIT_Y + (exit_option->height / 2) - (menu_shipR->height / 2);
            draw_sprite(menu_shipR, OPTIONS_X - menu_shipR->width, ship_y, buffer);
        }
    }
}


void draw_game_over_winner(uint8_t *buffer) {  
  if (current_player == PLAYER_1) {
    draw_sprite_recolor(player1, 200, WINNER_TEXT_Y, BRIGHT_ORANGE, buffer);
  } else if (current_player == PLAYER_2) {
    draw_sprite_recolor(player2, 200, WINNER_TEXT_Y, BRIGHT_ORANGE, buffer);
  }   

  draw_sprite_recolor(wins, 200 + player1->width + 20, WINNER_TEXT_Y, BRIGHT_ORANGE, buffer);
}



void draw_game_over_screen() {
    memcpy(current_buffer, game_over_buffer, frame_size);
    
    draw_game_over_options(current_buffer);
}
