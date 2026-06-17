#ifndef PROJ_LEVEL_2_H
#define PROJ_LEVEL_2_H

#include "../path_map.h"
#include "../wave.h"
#include "../enemy_types.h"
#include "../app_constants.h"

static const char level_2_data[] =
    "..................\n"
    ".S######..........\n"
    ".......#..........\n"
    ".......#####......\n"
    "...........#......\n"
    "....########......\n"
    "....#.........G...\n"
    "....#.........#...\n"
    "....#.........#...\n"
    "....#########.#...\n"
    "............#.#...\n"
    "............#.#...\n"
    "............###...\n"
    "..................";

/* Wave 1 */
static const wave_entry_t l2_wave1_entries[] = {
  { .kind = ENEMY_KIND_NORMAL, .count = 10, .burst_size = 2, .burst_interval = 8, .spawn_interval = 70 },
  { .kind = ENEMY_KIND_FAST,   .count = 4,  .burst_size = 1, .spawn_interval = 90 },
};

/* Wave 2: fast pressure */
static const wave_entry_t l2_wave2_entries[] = {
  { .kind = ENEMY_KIND_FAST,   .count = 8,  .burst_size = 2, .burst_interval = 6, .spawn_interval = 70 },
  { .kind = ENEMY_KIND_NORMAL, .count = 8,  .burst_size = 2, .burst_interval = 8, .spawn_interval = 75 },
};

/* Wave 3: armored appears */
static const wave_entry_t l2_wave3_entries[] = {
  { .kind = ENEMY_KIND_NORMAL, .count = 8,  .burst_size = 2, .burst_interval = 8, .spawn_interval = 70 },
  { .kind = ENEMY_KIND_ARMORED, .count = 4, .burst_size = 1, .spawn_interval = 105 },
};

/* Wave 4: healer pressure */
static const wave_entry_t l2_wave4_entries[] = {
  { .kind = ENEMY_KIND_FAST,   .count = 8,  .burst_size = 2, .burst_interval = 6, .spawn_interval = 65 },
  { .kind = ENEMY_KIND_HEALING,.count = 4,  .burst_size = 1, .spawn_interval = 100 },
  { .kind = ENEMY_KIND_NORMAL, .count = 6,  .burst_size = 2, .burst_interval = 8, .spawn_interval = 80 },
};

/* Wave 5: exploding introduced */
static const wave_entry_t l2_wave5_entries[] = {
  { .kind = ENEMY_KIND_NORMAL,  .count = 10, .burst_size = 2, .burst_interval = 8, .spawn_interval = 65 },
  { .kind = ENEMY_KIND_EXPLODING,.count = 4, .burst_size = 1, .spawn_interval = 95 },
  { .kind = ENEMY_KIND_ARMORED, .count = 3,  .burst_size = 1, .spawn_interval = 110 },
};

/* Wave 6: mixed final */
static const wave_entry_t l2_wave6_entries[] = {
  { .kind = ENEMY_KIND_FAST,   .count = 8,  .burst_size = 2, .burst_interval = 6, .spawn_interval = 60 },
  { .kind = ENEMY_KIND_ARMORED,.count = 5,  .burst_size = 1, .spawn_interval = 95 },
  { .kind = ENEMY_KIND_HEALING,.count = 4,  .burst_size = 1, .spawn_interval = 100 },
  { .kind = ENEMY_KIND_CASH,   .count = 1,  .burst_size = 1, .spawn_interval = 160 },
};

static const wave_t level_2_waves[] = {
  { .entries = l2_wave1_entries, .entry_count = 2, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l2_wave2_entries, .entry_count = 2, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l2_wave3_entries, .entry_count = 2, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l2_wave4_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l2_wave5_entries, .entry_count = 3, .pre_wave_delay = APP_TIMER_HZ * 10 },
  { .entries = l2_wave6_entries, .entry_count = 4, .pre_wave_delay = APP_TIMER_HZ * 10 },
};

static const level_t level_2 = {
  .width = 18,
  .height = 14,
  .data = level_2_data,
  .waves = level_2_waves,
  .wave_count = 6,
  .start_coins = 270,
  .wave_bonus_base = 30,
  .wave_bonus_step = 12,
};

#endif
