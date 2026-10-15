#include "player_menu.h"

extern uint8_t scancode;

extern GameMode gameMode;
extern GameMode previousGameMode;
extern GameState gameState;
extern PlayerMode playerMode;

int player_menu_selected_option = -1; // default to no selection
int num_players;

void player_menu_init() {
    num_players = 1;
    player_menu_selected_option = -1;
    draw_player_menu_frame();
}


void keyboard_player_menu() {
    switch (scancode) {
        case ENTER_KEY: 
            switch (player_menu_selected_option) {
                case 0: // 1 player mode
                    playerMode = ONE_PLAYER;
                    change_mode(GAMEMODE_MENU);
                    break;
                case 1: // 2 players mode
                    playerMode = TWO_PLAYERS;
                    change_mode(GAMEMODE_MENU);
                    break;
                case 2: // instructions
                    change_mode(INSTRUCTIONS_SCREEN);
                    break;
                case 3: // exit game
                    gameState = EXIT;
                    break;
            }
            break;
    }
}

void mouse_player_menu() {
    update_mouse_state_menus();
    
    if (cursor_x >= 268 && cursor_x <= 268 + 200 && cursor_y >= 284 && cursor_y <= 284 + 62 * 4) {
        int option = (cursor_y - 284) / 62;
        if (option >= 0 && option < 4) {
            player_menu_selected_option = option;
        }
    } else {
        player_menu_selected_option = -1; // no selection
    }

    if (isLeftButtonPressed()) {
        switch (player_menu_selected_option) {
            case 0: // 1 player mode
                playerMode = ONE_PLAYER;
                change_mode(GAMEMODE_MENU);
                break;
            case 1: // 2 players mode
                playerMode = TWO_PLAYERS;
                change_mode(GAMEMODE_MENU);
                break;
            case 2: // instructions
                change_mode(INSTRUCTIONS_SCREEN);
                break;
            case 3: // exit game
                gameState = EXIT;
                break;
        }
    }
}

void player_menu_timer_ih() {
    if (gameMode != PLAYER_MENU) return;
    draw_player_menu_frame();
}
