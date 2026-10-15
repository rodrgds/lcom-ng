#include "game_over_menu.h"

extern uint8_t scancode;
extern GameMode gameMode;
extern GameState gameState;
extern GameMode previousGameMode;

extern PlayerMode playerMode;
extern Winner winner;
extern TimeDiff timeDiff;

int game_over_menu_selected_option = -1; // default to no selection

void game_over_init() {
    game_over_menu_selected_option = -1;
    cursor_x = 40; 
    cursor_y = 40;
    draw_game_over_frame();
}

void keyboard_game_over_menu() {
    switch (scancode) {
        case ENTER_KEY: 
            switch (game_over_menu_selected_option) {
                case 0: // play again
                    change_mode(previousGameMode);
                    break;
                case 1: // go back
                    change_mode(GAMEMODE_MENU);
                    break;
            }
            break;
    }
}

void mouse_game_over_menu() {
    update_mouse_state_menus();
    
    if (cursor_x >= 268 && cursor_x <= 268 + 200 && cursor_y >= 284 + 62 * 2 && cursor_y <= 284 + 62 * 2 + 62 * 2) {
        int option = (cursor_y - (284 + 62 * 2)) / 62;
        if (option >= 0 && option < 2) {
            game_over_menu_selected_option = option;
        }
    } else {
        game_over_menu_selected_option = -1; // no selection
    }

    if (isLeftButtonPressed()) {
        switch (game_over_menu_selected_option) {
            case 0: // play again
                change_mode(previousGameMode);
                break;
            case 1: // go back
                change_mode(GAMEMODE_MENU);
                break;
        }
    }
}

void game_over_timer_ih() {
    if (gameMode != GAME_OVER) return;
    draw_game_over_frame();
}
