#ifndef PROJ_LEVEL_3_H
#define PROJ_LEVEL_3_H

#include "../path_map.h"
#include "../wave.h"
#include "../enemy_types.h"
#include "../app_constants.h"

static const char level_3_data[] =
    "..................\n"
    ".S#############...\n"
    "..............#...\n"
    "...############...\n"
    "...#..............\n"
    "...############...\n"
    "..............#...\n"
    "...############...\n"
    "...#..............\n"
    "...############...\n"
    "..............#...\n"
    "...############...\n"
    "...#..............\n"
    "...############G..";

/* Wave 1 */
static const wave_entry_t l3_wave1_entries[] = {
  { .kind = ENEMY_KIND_NORMAL, .count = 8,  .burst_size = 2, .burst_interval = 8, .spawn_interval = 75 },
  { .kind = ENEMY_KIND_FAST,   .count = 4,  .burst_size = 2, .burst_interval = 6, .spawn_interval = 90 },
};

/* Wave 2: healers + cash */
static const wave_entry_t l3_wave2_entries[] = {
  { .kind = ENEMY_KIND_NORMAL, .count = 8,  .burst_size = 2, .burst_interval = 8, .spawn_interval = 70 },
  { .kind = ENEMY_KIND_HEALING,.count = 3,  .burst_size = 1, .spawn_interval = 100 },
  { .kind = ENEMY_KIND_CASH,   .count = 1,  .burst_size = 1, .spawn_interval = 160 },
};

/* Wave 3: invisible introduced */
static const wave_entry_t l3_wave3_entries[] = {
  { .kind = ENEMY_KIND_INVISIBLE, .count = 6,  .burst_size = 2, .burst_interval = 8, .spawn_interval = 90 },
  { .kind = ENEMY_KIND_NORMAL,    .count = 8,  .burst_size = 2, .burst_interval = 8, .spawn_interval = 70 },
};

/* Wave 4: teleporting + armor */
static const wave_entry_t l3_wave4_entries[] = {
  { .kind = ENEMY_KIND_TELEPORTING, .count = 4,  .burst_size = 1, .spawn_interval = 100 },
  { .kind = ENEMY_KIND_ARMORED,     .count = 4,  .burst_size = 1, .spawn_interval = 105 },
  { .kind = ENEMY_KIND_NORMAL,      .count = 6,  .burst_size = 2, .burst_interval = 8, .spawn_interval = 75 },
};

/* Wave 5: explode + armor + fast */
static const wave_entry_t l3_wave5_entries[] = {
  { .kind = ENEMY_KIND_EXPLODING, .count = 5,  .burst_size = 1, .spawn_interval = 90 },
  { .kind = ENEMY_KIND_ARMORED,   .count = 4,  .burst_size = 1, .spawn_interval = 105 },
  { .kind = ENEMY_KIND_FAST,      .count = 6,  .burst_size = 2, .burst_interval = 6, .spawn_interval = 70 },
};

/* Wave 6: mixed special final */
static const wave_entry_t l3_wave6_entries[] = {
  { .kind = ENEMY_KIND_INVISIBLE,   .count = 6,  .burst_size = 2, .burst_interval = 7, .spawn_interval = 80 },
  { .kind = ENEMY_KIND_TELEPORTING, .count = 5,  .burst_size = 1, .spawn_interval = 85 },
  { .kind = ENEMY_KIND_ARMORED,     .count = 5,  .burst_size = 1, .spawn_interval = 95 },
  { .kind = ENEMY_KIND_CASH,        .count = 2,  .burst_size = 1, .spawn_interval = 150 },
};

static const wave_t level_3_waves[] = {
  { .entries = l3_wave1_entries, .entry_count = 2, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l3_wave2_entries, .entry_count = 2, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l3_wave3_entries, .entry_count = 2, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l3_wave4_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l3_wave5_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l3_wave6_entries, .entry_count = 4, .pre_wave_delay = APP_TIMER_HZ * 10 },
};

static const level_t level_3 = {
  .width = 18,
  .height = 14,
  .data = level_3_data,
  .waves = level_3_waves,
  .wave_count = 6,
  .start_coins = 360,
  .wave_bonus_base = 35,
  .wave_bonus_step = 14,
};

#endif
