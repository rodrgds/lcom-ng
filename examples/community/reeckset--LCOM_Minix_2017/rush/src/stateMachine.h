#ifndef STATEMACHINE_H
#define STATEMACHINE_H

/** @defgroup stateMachine stateMachine
 * @{
 *
 * Functions to manage game state machine
 */

#include "ev_queue.h"
#include "gameObjects.h"

typedef enum {

  MAIN_MENU = 0,
  GAME_PLAY = 1,
  PAUSE_MENU = 2,
  GAME_OVER = 3,
  GAME_EXIT = 4

} state_st;

typedef struct {
  int currentScore;
  int currentHP;
  state_st state;
  ObjLayer* objects;
  bool isKeyDown_SPACE;

} gameState_st;

/**
 * @brief Set the start state of the state machine
 *
 * @param state - desired start state pointer
 * @return none
 */
void startStateMachine(gameState_st* state);

/**
 * @brief create a new gameState
 *
 * @param state - state type
 * @param score - score
 * @return new state pointer
 */
gameState_st* newGameState(state_st state, int score, int hp);

/**
 * @brief updates the current state machine's state to the one with the type given
 *
 * @param newGameState - new current state
 * @return none
 */
void updateCurrentGameState(state_st newGameState);

/**
 * @brief Get current gameState
 *
 * @return current gameState pointer
 */
gameState_st* getCurrentState();

/**
 * @brief handle the given event for the current state
 *
 * @param currentState - current state pointer
 * @param event - event to handle
 * @param events - event queue pointer
 * @return new state after processing
 */
gameState_st* eventHandler(gameState_st* currentState, event_st event, ev_queue_st* events);

/**
 * @brief Get state based on the state type given
 *
 * @param s - state type
 *
 * @return asociated gameState pointer
 */
gameState_st* getState(state_st s);

/**
 * @brief start the stateMachine (basically start the game logic)
 *
 * @param events - event queue pointer
 * @param maxHP - maxHP until game over
 * @return none
 */
void startStates(ev_queue_st* events, int maxHP);

/**
 * @brief Free the memory occupied by the state machine
 *
 * @return none
 */
void stateMachine_free();

/**
 * @brief Free the memory occupied by a game state
 *
 * @param state - state pointer to free
 * @return none
 */
void gameState_free(gameState_st* state);
/** @} end of stateMachine*/

#endif
