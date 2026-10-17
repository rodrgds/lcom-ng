#pragma once

#include <lcom/lcf.h>
#include "errors.h"

typedef enum {
  INIT,
  MENU,
  GAME,
  MULTIPLAYER
} state_t;

typedef struct {
  state_t state;
  void (*init)();
  void (*cleanup)();
} state_handler;

/**
* @brief Initializes the game's state machine
*/
int (state_init)();

/**
* @brief Terminates the game's state machine
*/
int (state_cleanup)();

/**
 * @brief Registers a state to the state machine
 * @param state the state to register
 * @param init the function to execute when the state is entered
 * @param cleanup the function to execute when the state is left
 */
int (state_register)(state_t state, void (*init)(), void (*cleanup)());

/**
 * @brief Change the state of the machine
 * @param state the state to transition to
 */
int (state_set)(state_t state);

/**
 * @brief Get the current state
 */
state_t (state_get)();
