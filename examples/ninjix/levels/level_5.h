#ifndef PROJ_LEVEL_5_H
#define PROJ_LEVEL_5_H

#include "../path_map.h"
#include "../wave.h"
#include "../enemy_types.h"
#include "../app_constants.h"

static const char level_5_data[] =
    "..................\n"
    "..S############...\n"
    "..............#...\n"
    "..............#...\n"
    "..#############...\n"
    "..#...............\n"
    "..#...............\n"
    "..########........\n"
    ".........#........\n"
    ".........#........\n"
    ".........#........\n"
    ".........#........\n"
    "...#######........\n"
    "...#..............\n"
    "...#...........G..\n"
    "...#############..\n"
    "..................\n"
    "..................\n"
    "..................\n"
    "..................";

/* Wave 1: baseline pressure */
static const wave_entry_t l5_wave1_entries[] = {
  { .kind = ENEMY_KIND_NORMAL, .count = 10, .burst_size = 2, .burst_interval = 8, .spawn_interval = 60 },
  { .kind = ENEMY_KIND_FAST,   .count = 10, .burst_size = 2, .burst_interval = 6, .spawn_interval = 58 },
};

/* Wave 2: invis + teleport */
static const wave_entry_t l5_wave2_entries[] = {
  { .kind = ENEMY_KIND_INVISIBLE,   .count = 10, .burst_size = 2, .burst_interval = 7, .spawn_interval = 70 },
  { .kind = ENEMY_KIND_TELEPORTING, .count = 8,  .burst_size = 1, .spawn_interval = 75 },
};

/* Wave 3: sniper/poison check */
static const wave_entry_t l5_wave3_entries[] = {
  { .kind = ENEMY_KIND_ARMORED, .count = 10, .burst_size = 1, .spawn_interval = 75 },
  { .kind = ENEMY_KIND_FAST,    .count = 8,  .burst_size = 2, .burst_interval = 6, .spawn_interval = 55 },
};

/* Wave 4: anti-heal */
static const wave_entry_t l5_wave4_entries[] = {
  { .kind = ENEMY_KIND_HEALING, .count = 8,  .burst_size = 1, .spawn_interval = 75 },
  { .kind = ENEMY_KIND_ARMORED, .count = 8,  .burst_size = 1, .spawn_interval = 80 },
  { .kind = ENEMY_KIND_NORMAL,  .count = 8,  .burst_size = 2, .burst_interval = 8, .spawn_interval = 60 },
};

/* Wave 5: leak punishment */
static const wave_entry_t l5_wave5_entries[] = {
  { .kind = ENEMY_KIND_EXPLODING, .count = 10, .burst_size = 1, .spawn_interval = 70 },
  { .kind = ENEMY_KIND_FAST,      .count = 10, .burst_size = 2, .burst_interval = 6, .spawn_interval = 55 },
  { .kind = ENEMY_KIND_INVISIBLE, .count = 6,  .burst_size = 2, .burst_interval = 7, .spawn_interval = 70 },
};

/* Wave 6: support stress */
static const wave_entry_t l5_wave6_entries[] = {
  { .kind = ENEMY_KIND_TELEPORTING, .count = 8,  .burst_size = 1, .spawn_interval = 70 },
  { .kind = ENEMY_KIND_HEALING,     .count = 8,  .burst_size = 1, .spawn_interval = 75 },
  { .kind = ENEMY_KIND_ARMORED,     .count = 8,  .burst_size = 1, .spawn_interval = 75 },
};

/* Wave 7: heavy mixed */
static const wave_entry_t l5_wave7_entries[] = {
  { .kind = ENEMY_KIND_INVISIBLE,  .count = 10, .burst_size = 2, .burst_interval = 7, .spawn_interval = 65 },
  { .kind = ENEMY_KIND_ARMORED,    .count = 10, .burst_size = 1, .spawn_interval = 70 },
  { .kind = ENEMY_KIND_EXPLODING, .count = 8,  .burst_size = 1, .spawn_interval = 70 },
};

/* Wave 8: final wave */
static const wave_entry_t l5_wave8_entries[] = {
  { .kind = ENEMY_KIND_ARMORED,     .count = 12, .burst_size = 1, .spawn_interval = 65 },
  { .kind = ENEMY_KIND_TELEPORTING, .count = 10, .burst_size = 1, .spawn_interval = 68 },
  { .kind = ENEMY_KIND_HEALING,     .count = 8,  .burst_size = 1, .spawn_interval = 70 },
  { .kind = ENEMY_KIND_EXPLODING,   .count = 8,  .burst_size = 1, .spawn_interval = 68 },
  { .kind = ENEMY_KIND_CASH,        .count = 2,  .burst_size = 1, .spawn_interval = 140 },
};

static const wave_t level_5_waves[] = {
  { .entries = l5_wave1_entries, .entry_count = 2, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l5_wave2_entries, .entry_count = 2, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l5_wave3_entries, .entry_count = 2, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l5_wave4_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l5_wave5_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l5_wave6_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l5_wave7_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l5_wave8_entries, .entry_count = 5, .pre_wave_delay = APP_TIMER_HZ * 10 },
};

static const level_t level_5 = {
  .width = 18,
  .height = 20,
  .data = level_5_data,
  .waves = level_5_waves,
  .wave_count = 8,
  .start_coins = 430,
  .wave_bonus_base = 50,
  .wave_bonus_step = 18,
};

#endif
