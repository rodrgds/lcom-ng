#include <math.h>

#include "game_state.h"
#include "draw.h"
#include "interrupt_manager.h"

game_state state = MAIN_MENU;
bool game_over = false;
Position mouse_pos = {210, 330};


uint64_t delta_time;

bool changed;

void play(void) {
    state = PLAYING;
}

void exit_game(void) {
    state = EXIT;
}

void (instructions)(void) {
    state = INSTRUCTIONS;
    changed = true;
}


void update_mouse(int16_t x, int16_t y) {
    int32_t new_x = mouse_pos.x + x;
    int32_t new_y = mouse_pos.y - y;

    mouse_pos.x = new_x > 0 ? (new_x > x_max ? x_max : new_x) : 0;
    mouse_pos.y = new_y > 0 ? (new_y > y_max ? y_max : new_y) : 0;
}


void (move_up)(Player *player) {
     if (player->y_pos > 0) {
        player->y_pos = fmax((int)(player->y_pos - 50), 10)   /* 16.0f / delta_time */;
    }
}

void move_down(Player *player) {
    if (player->y_pos + player->height < y_max - 10) {
        player->y_pos = fmin(player->y_pos + 50, y_max - player->height - 10) /* (delta_time / 1000.0f) */;     
    }
}

