/**

@file timer.h
@brief This file contains the declarations of functions related to the PC's internal timer. */

#ifndef TIMER_H
#define TIMER_H

#include <lcom/lcf.h>

#include "i8254.h"

/**

@brief The global counter that stores the number of interrupts occurred by the timer. */
unsigned int global_counter;

/**
@brief Sets the frequency of a given timer.
@param timer The timer to be configured.
@param freq The desired frequency.
@return Return 0 upon success, 1 if error occurs */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);
/**
@brief Subscribes and enables Timer 0 interrupts.
@param bit_no The bit number associated with the timer's interrupt.
@return Return 0 upon success and non-zero otherwise. */ 
int (timer_subscribe_int)(uint8_t *bit_no);
/**
@brief Unsubscribes Timer 0 interrupts.
@return Return 0 upon success and non-zero otherwise. */ 
int (timer_unsubscribe_int)();
/**
@brief Timer 0 interrupt handler. */
void (timer_int_handler)();
/**
@brief Reads the configuration of a given timer in a 8-bit variable.
@param timer The timer to read.
@param st The pointer to an 8-bit variable that will store the timer's configuration.
@return Return 0 upon success and non-zero otherwise. */
int (timer_get_conf)(uint8_t timer, uint8_t *st);
/**
@brief Displays, in a friendly way, a timer configuration in the specified field, for a given timer.
@param timer The timer whose configuration is going to be displayed.
@param st The timer configuration.
@param field The field to be displayed in a friendly way.
@return Return 0 upon success and non-zero otherwise. */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

#endif //TIMER_H
