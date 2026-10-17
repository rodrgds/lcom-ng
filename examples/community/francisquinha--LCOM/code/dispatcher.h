#ifndef __DISPATCHER_H
#define __DISPATCHER_H

/** @file dispatcher.h
 * @brief file containing the declarations of the functions in dispatcher.c
 */

/** @defgroup dispatcher dispatcher
 * @{
 * @brief Functions for handling interruptions
 */

/** @brief Pointer to function that handles interruptions */
typedef unsigned short (*Handler)(int int_irq, long *arg);

/**
 * @brief Subscribes and enables a given interruption
 *
 * @param irq_line IRQ line of the hardware controler whose interruptions we want to subscribe
 * @param policy policy we want to subscribe for the interruption
 * @param hook_id value used both for input to the subscription and output from the subscription
 * @param enable 1 to enable subscription and 0 to disable it
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int subscribe_int(int irq_line, int policy, int *hook_id, unsigned short enable);

/**
 * @brief Unsubscribes a given interruption
 *
 * @param hook_id identifies the subscription to be canceled
 *
 * @return Returns 0 upon success and non-zero otherwise
 */
int unsubscribe_int(int *hook_id);

/**
 * @brief Runs an interruption loop
 *
 * Calls the interruption handler at every suitable interruption
 * Calls the interruption application handler at every suitable interruption
 * Uses the interruption application handler output to exit the loop
 *
 * @param irq_set bit mask that identifies the interruption(s)
 * @param int_handler function that handles the interruption(s)
 * @param app_handler function that handles the interruption(s) according to the application
 * @param arg arguments for the function int_handler
 *
 * @return Returns 0 upon success and non-zero otherwise
 */
void interrupt_loop(int irq_set, Handler int_handler, Handler app_handler, long *arg);

/** @} end of dispatcher*/

#endif /* __DISPATCHER_H */
