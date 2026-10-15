#include "../include/game/waves.h"
#include "../include/game/game_rand.h"
#include "../include/macros/physics.h"

#include <stddef.h>
#include <stdlib.h>

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

#define WAVE_MIN_SPEED_EASY 22
#define WAVE_MAX_SPEED_EASY 24
#define WAVE_MIN_SPEED_EASY_MID 23
#define WAVE_MAX_SPEED_EASY_MID 25
#define WAVE_MIN_SPEED_MID 24
#define WAVE_MAX_SPEED_MID 26
#define WAVE_MIN_SPEED_MID_HARD 25
#define WAVE_MAX_SPEED_MID_HARD 28
#define WAVE_MIN_SPEED_HARD 26
#define WAVE_MAX_SPEED_HARD 29

#define WAVE_GRAVITY_EASY PHYSICS_FROM_THOUSANDTHS(500)
#define WAVE_GRAVITY_EASY_MID PHYSICS_FROM_THOUSANDTHS(560)
#define WAVE_GRAVITY_MID PHYSICS_FROM_THOUSANDTHS(620)
#define WAVE_GRAVITY_MID_HARD PHYSICS_FROM_THOUSANDTHS(700)
#define WAVE_GRAVITY_HARD PHYSICS_FROM_THOUSANDTHS(780)

#define WAVE_FIRST_DELAY_TICKS 120
#define WAVE_GAP_MIN_TICKS 60
#define WAVE_GAP_MAX_TICKS 180

#define WAVE_MOTION_PROFILE(min_speed, max_speed, gravity_value)               \
  {                                                                            \
    .min_vy = -PHYSICS_FROM_PIXELS(max_speed),                                 \
    .max_vy = -PHYSICS_FROM_PIXELS(min_speed),                                 \
    .gravity = (gravity_value),                                                \
    .min_vx = -PHYSICS_FROM_PIXELS((max_speed) / 10),                          \
    .max_vx = PHYSICS_FROM_PIXELS((max_speed) / 10)                            \
  }

#define WAVE_EVENT(delay, spawn_kind, spawn_amount, score)                     \
  {                                                                            \
    .delay_ticks = (delay),                                                    \
    .request = {                                                               \
      .kind = (spawn_kind),                                                    \
      .sushi_type = WAVE_RANDOM_SUSHI_TYPE,                                    \
      .amount = (spawn_amount),                                                \
      .score_value = (score),                                                  \
    },                                                                         \
  }

typedef struct {
  motion_profile_t motion;
} wave_difficulty_profile_t;

static const wave_difficulty_profile_t DIFFICULTY_PROFILES[] = {
  [EASY] = {
    .motion = WAVE_MOTION_PROFILE(WAVE_MIN_SPEED_EASY,
                                  WAVE_MAX_SPEED_EASY,
                                  WAVE_GRAVITY_EASY),
  },
  [EASY_MID] = {
    .motion = WAVE_MOTION_PROFILE(WAVE_MIN_SPEED_EASY_MID,
                                  WAVE_MAX_SPEED_EASY_MID,
                                  WAVE_GRAVITY_EASY_MID),
  },
  [MID] = {
    .motion = WAVE_MOTION_PROFILE(WAVE_MIN_SPEED_MID,
                                  WAVE_MAX_SPEED_MID,
                                  WAVE_GRAVITY_MID),
  },
  [MID_HARD] = {
    .motion = WAVE_MOTION_PROFILE(WAVE_MIN_SPEED_MID_HARD,
                                  WAVE_MAX_SPEED_MID_HARD,
                                  WAVE_GRAVITY_MID_HARD),
  },
  [HARD] = {
    .motion = WAVE_MOTION_PROFILE(WAVE_MIN_SPEED_HARD,
                                  WAVE_MAX_SPEED_HARD,
                                  WAVE_GRAVITY_HARD),
  },
};

static const wave_event_t INTRO_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
};

static const wave_event_t SINGLE_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
};

static const wave_event_t BOMB_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 1, 0),
};

static const wave_event_t BURST_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 3, 1),
};

static const wave_event_t QUAD_BURST_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 4, 1),
};

static const wave_event_t FIVE_BURST_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 5, 1),
};

static const wave_event_t BURST_BOMB_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 1, 0),
};

static const wave_event_t STAGGERED_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 1, 1),
};

static const wave_event_t PUMP4_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(15, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(15, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(15, WAVE_SPAWN_REGULAR, 1, 1),
};

static const wave_event_t SPLIT_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(45, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(45, WAVE_SPAWN_REGULAR, 2, 1),
};

static const wave_event_t STAGGERED_BOMB_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 1, 1),
};

static const wave_event_t PUMP_BOMB_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(15, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(15, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(15, WAVE_SPAWN_REGULAR, 1, 1),
};

static const wave_event_t FEINT_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(30, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(15, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
};

static const wave_event_t EASY_MID_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(60, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
};

static const wave_event_t EASY_MID_BURST_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(60, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 3, 1),
};

static const wave_event_t MIXED_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(60, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
};

static const wave_event_t MIXED_BOMB_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 2, 0),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
};

static const wave_event_t QUICK_MIX_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(15, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(15, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(15, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(30, WAVE_SPAWN_BOMB, 1, 0),
};

static const wave_event_t DOUBLE_BURST_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(45, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 1, 0),
};

static const wave_event_t MID_HARD_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(60, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
};

static const wave_event_t MID_HARD_BURST_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 1, 0),
};

static const wave_event_t HARD_MIX_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 2, 0),
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(60, WAVE_SPAWN_BOMB, 1, 0),
};

static const wave_event_t HARD_CHAIN_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(15, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(15, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(15, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(15, WAVE_SPAWN_REGULAR, 2, 1),
};

static const wave_t PREDEFINED_WAVES[WAVE_PATTERN_COUNT] = {
  [WAVE_PATTERN_INTRO] = {
    .pattern = WAVE_PATTERN_INTRO,
    .events = INTRO_EVENTS,
    .event_count = ARRAY_COUNT(INTRO_EVENTS),
  },
  [WAVE_PATTERN_SINGLE] = {
    .pattern = WAVE_PATTERN_SINGLE,
    .events = SINGLE_EVENTS,
    .event_count = ARRAY_COUNT(SINGLE_EVENTS),
  },
  [WAVE_PATTERN_BOMB] = {
    .pattern = WAVE_PATTERN_BOMB,
    .events = BOMB_EVENTS,
    .event_count = ARRAY_COUNT(BOMB_EVENTS),
  },
  [WAVE_PATTERN_BURST] = {
    .pattern = WAVE_PATTERN_BURST,
    .events = BURST_EVENTS,
    .event_count = ARRAY_COUNT(BURST_EVENTS),
  },
  [WAVE_PATTERN_QUAD_BURST] = {
    .pattern = WAVE_PATTERN_QUAD_BURST,
    .events = QUAD_BURST_EVENTS,
    .event_count = ARRAY_COUNT(QUAD_BURST_EVENTS),
  },
  [WAVE_PATTERN_FIVE_BURST] = {
    .pattern = WAVE_PATTERN_FIVE_BURST,
    .events = FIVE_BURST_EVENTS,
    .event_count = ARRAY_COUNT(FIVE_BURST_EVENTS),
  },
  [WAVE_PATTERN_BURST_BOMB] = {
    .pattern = WAVE_PATTERN_BURST_BOMB,
    .events = BURST_BOMB_EVENTS,
    .event_count = ARRAY_COUNT(BURST_BOMB_EVENTS),
  },
  [WAVE_PATTERN_STAGGERED] = {
    .pattern = WAVE_PATTERN_STAGGERED,
    .events = STAGGERED_EVENTS,
    .event_count = ARRAY_COUNT(STAGGERED_EVENTS),
  },
  [WAVE_PATTERN_PUMP4] = {
    .pattern = WAVE_PATTERN_PUMP4,
    .events = PUMP4_EVENTS,
    .event_count = ARRAY_COUNT(PUMP4_EVENTS),
  },
  [WAVE_PATTERN_SPLIT] = {
    .pattern = WAVE_PATTERN_SPLIT,
    .events = SPLIT_EVENTS,
    .event_count = ARRAY_COUNT(SPLIT_EVENTS),
  },
  [WAVE_PATTERN_STAGGERED_BOMB] = {
    .pattern = WAVE_PATTERN_STAGGERED_BOMB,
    .events = STAGGERED_BOMB_EVENTS,
    .event_count = ARRAY_COUNT(STAGGERED_BOMB_EVENTS),
  },
  [WAVE_PATTERN_PUMP_BOMB] = {
    .pattern = WAVE_PATTERN_PUMP_BOMB,
    .events = PUMP_BOMB_EVENTS,
    .event_count = ARRAY_COUNT(PUMP_BOMB_EVENTS),
  },
  [WAVE_PATTERN_FEINT] = {
    .pattern = WAVE_PATTERN_FEINT,
    .events = FEINT_EVENTS,
    .event_count = ARRAY_COUNT(FEINT_EVENTS),
  },
  [WAVE_PATTERN_EASY_MID] = {
    .pattern = WAVE_PATTERN_EASY_MID,
    .events = EASY_MID_EVENTS,
    .event_count = ARRAY_COUNT(EASY_MID_EVENTS),
  },
  [WAVE_PATTERN_EASY_MID_BURST] = {
    .pattern = WAVE_PATTERN_EASY_MID_BURST,
    .events = EASY_MID_BURST_EVENTS,
    .event_count = ARRAY_COUNT(EASY_MID_BURST_EVENTS),
  },
  [WAVE_PATTERN_MIXED] = {
    .pattern = WAVE_PATTERN_MIXED,
    .events = MIXED_EVENTS,
    .event_count = ARRAY_COUNT(MIXED_EVENTS),
  },
  [WAVE_PATTERN_MIXED_BOMB] = {
    .pattern = WAVE_PATTERN_MIXED_BOMB,
    .events = MIXED_BOMB_EVENTS,
    .event_count = ARRAY_COUNT(MIXED_BOMB_EVENTS),
  },
  [WAVE_PATTERN_QUICK_MIX] = {
    .pattern = WAVE_PATTERN_QUICK_MIX,
    .events = QUICK_MIX_EVENTS,
    .event_count = ARRAY_COUNT(QUICK_MIX_EVENTS),
  },
  [WAVE_PATTERN_DOUBLE_BURST] = {
    .pattern = WAVE_PATTERN_DOUBLE_BURST,
    .events = DOUBLE_BURST_EVENTS,
    .event_count = ARRAY_COUNT(DOUBLE_BURST_EVENTS),
  },
  [WAVE_PATTERN_MID_HARD] = {
    .pattern = WAVE_PATTERN_MID_HARD,
    .events = MID_HARD_EVENTS,
    .event_count = ARRAY_COUNT(MID_HARD_EVENTS),
  },
  [WAVE_PATTERN_MID_HARD_BURST] = {
    .pattern = WAVE_PATTERN_MID_HARD_BURST,
    .events = MID_HARD_BURST_EVENTS,
    .event_count = ARRAY_COUNT(MID_HARD_BURST_EVENTS),
  },
  [WAVE_PATTERN_HARD_MIX] = {
    .pattern = WAVE_PATTERN_HARD_MIX,
    .events = HARD_MIX_EVENTS,
    .event_count = ARRAY_COUNT(HARD_MIX_EVENTS),
  },
  [WAVE_PATTERN_HARD_CHAIN] = {
    .pattern = WAVE_PATTERN_HARD_CHAIN,
    .events = HARD_CHAIN_EVENTS,
    .event_count = ARRAY_COUNT(HARD_CHAIN_EVENTS),
  },
};

static const uint8_t EASY_POOL[] = {
  WAVE_PATTERN_SINGLE,
  WAVE_PATTERN_BOMB,
  WAVE_PATTERN_BURST,
  WAVE_PATTERN_BURST_BOMB,
  WAVE_PATTERN_STAGGERED,
  WAVE_PATTERN_PUMP4,
  WAVE_PATTERN_SPLIT,
  WAVE_PATTERN_STAGGERED_BOMB,
};

static const uint8_t EASY_MID_POOL[] = {
  WAVE_PATTERN_SINGLE,
  WAVE_PATTERN_BOMB,
  WAVE_PATTERN_BURST,
  WAVE_PATTERN_QUAD_BURST,
  WAVE_PATTERN_FIVE_BURST,
  WAVE_PATTERN_BURST_BOMB,
  WAVE_PATTERN_STAGGERED,
  WAVE_PATTERN_PUMP4,
  WAVE_PATTERN_SPLIT,
  WAVE_PATTERN_STAGGERED_BOMB,
  WAVE_PATTERN_PUMP_BOMB,
  WAVE_PATTERN_FEINT,
  WAVE_PATTERN_EASY_MID,
  WAVE_PATTERN_EASY_MID_BURST,
};

static const uint8_t MID_POOL[] = {
  WAVE_PATTERN_SINGLE,
  WAVE_PATTERN_BOMB,
  WAVE_PATTERN_BURST,
  WAVE_PATTERN_QUAD_BURST,
  WAVE_PATTERN_FIVE_BURST,
  WAVE_PATTERN_BURST_BOMB,
  WAVE_PATTERN_STAGGERED,
  WAVE_PATTERN_PUMP4,
  WAVE_PATTERN_SPLIT,
  WAVE_PATTERN_STAGGERED_BOMB,
  WAVE_PATTERN_PUMP_BOMB,
  WAVE_PATTERN_FEINT,
  WAVE_PATTERN_EASY_MID,
  WAVE_PATTERN_EASY_MID_BURST,
  WAVE_PATTERN_MIXED,
  WAVE_PATTERN_MIXED_BOMB,
  WAVE_PATTERN_QUICK_MIX,
};

static const uint8_t MID_HARD_POOL[] = {
  WAVE_PATTERN_BURST,
  WAVE_PATTERN_QUAD_BURST,
  WAVE_PATTERN_FIVE_BURST,
  WAVE_PATTERN_BURST_BOMB,
  WAVE_PATTERN_STAGGERED,
  WAVE_PATTERN_PUMP4,
  WAVE_PATTERN_SPLIT,
  WAVE_PATTERN_STAGGERED_BOMB,
  WAVE_PATTERN_PUMP_BOMB,
  WAVE_PATTERN_FEINT,
  WAVE_PATTERN_EASY_MID,
  WAVE_PATTERN_EASY_MID_BURST,
  WAVE_PATTERN_MIXED,
  WAVE_PATTERN_MIXED_BOMB,
  WAVE_PATTERN_QUICK_MIX,
  WAVE_PATTERN_DOUBLE_BURST,
  WAVE_PATTERN_MID_HARD,
  WAVE_PATTERN_MID_HARD_BURST,
};

static const uint8_t HARD_POOL[] = {
  WAVE_PATTERN_SINGLE,
  WAVE_PATTERN_BURST,
  WAVE_PATTERN_QUAD_BURST,
  WAVE_PATTERN_FIVE_BURST,
  WAVE_PATTERN_STAGGERED,
  WAVE_PATTERN_PUMP4,
  WAVE_PATTERN_SPLIT,
  WAVE_PATTERN_STAGGERED_BOMB,
  WAVE_PATTERN_MIXED,
  WAVE_PATTERN_MIXED_BOMB,
  WAVE_PATTERN_QUICK_MIX,
  WAVE_PATTERN_DOUBLE_BURST,
  WAVE_PATTERN_MID_HARD,
  WAVE_PATTERN_MID_HARD_BURST,
  WAVE_PATTERN_HARD_MIX,
  WAVE_PATTERN_HARD_CHAIN,
};

static const wave_difficulty_profile_t *get_profile(difficulty_t difficulty) {
  if (difficulty < EASY || difficulty > HARD) {
    difficulty = EASY;
  }

  return &DIFFICULTY_PROFILES[difficulty];
}

static uint32_t rand_gap(uint32_t *prng_seed) {
  return WAVE_GAP_MIN_TICKS +
         (uint32_t)(game_rand(prng_seed) % (WAVE_GAP_MAX_TICKS - WAVE_GAP_MIN_TICKS + 1));
}

static uint8_t pick_wave(difficulty_t difficulty, uint8_t wave_count, uint32_t *prng_seed) {
  const uint8_t *pool = EASY_POOL;
  uint8_t count = ARRAY_COUNT(EASY_POOL);

  if (difficulty == EASY_MID) {
    pool = EASY_MID_POOL;
    count = ARRAY_COUNT(EASY_MID_POOL);
  }
  else if (difficulty == MID) {
    pool = MID_POOL;
    count = ARRAY_COUNT(MID_POOL);
  }
  else if (difficulty == MID_HARD) {
    pool = MID_HARD_POOL;
    count = ARRAY_COUNT(MID_HARD_POOL);
  }
  else if (difficulty == HARD) {
    pool = HARD_POOL;
    count = ARRAY_COUNT(HARD_POOL);
  }

  for (uint8_t i = 0; i < count; i++) {
    uint8_t selected = pool[game_rand(prng_seed) % count];
    if (selected < wave_count) return selected;
  }

  return WAVE_PATTERN_SINGLE < wave_count ? WAVE_PATTERN_SINGLE : 0;
}

static void next_wave(wave_runtime_t *runtime, difficulty_t difficulty,
                      uint8_t wave_count, uint32_t current_tick, uint32_t *prng_seed) {
  runtime->current_wave = pick_wave(difficulty, wave_count, prng_seed);
  runtime->current_event = 0;
  runtime->next_wave_start_tick = current_tick + rand_gap(prng_seed);
  runtime->wave_start_tick = runtime->next_wave_start_tick;
}

void waves_init(wave_runtime_t *runtime, uint32_t current_tick) {
  if (runtime == NULL) return;

  runtime->current_wave = WAVE_PATTERN_INTRO;
  runtime->current_event = 0;
  runtime->wave_start_tick = current_tick + WAVE_FIRST_DELAY_TICKS;
  runtime->next_wave_start_tick = runtime->wave_start_tick;
  runtime->status = WAVE_STATUS_IDLE;
}

wave_status_t waves_update(wave_runtime_t *runtime, const wave_t *waves,
                           uint8_t wave_count, difficulty_t difficulty,
                           uint32_t current_tick,
                           uint32_t *prng_seed,
                           wave_spawn_request_t *out_request, bool *has_request) {
  if (has_request != NULL) *has_request = false;

  if (runtime == NULL || waves == NULL || out_request == NULL ||
      has_request == NULL || wave_count == 0) {
    if (runtime != NULL) runtime->status = WAVE_STATUS_ERROR;
    return WAVE_STATUS_ERROR;
  }

  if (runtime->status == WAVE_STATUS_ERROR) {
    return runtime->status;
  }

  runtime->status = WAVE_STATUS_RUNNING;

  if (runtime->current_wave >= wave_count) {
    runtime->current_wave = pick_wave(difficulty, wave_count, prng_seed);
  }

  while (true) {
    const wave_t *wave = &waves[runtime->current_wave];

    if (wave->event_count == 0 || wave->events == NULL) {
      runtime->status = WAVE_STATUS_ERROR;
      return runtime->status;
    }

    if (current_tick < runtime->next_wave_start_tick) {
      return runtime->status;
    }

    if (runtime->current_event >= wave->event_count) {
      next_wave(runtime, difficulty, wave_count, current_tick, prng_seed);
      continue;
    }

    const wave_event_t *event = &wave->events[runtime->current_event];
    if (current_tick - runtime->wave_start_tick < event->delay_ticks) {
      return runtime->status;
    }

    *out_request = event->request;
    *has_request = true;
    runtime->current_event++;
    runtime->wave_start_tick = current_tick;
    return runtime->status;
  }

  return runtime->status;
}

const wave_t *waves_get_predefined(wave_pattern_t pattern) {
  if (pattern >= WAVE_PATTERN_COUNT) return NULL;

  return &PREDEFINED_WAVES[pattern];
}

const motion_profile_t *waves_get_difficulty_motion(difficulty_t difficulty) {
  return &get_profile(difficulty)->motion;
}

void waves_pause_tick(wave_runtime_t *runtime) {
  if (runtime == NULL) return;

  runtime->wave_start_tick++;
  runtime->next_wave_start_tick++;
}
