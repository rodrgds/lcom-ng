#ifndef PROJ_LEVEL_4_H
#define PROJ_LEVEL_4_H

#include "../path_map.h"
#include "../wave.h"
#include "../enemy_types.h"
#include "../app_constants.h"

static const char level_4_data[] =
    "..................\n"
    "..S##########.....\n"
    "............#.....\n"
    "..###########.....\n"
    "..#...............\n"
    "..#...............\n"
    "..#...............\n"
    "..#...............\n"
    "..#...............\n"
    "..#####...........\n"
    "......#...........\n"
    "......#...........\n"
    "......#########...\n"
    "..............#...\n"
    "..............#.G.\n"
    "..............###.\n"
    "..................\n"
    "..................";

/* Wave 1: economy opener */
static const wave_entry_t l4_wave1_entries[] = {
  { .kind = ENEMY_KIND_FAST,   .count = 10, .burst_size = 2, .burst_interval = 6, .spawn_interval = 60 },
  { .kind = ENEMY_KIND_NORMAL, .count = 8,  .burst_size = 2, .burst_interval = 8, .spawn_interval = 65 },
  { .kind = ENEMY_KIND_CASH,   .count = 1,  .burst_size = 1, .spawn_interval = 150 },
};

/* Wave 2: armor wall */
static const wave_entry_t l4_wave2_entries[] = {
  { .kind = ENEMY_KIND_ARMORED, .count = 8,  .burst_size = 1, .spawn_interval = 85 },
  { .kind = ENEMY_KIND_NORMAL,  .count = 6,  .burst_size = 2, .burst_interval = 8, .spawn_interval = 65 },
};

/* Wave 3: healer swarm */
static const wave_entry_t l4_wave3_entries[] = {
  { .kind = ENEMY_KIND_HEALING, .count = 7,  .burst_size = 1, .spawn_interval = 80 },
  { .kind = ENEMY_KIND_FAST,    .count = 10, .burst_size = 2, .burst_interval = 6, .spawn_interval = 58 },
};

/* Wave 4: smoke/reveal check */
static const wave_entry_t l4_wave4_entries[] = {
  { .kind = ENEMY_KIND_TELEPORTING, .count = 8,  .burst_size = 1, .spawn_interval = 75 },
  { .kind = ENEMY_KIND_INVISIBLE,   .count = 8,  .burst_size = 2, .burst_interval = 7, .spawn_interval = 75 },
  { .kind = ENEMY_KIND_NORMAL,      .count = 6,  .burst_size = 2, .burst_interval = 8, .spawn_interval = 65 },
};

/* Wave 5: leak danger */
static const wave_entry_t l4_wave5_entries[] = {
  { .kind = ENEMY_KIND_EXPLODING, .count = 8,  .burst_size = 1, .spawn_interval = 75 },
  { .kind = ENEMY_KIND_ARMORED,   .count = 6,  .burst_size = 1, .spawn_interval = 85 },
  { .kind = ENEMY_KIND_FAST,      .count = 8,  .burst_size = 2, .burst_interval = 6, .spawn_interval = 58 },
};

/* Wave 6: priority targeting */
static const wave_entry_t l4_wave6_entries[] = {
  { .kind = ENEMY_KIND_HEALING,     .count = 8,  .burst_size = 1, .spawn_interval = 75 },
  { .kind = ENEMY_KIND_ARMORED,     .count = 6,  .burst_size = 1, .spawn_interval = 80 },
  { .kind = ENEMY_KIND_TELEPORTING, .count = 6,  .burst_size = 1, .spawn_interval = 75 },
};

/* Wave 7: hard mixed final */
static const wave_entry_t l4_wave7_entries[] = {
  { .kind = ENEMY_KIND_ARMORED,     .count = 10, .burst_size = 1, .spawn_interval = 75 },
  { .kind = ENEMY_KIND_INVISIBLE,   .count = 8,  .burst_size = 2, .burst_interval = 7, .spawn_interval = 70 },
  { .kind = ENEMY_KIND_EXPLODING,   .count = 6,  .burst_size = 1, .spawn_interval = 75 },
  { .kind = ENEMY_KIND_CASH,        .count = 2,  .burst_size = 1, .spawn_interval = 145 },
};

static const wave_t level_4_waves[] = {
  { .entries = l4_wave1_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l4_wave2_entries, .entry_count = 2, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l4_wave3_entries, .entry_count = 2, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l4_wave4_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l4_wave5_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l4_wave6_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l4_wave7_entries, .entry_count = 4, .pre_wave_delay = APP_TIMER_HZ * 10 },
};

static const level_t level_4 = {
  .width = 18,
  .height = 18,
  .data = level_4_data,
  .waves = level_4_waves,
  .wave_count = 7,
  .start_coins = 370,
  .wave_bonus_base = 40,
  .wave_bonus_step = 16,
};

#endif
