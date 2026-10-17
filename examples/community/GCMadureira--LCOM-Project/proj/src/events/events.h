/** 
 * @file events.h
 * @brief Header file for @ref events.c
 * 
 * This file declares all the functions for the input events' logic.
 */


#ifndef _EVENTS_PROJ_H_
#define _EVENTS_PROJ_H_

#include <lcom/lcf.h>

#include "../macros/scancodes.h"
#include "../drivers/drivers.h"
#include "../model/structs.h"

/**
 * @brief Handles a mouse event
 * 
 * Starts by validating and handling an interruption from the kbc, and then adds the mouse event to the @ref input_event_list when all three packet bytes are received.
 */
void (event_handle_mouse)();

/**
 * @brief Handles a keyboard event
 * 
 * Starts by validating and handling an interruption from the kbc, and then adds the keyboard event to the @ref input_event_list when all the scancodes are received.
 */
void (event_handle_keyboard)();

/**
 * @brief Gets the next event on the @ref input_event_list
 * 
 * Loads the next event into the next_event parameter passed
 * 
 * @param next_event Pointer to the input_event variable that will be loaded
 * @return 0 if an event was loaded, 1 otherwise
 */
int (events_get_next)(input_event* next_event);

/**
 * @brief Removes all the events from the @ref input_event_list
 * 
 * Iterates through all the events on the @ref input_event_list and releases the allocated memory of each one.
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (events_clear)();

#endif /*_EVENTS_PROJ_H_ */
