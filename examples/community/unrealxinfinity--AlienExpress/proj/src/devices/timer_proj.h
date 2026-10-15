#ifndef TIMER_PROJ_H
#define TIMER_PROJ_H

#include "../macros/includes.h"


/**
 * @brief Subscribes the timer device, setting into the interrupt mask the respective timer hook id bit to 1
 * @return Returns the bit number that was set if successfull or 0xFF if unsuccessfull
 */
uint8_t (timer_subscribe)();
/**
 * @brief Unsubscribes the timer device, setting into the interrupt mask the respective timer hook id bit to 0
 * @return Returns 0 if successfull and 1 if otherwise
 */
int (timer_unsubscribe)();
/**
 * @brief Handles the interruupts of the timer, incrementing the timer count variable
 */
void (timer_int_handler_proj)();
/**
 * @brief Sets the timer frequency to a certain frequency value
 * @param timer The timer to set the frequency
 * @param freq The value of the frequency to be set
 * @return Returns 0 if successfull and 1 if otherwise
 */
int (timer_set_frequency_proj)(uint8_t timer, uint32_t freq);
/**
 * @brief Gets the current timer configuration
 * @param timer The timer to get the configuration from
 * @return Returns the current timer configuration if successfull and 0xFFFF if otherwise
 */
uint32_t timer_get_config(uint8_t timer);

#endif
