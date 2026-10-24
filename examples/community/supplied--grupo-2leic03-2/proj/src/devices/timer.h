/**
 * @file timer.h
 * @brief Timer device interface.
 */
#ifndef PROJECT_TIMER_H
#define PROJECT_TIMER_H

#include <stdint.h>

/**
 * @brief Subscribes timer interrupts.
 * @param irq_set Pointer to store the IRQ bitmask.
 * @return 0 upon success, non-zero otherwise.
 */
int project_timer_subscribe(uint32_t *irq_set);

/**
 * @brief Unsubscribes timer interrupts.
 * @return 0 upon success, non-zero otherwise.
 */
int project_timer_unsubscribe(void);

#endif
