/**
 * @file timer_handler.h
 * @brief Declares the function for handling timer-based game updates based on the game state.
 */

#ifndef TIMER_HANDLER_H
#define TIMER_HANDLER_H

#include <stdio.h>
#include "controllers/timer/timer.h"
#include <lcom/timer.h>
#include "manager.h"
#include "gameState.h"

/**
 * @brief Handles events triggered by the timer interrupt, updating game logic depending on the current state.
 *
 * This function is typically called on each timer tick to update animations, entity states,
 * and other time-dependent aspects of the game.
 *
 * @param game_state The current game state used to determine what should be updated or drawn.
 */
void timer_event_handler(gameState game_state);

#endif // TIMER_HANDLER_H

