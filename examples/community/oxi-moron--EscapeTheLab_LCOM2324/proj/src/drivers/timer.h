#ifndef G6_TIMER_H
#define G6_TIMER_H

#include <lcom/lcf.h>

/**
 * @brief Gets the current frequency of Timer 0.
 * @param freq Address where the frequency will be stored.
 * @return 0 if successful, non-zero otherwise.
 */
int (timer_get_frequency)(uint32_t* freq);

/**
 * @brief Sets the frequency of a timer.
 * @param timer Timer to set the frequency.
 * @param freq Frequency to be set.
 * @return 0 if successful, non-zero otherwise.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes to the timer interrupts with the IRQ_REENABLE policy.
 * @param bit_no Bit number to be used in the timer hook ID.
 * @return 0 if successful, non-zero otherwise.
 */
int (timer_subscribe_int)(uint8_t* bit_no);

/**
 * @brief Unsubscribes to the timer interrupts.
 * @return 0 if successful, non-zero otherwise.
 */
int (timer_unsubscribe_int)();

/**
 * @brief Processes timer interrupts by incrementing a counter.
 */
void (timer_ih) ();

/**
 * @brief Writes a command to the timers command register.
 * @param cmd Command to be written.
 * @return 0 if successful, non-zero otherwise.
 */
int timer_write_cmd(uint8_t cmd);

/**
 * @brief Reads the status of a timer.
 * @param st Address where the status will be stored.
 * @param timer Timer to read status from.
 * @return 0 if successful, non-zero otherwise.
 */
int timer_read_status(uint8_t* st, uint8_t timer);

#endif //G6_TIMER_H
