#ifndef _WAVE_H_
#define _WAVE_H_

#include <stdbool.h>
#include "zombie.h"
#include "constants.h"

/* Banner type shown at the start of a wave */
typedef enum {
    WAVE_BANNER_NONE  = 0,
    WAVE_BANNER_HUGE  = 1,   /* "HUGE WAVE INCOMING" - wave 3 */
    WAVE_BANNER_FINAL = 2    /* "FINAL WAVE"         - wave 5 */
} WaveBannerType;

typedef struct {
    int            wave_number;
    int            zombies_total;    /* total a spawnar nesta onda */
    int            zombies_spawned;
    int            spawn_interval;   /* ticks entre spawns */
    int            spawn_timer;
    int            break_timer;      /* pausa entre ondas */
    bool           active;
    bool           between_waves;
    int            banner_timer;     /* ticks restantes do banner; 0 = oculto */
    WaveBannerType banner_type;
} WaveState;

void init_wave_system(void);
void update_wave_system(Zombie *zombies, int max_zombies);
WaveState *get_wave_state(void);

#endif /* _WAVE_H_ */
