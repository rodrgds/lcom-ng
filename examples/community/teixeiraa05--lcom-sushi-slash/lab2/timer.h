#ifndef _LAB2_TIMER_H_
#define _LAB2_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @file timer.h
 * @brief Declares timer subscription and project tick helpers.
 */

/**
 * @brief Get the current project timer interrupt counter.
 *
 * @return Number of timer interrupts handled by the project timer handler.
 */
int get_int_counter();

/**
 * @brief Set the project timer interrupt counter.
 *
 * @param new_value Counter value to store.
 * @return 0 on success, non-zero on failure.
 */
int set_int_counter(int new_value);

/**
 * @brief Project-level timer interrupt handler.
 */
void timer_ih_proj(void);

/**
 * @brief Get the number of project timer ticks elapsed.
 *
 * @return Current timer tick count.
 */
uint32_t timer_get_ticks(void);

/**
 * @brief Check whether a number of milliseconds has elapsed.
 *
 * @param ms Milliseconds to test against the project timer.
 * @return true when the interval has elapsed, false otherwise.
 */
bool timer_elapsed(int ms);

#endif /* _LAB2_TIMER_H_ */
