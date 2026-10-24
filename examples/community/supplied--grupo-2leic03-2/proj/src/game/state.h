/**
 * @file state.h
 * @brief Game state machine and transitions.
 */
#ifndef STATE_H
#define STATE_H

#include "../input/input.h"
#include "../graphics/ui.h"

/**
 * @brief Enumeration of possible game states.
 */
typedef enum {
  STATE_MENU,
  STATE_PLAYING,
  STATE_PAUSED,
  STATE_GAME_OVER,
  STATE_EXIT
} GameState;

struct Game;

/**
 * @brief Initializes the state machine.
 */
void state_init(void);

/**
 * @brief Updates the logic for the current state.
 * @param game Pointer to the Game state.
 * @param input Pointer to the current InputState.
 */
void state_update(struct Game *game, const InputState *input);

/**
 * @brief Draws the current state.
 * @param game Pointer to the Game state.
 * @param input Pointer to the current InputState.
 */
void state_draw(struct Game *game, const InputState *input);

/**
 * @brief Draws the Heads-Up Display during gameplay.
 * @param game Pointer to the Game state.
 */
void state_draw_playing_hud(const struct Game *game);

#endif
