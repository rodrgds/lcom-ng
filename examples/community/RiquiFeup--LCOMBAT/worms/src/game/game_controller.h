#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "game.h"
#include "../engine/event/event_queue.h"

/**
 * @brief Handles a single event (timer, keyboard, mouse) for the game state.
 * @param context The current game context.
 * @param event The event to process.
 */
void game_controller_handle_event(GameContext *context, const GameEvent *event);

#endif /* GAME_CONTROLLER_H */
