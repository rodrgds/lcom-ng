#include "gamemode_menu.h"

extern uint8_t scancode;

extern GameMode gameMode;
extern int gameState;
int game_menu_selected_option = -1; // default to no selection

void gamemode_menu_init() {
    game_menu_selected_option = -1;
    draw_gamemode_menu_frame();
}

void keyboard_game_menu() {
    switch (scancode) {
        case ENTER_KEY: 
            switch (game_menu_selected_option) {
                case 0: // guess mode
                    change_mode(GUESS_MODE);
                    break;
                case 1: // react mode
                    change_mode(REACT_MODE);
                    break;
                case 2: // instructions
                    change_mode(INSTRUCTIONS_SCREEN);
                    break;
                case 3: // go back to player menu
                    gameMode = PLAYER_MENU;
                    break;
            }
            break;
    }
}

void mouse_game_menu() {
    update_mouse_state_menus();
    
    if (cursor_x >= 268 && cursor_x <= 268 + 200 && cursor_y >= 284 && cursor_y <= 284 + 62 * 4) {
        int option = (cursor_y - 284) / 62;
        if (option >= 0 && option < 4) {
            game_menu_selected_option = option;
        }
    } else {
        game_menu_selected_option = -1; // no selection
    }

    if (isLeftButtonPressed()) {
        switch (game_menu_selected_option) {
            case 0: // guess mode
                change_mode(GUESS_MODE);
                break;
            case 1: // react mode
                change_mode(REACT_MODE);
                break;
            case 2: // instructions
                change_mode(INSTRUCTIONS_SCREEN);
                break;
            case 3: // go back to player menu
                gameMode = PLAYER_MENU;
                break;
        }
    }
}

void gamemode_menu_timer_ih() {
    if (gameMode != GAMEMODE_MENU) return;
    draw_gamemode_menu_frame();
}
