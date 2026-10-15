#ifndef _SESSION_TIMER_H_
#define _SESSION_TIMER_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @file session_timer.h
 * @brief Declares an accumulated gameplay session timer.
 */

/**
 * @brief Timer that accumulates elapsed gameplay ticks while running.
 */
typedef struct {
  /** @brief Total accumulated ticks. */
  uint32_t elapsed_ticks;

  /** @brief Last raw tick used to update elapsed_ticks. */
  int last_tick;

  /** @brief Whether elapsed time is currently accumulating. */
  bool running;
} GameSessionTimer;

/**
 * @brief Reset a session timer to zero elapsed ticks.
 *
 * @param timer Timer to reset.
 * @param current_tick Raw tick used as the new timing anchor.
 */
void session_timer_reset(GameSessionTimer *timer, int current_tick);

/**
 * @brief Start accumulating elapsed ticks.
 *
 * @param timer Timer to start.
 * @param current_tick Raw tick used as the new timing anchor.
 */
void session_timer_start(GameSessionTimer *timer, int current_tick);

/**
 * @brief Pause elapsed tick accumulation.
 *
 * @param timer Timer to pause.
 * @param current_tick Current raw tick, used for a final update before pausing.
 */
void session_timer_pause(GameSessionTimer *timer, int current_tick);

/**
 * @brief Update accumulated ticks if the timer is running.
 *
 * @param timer Timer to update.
 * @param current_tick Current raw tick.
 */
void session_timer_update(GameSessionTimer *timer, int current_tick);

/**
 * @brief Read accumulated elapsed ticks.
 *
 * @param timer Timer to inspect.
 * @return Accumulated ticks, or 0 for an invalid timer.
 */
uint32_t session_timer_ticks(const GameSessionTimer *timer);

/**
 * @brief Read accumulated elapsed time in whole seconds.
 *
 * @param timer Timer to inspect.
 * @return Accumulated seconds, or 0 for an invalid timer.
 */
uint32_t session_timer_seconds(const GameSessionTimer *timer);

#endif /* _SESSION_TIMER_H_ */
