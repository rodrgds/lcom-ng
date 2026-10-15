/**
 * @file dispatcher.h
 * @author Bernardo Santos (bernardo.ots@fe.up.pt)
 * @brief Game state handling and event dispatching
 * @version 0.1
 * @date 2021-01-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __DISPATCHER_H
#define __DISPATCHER_H

#include <lcom/lcf.h>

#include "constants.h"

/** @defgroup dispatcher Dispatcher
 * @{
 *
 * Game state handling and event dispatching
 */

/**
 * @brief Get the game state value.
 * 
 * @return the game state 
 */
state_t get_state();

/**
 * @brief Game state machine.
 * 
 * @param evt current event
 */
void game_state(event_t evt);

/** @} end of dispatcher */

#endif
