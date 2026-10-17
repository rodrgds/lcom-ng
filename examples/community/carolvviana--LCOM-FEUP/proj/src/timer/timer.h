#ifndef __TIMER_H
#define __TIMER_H

#include <stdbool.h>
#include <stdint.h>


/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int(timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(timer_unsubscribe_int)();

/**
 * @brief Timer 0 interrupt handler
 * Used to increment counter
 */
void(timer_int_handler)();


#endif /* __TIMER_H */
