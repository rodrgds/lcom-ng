#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#define MAX_INTERRUPTS 32

#include "scene.h"

/** @defgroup interrupt interrupt
 * @{
 * Functions for controlling the interruptions
 */

/**
 * @brief Initiates the interrupt controller.
 */

void int_init();

/**
 * @brief Sets the interruption controller mode.
 *
 * @param new_mode The mode to set the controller to.
 * @return The number of lines cleared.
 */

void int_setMode(kInterruptMode new_mode);

/**
 * @brief Subscribes an interruption.
 *
 * @param irq_line The IRQ line of the hardware.
 * @param policy The policy to apply to the interruption.
 * @param callback The callback of the interrupt (currently unused, left for legacy reasons)
 *
 * @return 0 on success, other on fail.
 */

int  int_subscribe(int irq_line, int policy, void (*callback)());

/**
 * @brief Unsubscribes an interruption.
 *
 * @param subpos The number of the interruption.
 *
 * @return 0 on success, 1 or 2 on failure (check .c)
 */

int  int_unsubscribe(int subpos);

/**
 * @brief Starts the interruption handler.
 *
 * @return 0 on termination.
 */

int  int_start_handler();

/**
 * @brief Stops the interruption handler.
 */

void int_stop_handler();

/**@}*/

#endif /* INTERRUPT_H_ */