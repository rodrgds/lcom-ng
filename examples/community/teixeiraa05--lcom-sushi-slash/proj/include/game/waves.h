#ifndef __WAVES_H
#define __WAVES_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file waves.h
 * @brief Declares the wave scheduling API used by gameplay.
 *
 * The wave module decides when and what should be spawned, but it does not
 * create sushi pieces directly. Gameplay consumes wave_spawn_request_t values
 * and turns them into concrete game objects.
 */

/** @brief Sushi type value meaning that gameplay should choose the type. */
#define WAVE_RANDOM_SUSHI_TYPE 0xFF

/**
 * @brief Runtime difficulty used for gameplay wave pacing and motion.
 */
typedef enum difficulty {
  /** @brief Introductory difficulty with slower, simpler waves. */
  EASY,

  /** @brief Bridge difficulty between easy and mid. */
  EASY_MID,

  /** @brief Standard difficulty. */
  MID,

  /** @brief Bridge difficulty between mid and hard. */
  MID_HARD,

  /** @brief Highest standard difficulty. */
  HARD
} difficulty_t;

/**
 * @brief Identifies a predefined wave pattern.
 *
 * Pattern names describe the structure of the wave. The concrete events for
 * each pattern are provided by the wave implementation and interpreted by
 * waves_update().
 */
typedef enum {
  /** @brief Intro wave: three sushi-only events, starting with one piece. */
  WAVE_PATTERN_INTRO,

  /** @brief A small wave that asks gameplay to spawn one item. */
  WAVE_PATTERN_SINGLE,

  /** @brief A small wave that asks gameplay to spawn one bomb. */
  WAVE_PATTERN_BOMB,

  /** @brief A compact wave that asks gameplay to spawn multiple items together.
   */
  WAVE_PATTERN_BURST,

  /** @brief A compact wave that asks gameplay to spawn four sushi together. */
  WAVE_PATTERN_QUAD_BURST,

  /** @brief A compact wave that asks gameplay to spawn five sushi together. */
  WAVE_PATTERN_FIVE_BURST,

  /** @brief A compact wave mixing a small sushi burst with one bomb. */
  WAVE_PATTERN_BURST_BOMB,

  /** @brief A wave whose items are separated by short delays. */
  WAVE_PATTERN_STAGGERED,

  /** @brief A quick four-step stream of single sushi pieces. */
  WAVE_PATTERN_PUMP4,

  /** @brief A split rhythm that alternates single and paired sushi. */
  WAVE_PATTERN_SPLIT,

  /** @brief A staggered wave that includes a bomb. */
  WAVE_PATTERN_STAGGERED_BOMB,

  /** @brief A quick four-step stream with one bomb mixed in. */
  WAVE_PATTERN_PUMP_BOMB,

  /** @brief A one-bomb feint followed by sushi pressure. */
  WAVE_PATTERN_FEINT,

  /** @brief A bridge wave between easy and normal pressure. */
  WAVE_PATTERN_EASY_MID,

  /** @brief A burst bridge wave between easy and normal pressure. */
  WAVE_PATTERN_EASY_MID_BURST,

  /** @brief A wave mixing different spawn kinds, such as sushi and bombs. */
  WAVE_PATTERN_MIXED,

  /** @brief A stronger mixed wave with simultaneous sushi and bombs. */
  WAVE_PATTERN_MIXED_BOMB,

  /** @brief A quick mixed wave with alternating sushi and bombs. */
  WAVE_PATTERN_QUICK_MIX,

  /** @brief A double burst wave separated by a short pause. */
  WAVE_PATTERN_DOUBLE_BURST,

  /** @brief A bridge wave between normal and hard pressure. */
  WAVE_PATTERN_MID_HARD,

  /** @brief A burst bridge wave between normal and hard pressure. */
  WAVE_PATTERN_MID_HARD_BURST,

  /** @brief A hard mixed wave with heavier bomb pressure. */
  WAVE_PATTERN_HARD_MIX,

  /** @brief A hard quick chain with several close-spaced events. */
  WAVE_PATTERN_HARD_CHAIN,

  /** @brief Number of predefined wave patterns. */
  WAVE_PATTERN_COUNT
} wave_pattern_t;

/**
 * @brief Type of spawn requested by a wave event.
 *
 * This only describes what gameplay should spawn. It does not create or own any
 * sushi, bomb, sprite, or gameplay object.
 */
typedef enum {
  /** @brief Request a regular sushi piece. */
  WAVE_SPAWN_REGULAR,

  /** @brief Request a bomb hazard. */
  WAVE_SPAWN_BOMB
} wave_spawn_kind_t;

/**
 * @brief Motion limits requested for spawned objects.
 *
 * Values use the fixed-point physics representation from macros/physics.h.
 */
typedef struct {
  /** @brief Minimum vertical velocity for the spawned object, in fixed-point units. */
  int min_vy;

  /** @brief Maximum vertical velocity for the spawned object, in fixed-point units. */
  int max_vy;

  /** @brief Gravity applied to the spawned object, in fixed-point units per tick. */
  int gravity;

  /** @brief Minimum horizontal velocity for the spawned object, in fixed-point units. */
  int min_vx;

  /** @brief Maximum horizontal velocity for the spawned object, in fixed-point units. */
  int max_vx;
} motion_profile_t;

/**
 * @brief Runtime state of the wave scheduler.
 */
typedef enum {
  /** @brief Scheduler is initialized but not currently advancing a wave. */
  WAVE_STATUS_IDLE,

  /** @brief Scheduler is actively processing wave events. */
  WAVE_STATUS_RUNNING,

  /** @brief Scheduler encountered invalid input. */
  WAVE_STATUS_ERROR
} wave_status_t;

/**
 * @brief Request emitted by the wave scheduler and consumed by gameplay.
 *
 * Gameplay is responsible for turning this request into actual objects. For
 * example, a WAVE_SPAWN_REGULAR request may become one or more sushi_t values,
 * while a WAVE_SPAWN_BOMB request may become one or more hazards.
 */
typedef struct {
  /** @brief Kind of object or encounter requested. */
  wave_spawn_kind_t kind;

  /** @brief Sushi asset/type to use, or WAVE_RANDOM_SUSHI_TYPE for random. */
  uint8_t sushi_type;

  /** @brief Number of objects requested by this spawn. */
  uint8_t amount;

  /** @brief Score awarded if the requested object is successfully cut. */
  uint32_t score_value;
} wave_spawn_request_t;

/**
 * @brief Timed spawn request inside a wave.
 *
 * delay_ticks is relative to the previous event. For the first event in a
 * wave, it is relative to the wave start.
 */
typedef struct {
  /** @brief Tick delay before this event should be emitted. */
  uint32_t delay_ticks;

  /** @brief Spawn request emitted when the delay is reached. */
  wave_spawn_request_t request;
} wave_event_t;

/**
 * @brief Definition of one wave pattern instance.
 */
typedef struct {
  /** @brief Identifier of the predefined pattern represented by this wave. */
  wave_pattern_t pattern;

  /** @brief Ordered list of timed spawn events. */
  const wave_event_t *events;

  /** @brief Number of entries in events. */
  uint8_t event_count;

} wave_t;

/**
 * @brief Mutable state used while waves are being processed.
 */
typedef struct {
  /** @brief Index of the wave currently being processed. */
  uint8_t current_wave;

  /** @brief Index of the next event to emit within the current wave. */
  uint8_t current_event;

  /** @brief Tick used as the anchor for the current event delay. */
  uint32_t wave_start_tick;

  /** @brief Earliest tick at which the next wave may start. */
  uint32_t next_wave_start_tick;

  /** @brief Current scheduler status. */
  wave_status_t status;

} wave_runtime_t;

/**
 * @brief Initialize a wave runtime.
 *
 * @param runtime Runtime object to initialize.
 * @param current_tick Current timer tick.
 */
void waves_init(wave_runtime_t *runtime, uint32_t current_tick);



/**
 * @brief Advance the wave scheduler and return a due spawn request.
 *
 * @param runtime Mutable scheduler state.
 * @param waves Array of wave definitions to process.
 * @param wave_count Number of entries in waves.
 * @param difficulty Current gameplay difficulty used for random selection.
 * @param current_tick Current timer tick.
 * @param prng_seed Pointer to the PRNG seed state used for wave selection.
 * @param out_request Output parameter filled with the due spawn request.
 * @param has_request Output parameter set to true if a request is due, false otherwise.
 * 
 * @return Scheduler status indicating the result of the update.
 */
wave_status_t waves_update(wave_runtime_t *runtime, const wave_t *waves,
                           uint8_t wave_count, difficulty_t difficulty,
                           uint32_t current_tick,
                           uint32_t *prng_seed,
                           wave_spawn_request_t *out_request, bool *has_request);

/**
 * @brief Get a predefined wave definition by pattern id.
 *
 * @param pattern Predefined pattern to retrieve.
 *
 * @return Pointer to the predefined wave, or NULL if pattern is invalid.
 */
const wave_t *waves_get_predefined(wave_pattern_t pattern);

/**
 * @brief Get the motion profile used by a difficulty.
 *
 * @param difficulty Difficulty to inspect.
 *
 * @return Pointer to a static motion profile. Invalid values fall back to easy.
 */
const motion_profile_t *waves_get_difficulty_motion(difficulty_t difficulty);

/**
 * @brief Pauses the wave scheduler by shifting its start ticks forward by 1.
 *
 * @param runtime Wave scheduler runtime to pause.
 */
void waves_pause_tick(wave_runtime_t *runtime);

#endif /* __WAVES_H */
