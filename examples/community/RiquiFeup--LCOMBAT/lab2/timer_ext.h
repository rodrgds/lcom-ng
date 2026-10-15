#ifndef _LCOM_TIMER_EXT_H_
#define _LCOM_TIMER_EXT_H_

#include <stdint.h>

/**
 * @brief Gets the current value of the timer counter
 * @return Returns the current number of ticks
 */
int (timer_get_counter)();

/**
 * @brief Resets the timer counter to 0
 */
void (timer_reset_counter)();

#endif /* _LCOM_TIMER_EXT_H_ */
