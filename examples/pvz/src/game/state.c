#include "state.h"

#include "constants.h"

static GameState current_state = STATE_MAIN_MENU;
static GameInfo game_info;

GameState get_game_state(void) {
    return current_state;
}

void set_game_state(GameState new_state) {
    current_state = new_state;
}

const GameInfo *get_game_info(void) {
    return &game_info;
}

void add_sun(int amount) {
    if (amount > 0) {
        game_info.sun_count += amount;
    }
}

bool spend_sun(int amount) {
    if (amount < 0 || game_info.sun_count < amount) {
        return false;
    }

    game_info.sun_count -= amount;
    return true;
}

void set_game_wave(int wave) {
    game_info.wave = wave;
}

void init_game_info(void) {
    game_info.sun_count = INITIAL_SUN_COUNT;
    game_info.score = 0;
    game_info.zombie_spawn_rate = ZOMBIE_SPAWN_X;
    game_info.wave = 1;
}
