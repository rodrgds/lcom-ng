#include "pvz_platform.h"
#include <stdlib.h>

#include "wave.h"
#include "state.h"

/* Wave configuration */
typedef struct {
    int            zombies;               /* total de zombies a spawnar */
    int            spawn_interval;        /* ticks entre spawns */
    int            conehead_chance;       /* % de probabilidade de conehead */
    int            pole_vaulting_chance;  /* % de probabilidade de pole vaulting */
    int            initial_delay;         /* ticks antes do 1o spawn */
    int            break_after;           /* pausa apos esta wave antes da proxima (ticks) */
    WaveBannerType banner_type;
} WaveConfig;

static const WaveConfig wave_configs[MAX_WAVES] = {
    /*          zombies  interval  cone%  pole%  delay             break  banner            */
    /* Wave 1 - initial warmup */
    {  4, 150,  5,  0, WAVE_LONG_DELAY,   180, WAVE_BANNER_NONE  },
    /* Wave 2 - normal */
    {  7, 110, 15,  8, WAVE_SHORT_DELAY,  300, WAVE_BANNER_NONE  },
    /* Wave 3 - huge wave */
    { 12,  75, 50, 16, WAVE_LONG_DELAY,   180, WAVE_BANNER_HUGE  },
    /* Wave 4 - normal+ */
    {  9,  95, 25, 12, WAVE_SHORT_DELAY,  420, WAVE_BANNER_NONE  },
    /* Wave 5 - final mega wave */
    { 18,  60, 80, 24, WAVE_LONG_DELAY,   180, WAVE_BANNER_FINAL },
};

/* Global state */
static WaveState wave_state;

/* Helpers */
static bool any_zombie_alive(Zombie *zombies, int max_zombies) {
    for (int i = 0; i < max_zombies; i++) {
        if (zombies[i].alive) return true;
    }
    return false;
}

static void spawn_zombie(Zombie *zombies, int max_zombies) {
    for (int i = 0; i < max_zombies; i++) {
        if (!zombies[i].alive) {
            int row  = rand() % GRID_ROWS;
            int idx  = wave_state.wave_number - 1;
            int roll = rand() % 100;
            int pole_chance = wave_configs[idx].pole_vaulting_chance;
            int cone_chance = wave_configs[idx].conehead_chance;
            ZombieType type;
            if (roll < pole_chance) type = POLE_VAULTING_ZOMBIE;
            else if (roll < pole_chance + cone_chance) type = CONEHEAD_ZOMBIE;
            else type = REGULAR_ZOMBIE;
            init_zombie(&zombies[i], row, type);
            wave_state.zombies_spawned++;
            return;
        }
    }
}

static void apply_wave_config(int wave_number) {
    int idx = wave_number - 1;
    const WaveConfig *cfg = &wave_configs[idx];

    wave_state.zombies_total   = cfg->zombies;
    wave_state.zombies_spawned = 0;
    wave_state.spawn_interval  = cfg->spawn_interval;
    wave_state.spawn_timer     = cfg->initial_delay;
    wave_state.between_waves   = false;
    wave_state.banner_type     = cfg->banner_type;
    wave_state.banner_timer    = (cfg->banner_type != WAVE_BANNER_NONE) ? WAVE_BANNER_TICKS : 0;

    set_game_wave(wave_number);
}

/* Public API */
void init_wave_system(void) {
    wave_state.wave_number  = 1;
    wave_state.break_timer  = 0;
    wave_state.active       = true;
    wave_state.banner_timer = 0;
    wave_state.banner_type  = WAVE_BANNER_NONE;

    apply_wave_config(1);
}

void update_wave_system(Zombie *zombies, int max_zombies) {
    if (!wave_state.active) return;

    /* Decrementa o timer do banner */
    if (wave_state.banner_timer > 0)
        wave_state.banner_timer--;

    if (wave_state.between_waves) {
        wave_state.break_timer--;
        if (wave_state.break_timer <= 0) {
            if (wave_state.wave_number >= MAX_WAVES) {
                wave_state.active = false;
                set_game_state(STATE_VICTORY);
            } else {
                wave_state.wave_number++;
                apply_wave_config(wave_state.wave_number);
            }
        }
        return;
    }

    if (wave_state.zombies_spawned >= wave_state.zombies_total) {
        if (!any_zombie_alive(zombies, max_zombies)) {
            wave_state.between_waves = true;
            wave_state.break_timer   = wave_configs[wave_state.wave_number - 1].break_after;
        }
        return;
    }

    wave_state.spawn_timer--;
    if (wave_state.spawn_timer <= 0) {
        spawn_zombie(zombies, max_zombies);
        wave_state.spawn_timer = wave_state.spawn_interval;
    }
}

WaveState *get_wave_state(void) {
    return &wave_state;
}
