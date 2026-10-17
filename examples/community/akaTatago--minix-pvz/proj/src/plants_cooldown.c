#include "plants_cooldown.h"

static int plant_cooldown_timers[4]; 

void cooldowns_init() {
    plant_cooldown_timers[0] = 0;
    plant_cooldown_timers[PLANT_SUNFLOWER] = 0;
    plant_cooldown_timers[PLANT_PEASHOOTER] = 0;
    plant_cooldown_timers[PLANT_NUT] = 0; 
}

void cooldowns_update() {
    for (int i = 0; i < 4; i++) { 
        if (plant_cooldown_timers[i] > 0) {
            plant_cooldown_timers[i]--;
        }
    }
}

bool cooldown_is_ready(plant_type_t type) {
    // Changed upper bound check from PLANT_PEASHOOTER to PLANT_NUT
    if (type <= PLANT_NONE || type > PLANT_NUT) return false; 
    return plant_cooldown_timers[type] == 0;
}

void cooldown_start(plant_type_t type) {
    if (type == PLANT_SUNFLOWER) {
        plant_cooldown_timers[type] = COOLDOWN_FAST; 
    } else if (type == PLANT_PEASHOOTER) {
        plant_cooldown_timers[type] = COOLDOWN_FAST; 
    } else if (type == PLANT_NUT) {
        plant_cooldown_timers[type] = COOLDOWN_SLOW; 
    }
}