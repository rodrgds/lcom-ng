#include "model/modes.h"

GameMode gameMode = PLAYER_MENU;
GameMode previousGameMode = NO;
Winner winner = SOMEONE;
PlayerMode playerMode = ONE_PLAYER;
TimeDiff timeDiff = {0, 0};

HandlerFn mouse_ih_array[6] = {
    mouse_player_menu, 
    mouse_game_menu, 
    react_mode_mouse_ih, 
    guess_mode_mouse_ih, 
    mouse_instructions, 
    mouse_game_over_menu
};

HandlerFn keyboard_ih_array[6] = {
    keyboard_player_menu,
    keyboard_game_menu,
    react_mode_keyboard_ih,
    guess_mode_keyboard_ih,
    keyboard_instructions,
    keyboard_game_over_menu
};

HandlerFn timer_ih_array[6] = {
    player_menu_timer_ih, 
    gamemode_menu_timer_ih,
    react_mode_timer_ih, 
    guess_mode_timer_ih, 
    instructions_timer_ih, 
    game_over_timer_ih
};

HandlerFn mode_init[6] = {
    player_menu_init,
    gamemode_menu_init,
    react_mode_init,
    guess_mode_init,
    instructions_init,
    game_over_init
};

void change_mode(GameMode new_mode) {
    if (new_mode == gameMode) return;
    previousGameMode = gameMode;
    gameMode = new_mode;

    if(mode_init[gameMode] != NULL) {
        mode_init[gameMode]();
    }
    return;
}
