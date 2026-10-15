#ifndef __BOSS_H
#define __BOSS_H

#include <lcom/lcf.h> 

#include "sprite.h"
#include "sushi.h"
#include "combo.h"

/** @brief Total ticks available to complete any boss sequence. */
#define BOSS_TOTAL_TICKS 600

/** @brief Score awarded for each correctly completed boss key. */
#define BOSS_POINTS_PER_KEY 2

/**
 * @brief Runtime outcome of a boss encounter.
 */
typedef enum {
    /** @brief Boss encounter is still active. */
    BOSS_STATUS_ACTIVE,

    /** @brief Boss sequence was completed successfully. */
    BOSS_STATUS_SUCCESS,

    /** @brief Boss timer expired before the sequence was completed. */
    BOSS_STATUS_FAILURE
} boss_status_t;

/**
 * @brief Result of one typed key during a boss encounter.
 */
typedef enum {
    /** @brief Input did not match the expected key. */
    BOSS_INPUT_WRONG,

    /** @brief Input matched and advanced the sequence. */
    BOSS_INPUT_CORRECT,

    /** @brief Input matched and completed the sequence. */
    BOSS_INPUT_COMPLETE
} boss_input_result_t;

/**
 * @file boss.h
 * @brief Declares Boss Sushi runtime data.
 */

/**
 * @brief Runtime state for a boss encounter.
 */
typedef struct boss {
    /** @brief Physical boss sushi object shown on screen. */
    sushi_t sushi;

    /** @brief Required key sequence for defeating the boss. */
    combo_t combo;

    /** @brief Time remaining until the boss fight ends, in ticks. */
    uint32_t countdown;

    /** @brief Session tick at which the boss encounter started. */
    uint32_t start_tick;

    /** @brief Session tick at which the current key became active. */
    uint32_t current_key_start_tick;

    /** @brief Total ticks available for the whole encounter. */
    uint32_t total_ticks;

    /** @brief Ticks available for each individual key. */
    uint32_t per_key_ticks;

    /** @brief PRNG seed state specifically for boss key/position generation. */
    uint32_t prng_seed;
} boss_t;

/**
 * @brief Initialize a side-spawned boss sushi wave.
 *
 * @param combo Combo structure to track the boss's key sequence.
 * @param start_tick Session tick at which the boss begins.
 * @param total_ticks Total time available for the boss sequence.
 * @param per_key_ticks Time available for each key in the sequence.
 * @param prng_seed Pointer to the PRNG seed state.
 */
boss_t boss_init(combo_t combo,
                 uint32_t start_tick,
                 uint32_t total_ticks,
                 uint32_t per_key_ticks,
                 uint32_t *prng_seed);

/**
 * @brief Verify one key input against the current boss sequence key.
 *
 * @param boss Pointer to the boss to update.
 * @param input The character input to verify.
 * @param current_tick Current gameplay/session tick.
 * @return Result describing whether the input advanced or completed the sequence.
 */
boss_input_result_t boss_verify_combo(boss_t* boss, char input, uint32_t current_tick);

/**
 * @brief Move the boss sushi and evaluate sequence timers.
 *
 * @param boss Pointer to the boss to update.
 * @param current_tick Current gameplay/session tick.
 * @return Current boss encounter status.
 */
boss_status_t boss_update(boss_t* boss, uint32_t current_tick);

/**
 * @brief Get the remaining ticks for the whole boss encounter.
 *
 * @param boss Boss to inspect.
 * @param current_tick Current gameplay/session tick.
 * @return Remaining total ticks, or 0 when expired.
 */
uint32_t boss_total_ticks_left(const boss_t* boss, uint32_t current_tick);

/**
 * @brief Get the remaining ticks for the current key.
 *
 * @param boss Boss to inspect.
 * @param current_tick Current gameplay/session tick.
 * @return Remaining per-key ticks, or 0 when expired.
 */
uint32_t boss_key_ticks_left(const boss_t* boss, uint32_t current_tick);

/**
 * @brief Destroys the boss's resources, such as the sushi sprite.
 *
 * @param boss Pointer to the boss to destroy.
 */
void boss_destroy(boss_t* boss);

/**
 * @brief Checks if either the current key timer or overall boss timer has timed out.
 *
 * @param boss Boss to check.
 * @param current_tick Current timer tick.
 * @return true if timed out, false otherwise.
 */
bool boss_has_timed_out(const boss_t* boss, uint32_t current_tick);

/**
 * @brief Resets the boss sequence back to the first key, re-randomizing its position and timer.
 *
 * @param boss Boss to reset.
 * @param current_tick Current timer tick.
 */
void boss_reset_sequence(boss_t* boss, uint32_t current_tick);

#endif /* __BOSS_H */
