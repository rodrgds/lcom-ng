#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "../game.h"

typedef enum {
    STATE_SPLASH, 
    STATE_MENU,
    STATE_CONTROLS,
    STATE_GAME,
    STATE_AIM,
    STATE_FLIGHT,
    STATE_EXPLOSION,
    STATE_TURN_END,
    STATE_PAUSE,
    STATE_GAME_OVER,
    STATE_QUIT,
    STATE_COUNT /* Keeps the number of states centralized. */
} GameState;

/**
 * @brief Runs the main game state machine.
 * @param context Game context used by all states. NULL is rejected.
 */
void state_machine_run(GameContext *context);

#endif
