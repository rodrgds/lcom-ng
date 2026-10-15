#pragma once

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Tracks timer interrupt cadence for fixed-rate game updates.
 */
typedef struct {
    int ticks_per_update; /**< Number of timer ticks between game updates. */
    int tick_counter; /**< Ticks accumulated since the last game update. */
    uint32_t total_ticks; /**< Total ticks elapsed since initialisation. */
    bool should_update; /**< True on the tick that triggers a game update. */
    bool second_elapsed; /**< True each time a full second has passed. */
} TimerState;

/**
 * @brief Initialises a TimerState with the desired update frequency.
 * @param state TimerState to initialise.
 * @param ticks_per_update Number of timer interrupts between game logic updates.
 */
void timerStateInit(TimerState *state, int ticks_per_update);

/**
 * @brief Called once per timer interrupt to advance counters and set update flags.
 * Sets should_update when tick_counter reaches ticks_per_update, and second_elapsed
 * when total_ticks crosses a multiple of the timer frequency.
 * @param state TimerState to update.
 */
void timerInterpretInterrupt(TimerState *state);
