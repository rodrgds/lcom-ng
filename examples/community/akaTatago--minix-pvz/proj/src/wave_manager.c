#include "wave_manager.h"
#include "levels.h"
#include "zombie.h"
#include "game_state.h"

static Wave level_waves[MAX_WAVES];
static int total_waves = 0;
static int current_wave = 0;
static int wave_timer = 0;

void wave_manager_init(int level_number) {
    if (level_number < 1 || level_number > (int)(sizeof(ALL_LEVELS) / sizeof(LevelConfig))) {
        level_number = 1;
    }

    LevelConfig config = ALL_LEVELS[level_number - 1];

    total_waves = config.total_waves;
    current_wave = 0;
    wave_timer = config.initial_delay * 60; 

    for (int i = 0; i < total_waves; i++) {
        level_waves[i].num_zombies = config.waves[i].num_zombies;
        level_waves[i].time_to_next_wave = config.waves[i].time_to_next_wave * 60;

        for (int j = 0; j < config.waves[i].num_zombies; j++) {
            level_waves[i].spawns[j].type = config.waves[i].spawns[j].type;
            level_waves[i].spawns[j].lane = config.waves[i].spawns[j].lane;
        }
    }
}

void wave_manager_update() {
    if (total_waves == 0) return;

    if (current_wave >= total_waves) {
        if (wave_timer > 0) wave_timer--;
        return;
    }

    if (wave_timer > 0) {
        wave_timer--;
        return;
    }

    Wave *w = &level_waves[current_wave];
    
    bool was_empty[MAX_ZOMBIES];
    for (int z = 0; z < MAX_ZOMBIES; z++) {
        was_empty[z] = !zombies[z].active;
    }

    for (int i = 0; i < w->num_zombies; i++) {
        uint8_t lane = (uint8_t)(w->spawns[i].lane);
        if (lane >= GRID_ROWS) lane = GRID_ROWS - 1; 
        spawn_zombie(lane, current_level);
    }
    
    int lane_offsets[5] = {0}; 
    
    for (int z = 0; z < MAX_ZOMBIES; z++) {
        if (was_empty[z] && zombies[z].active) {
            uint8_t lane = zombies[z].lane;
            zombies[z].x += lane_offsets[lane] * 50; 
            lane_offsets[lane]++; 
        }
    }
    
    wave_timer = w->time_to_next_wave;
    current_wave++;
}

bool wave_manager_is_complete() {
    return (current_wave >= total_waves && wave_timer == 0 && zombies_alive_count() == 0);
}
