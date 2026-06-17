#ifndef PROJ_LEVEL_1_H
#define PROJ_LEVEL_1_H

#include "../path_map.h"
#include "../wave.h"
#include "../enemy_types.h"
#include "../app_constants.h"

static const char level_1_data[] =
    "..................\n"
    "..................\n"
    "S#####............\n"
    ".....#............\n"
    ".....#............\n"
    ".....#####........\n"
    ".........#........\n"
    ".........#........\n"
    ".........#........\n"
    ".........####.....\n"
    "............#.....\n"
    "............#.....\n"
    "............#.....\n"
    "............#.....\n"
    "............#.G...\n"
    "............###...\n"
    "..................\n"
    "..................";

/* Wave 1: 6 normals */
static const wave_entry_t l1_wave1_entries[] = {
  { .kind = ENEMY_KIND_NORMAL, .count = 6, .burst_size = 1, .spawn_interval = 90 },
};

/* Wave 2: normals + fast */
static const wave_entry_t l1_wave2_entries[] = {
  { .kind = ENEMY_KIND_NORMAL, .count = 8, .burst_size = 2, .burst_interval = 8, .spawn_interval = 85 },
  { .kind = ENEMY_KIND_FAST,   .count = 2, .burst_size = 1, .spawn_interval = 110 },
};

/* Wave 3: introduce cash */
static const wave_entry_t l1_wave3_entries[] = {
  { .kind = ENEMY_KIND_NORMAL, .count = 10, .burst_size = 2, .burst_interval = 8, .spawn_interval = 75 },
  { .kind = ENEMY_KIND_FAST,   .count = 4,  .burst_size = 1, .spawn_interval = 90 },
  { .kind = ENEMY_KIND_CASH,   .count = 1,  .burst_size = 1, .spawn_interval = 150 },
};

/* Wave 4: first healer */
static const wave_entry_t l1_wave4_entries[] = {
  { .kind = ENEMY_KIND_NORMAL, .count = 8,  .burst_size = 2, .burst_interval = 7, .spawn_interval = 70 },
  { .kind = ENEMY_KIND_FAST,   .count = 4,  .burst_size = 2, .burst_interval = 6, .spawn_interval = 80 },
  { .kind = ENEMY_KIND_HEALING,.count = 3,  .burst_size = 1, .spawn_interval = 95 },
};

/* Wave 5: armored appears */
static const wave_entry_t l1_wave5_entries[] = {
  { .kind = ENEMY_KIND_NORMAL, .count = 10, .burst_size = 2, .burst_interval = 7, .spawn_interval = 65 },
  { .kind = ENEMY_KIND_FAST,   .count = 6,  .burst_size = 2, .burst_interval = 6, .spawn_interval = 75 },
  { .kind = ENEMY_KIND_ARMORED,.count = 2,  .burst_size = 1, .spawn_interval = 110 },
};

static const wave_t level_1_waves[] = {
  { .entries = l1_wave1_entries, .entry_count = 1, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l1_wave2_entries, .entry_count = 2, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l1_wave3_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l1_wave4_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l1_wave5_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
};

static const level_t level_1 = {
  .width = 18,
  .height = 18,
  .data = level_1_data,
  .waves = level_1_waves,
  .wave_count = 5,
  .start_coins = 230,
  .wave_bonus_base = 25,
  .wave_bonus_step = 10,
};

#endif
