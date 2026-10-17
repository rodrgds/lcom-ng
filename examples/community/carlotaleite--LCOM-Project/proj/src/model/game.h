#pragma once

typedef enum {
    GAME_RUNNING,
    GAME_OVER
} GameState;

void (game_init)();
void (update_timer_state)();
void (update_keyboard_state)(); 
