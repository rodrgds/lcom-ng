#ifndef MAINGAME_H
#define MAINGAME_H

/** @defgroup mainGame mainGame
 * @{
 *
 * Functions to specify the game logic (what do kbd/mouse events do, what happens every frame and what happends on startup)
 */

#include "gameEngine.h"

#define MAX_HP 3

/**
 * @brief Called on the start of the game
 * Initializes the game's GameObjects
 *
 * @param events - event queue
 * @param startState - pointer to the stateMachine's startState
 *
 * @return none
 */
void startup(ev_queue_st* events, gameState_st* startState);

/**
 * @brief Called every frame
 * Updates the game's GameObjects, checks collisions
 *
 * @param events - event queue
 * @param currentState - pointer to the stateMachine's current state
 *
 * @return none
 */
void update(ev_queue_st* events, gameState_st* currentState);

/**
 * @brief Process a given keyboard event
 *
 * @param currentState - pointer to the stateMachine's current state
 * @param event - event to process
 * @param gameEvents - pointer to event queue
 *
 * @return none
 */
void onKbdEvent(gameState_st* currentState, event_st event, ev_queue_st* gameEvents);
/**
 * @brief Process a given mouse event
 *
 * @param currentState - pointer to the stateMachine's current state
 * @param event - event to process
 * @param gameEvents - pointer to event queue
 *
 * @return none
 */
void onMouseEvent(gameState_st* currentState, event_st event, ev_queue_st* gameEvents);

/**
 * @brief Checks for collisions between GameObjects and acts accordingly
 *
 * @param objects - ObjLayer array
 *
 * @return none
 */
void checkCollisions(ObjLayer* objects);

/**
 * @brief Checks for collisions between 2 GameObjects
 * @param obj1 - GameObject 1
 * @param obj2 - GameObject 2
 *
 * @return true if the objects are coliding, false otherwise
 */
bool isColliding(GameObject* obj1, GameObject* obj2);

/**
 * @brief Updates the gameObjects positions, based on their velocity and the state
 * @param currentState - pointer current state
 *
 * @return none
 */
void updatePositions(gameState_st* currentState);

/**
 * @brief Resets the game to start playing again
 * @param gameEvents - pointer to event queue
 *
 * @return none
 */
void resetGame(ev_queue_st* gameEvents);

/**
 * @brief Display hp in hearts
 * @param hp - current hp
 * @param gameObjects - Game objects array
 *
 * @return none
 */
void displayHP(int hp, ObjLayer* gameObjects);
#endif

/** @} end of mainGame*/
