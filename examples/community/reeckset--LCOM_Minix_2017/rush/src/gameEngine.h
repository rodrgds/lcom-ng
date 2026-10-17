#ifndef GAMEENGINE_H
#define GAMEENGINE_H


/** @defgroup gameEngine gameEngine
 * @{
 *
 * Functions to manage the game engine such as receiving interrupts, processing events and drawing objects
 */

#include <stdbool.h>

#include "stateMachine.h"
#include "gameObjects.h"

/**
 * @brief Starts the game Engine, switching to graphic mode and calls the mainGameLoop().
 *
 * @return 0
 */
int startEngine();

/**
 * @brief Loops until the game reaches GAME_EXIT state. On each loop, checks for interrupts and calls interrupt handlers, and updates the game
 *
 * @return True if succesfull, false otherwise
 */
bool mainGameLoop();

/**
 * @brief Processes the events on a given ev_queue_st.
 *
 * @param events - event queue pointer
 * @param currentState - pointer to the current gameState
 *
 * @return pointer to current gameState after processing the events
 */
gameState_st* processEvents(ev_queue_st* events, gameState_st* currentState);

/**
 * @brief Draws all objects of the given state.
 *
 * @return None
 */
void drawFrame(gameState_st* currentState);

/**
 * @brief Checks if the cursor is inside of an object.
 * @param cursor - GameObject pointer of the cursor
 * @param obj2 - GameObject pointer of the object to check
 * @return true if cursor is inside of the object, false otherwise
 */
bool cursorIsInside(GameObject* cursor, GameObject* obj2);

/** @} end of gameEngine*/

#endif /* __GAMEENGINE_H */
